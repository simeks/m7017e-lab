#include "../shared/common.h"

#include "client.h"
#include "qt/mainwindow.h"
#include "../shared/configvalue.h"
#include "../shared/json.h"

Client::Client(MainWindow* window) :
    _window(window),
	_callback_handler(this)
{
	// Register callbacks
	_callback_handler.RegisterCallback("NET_WELCOME", &Client::OnWelcomeMsg);
	_callback_handler.RegisterCallback("NET_CHAT_MSG", &Client::OnChatMsg);
	_callback_handler.RegisterCallback("NET_SERVER_STATE", &Client::OnServerState);
	_callback_handler.RegisterCallback("NET_USER_STATE", &Client::OnUserState);
}
Client::~Client()
{

}

void Client::ConnectClicked()
{
    _socket = new QTcpSocket(this);
    _socket->connectToHost(_server_ip, _server_port);
	
	connect(_socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));
	connect(_socket, SIGNAL(disconnected()), this, SLOT(Disconnected()));

    if(_socket->waitForConnected(500))
    {
        _window->Connected();
        SendHelloMessage(_user_name);
    }
}

void Client::SetServerIp(QString serverIP)
{
    _server_ip = serverIP;
}

void Client::SetUserName(QString userName)
{
    _user_name = userName;
}

void Client::SetServerPort(int port)
{
    _server_port = port;
}

void Client::SendMessage(const ConfigValue& msg_object)
{
    json::Writer json_writer;

    std::stringstream ss;
    json_writer.Write(msg_object, ss, false);

    std::string data = ss.str();


    _socket->write(data.c_str(), data.size()+1);
    _socket->flush();
}

void Client::SendHelloMessage(const QString &username)
{
    ConfigValue msg_object;
    std::string uname = username.toLocal8Bit().constData(); // convert from Qstring to std::string

    net_client::CreateHelloMsg(msg_object, uname.c_str());
    SendMessage(msg_object);
}

void Client::SendChatMessage(const QString &message)
 {
    ConfigValue msg_object;
    std::string mess = message.toLocal8Bit().constData();

    net_client::CreateChatMsg(msg_object, mess.c_str());
    SendMessage(msg_object);
 }

void Client::MuteMic(bool toggled)
{

}

void Client::MuteVolume(bool toggled)
{

}
void Client::Disconnected()
{

}
void Client::ReadyRead()
{
	std::string msg = "";

	// Read the message character by character until we reach the terminator
	char c;
	while(_socket->read(&c, 1))
	{
		if(c == '\0')
			ProcessMessage(msg);

		msg += c;
	}
}

void Client::ProcessMessage(const std::string& message)
{
	if(message.empty())
		return;

	// Read the json formatted message
	json::Reader json_reader;

	ConfigValue msg_object;
	if(!json_reader.Read(message.c_str(), message.size(), msg_object))
	{
		debug::Printf("[Error] Failed to read message: %s\n", json_reader.GetErrorMessage().c_str());
		return;
	}

	if(!msg_object["msg_type"].IsString())
	{
		debug::Printf("[Error] Invalid message, missing msg_type\n");
		return;
	}

	std::string msg_type = msg_object["msg_type"].AsString();

	_callback_handler.InvokeCallback(msg_type, msg_object);
}
void Client::OnWelcomeMsg(const ConfigValue& msg_object)
{

}

void Client::OnChatMsg(const ConfigValue& msg_object)
{
}
void Client::OnServerState(const ConfigValue& msg_object)
{
}
void Client::OnUserState(const ConfigValue& msg_object)
{

}
