#include "shared/common.h"

#include "client.h"
#include "qt/mainwindow.h"
#include "shared/configvalue.h"
#include "shared/json.h"

#include "receiverpipeline.h"
#include "senderpipeline.h"

#include <sstream>

#include <qhostaddress.h>
#include <qthread.h>

Client::Client(MainWindow* window) :
    _window(window),
	_callback_handler(this),
	_server_udp_port(25011),
	_server_tcp_port(25010),
	_receiver_pipeline(NULL),
	_sender_pipeline(NULL)
{
    _socket = new QTcpSocket(this);

	// Register callbacks
    _callback_handler.RegisterCallback("NET_WELCOME", &Client::OnWelcomeMsg);
	_callback_handler.RegisterCallback("NET_CHAT_MSG", &Client::OnChatMsg);
	_callback_handler.RegisterCallback("NET_SERVER_STATE", &Client::OnServerState);
    _callback_handler.RegisterCallback("NET_USER_STATE", &Client::OnUserState);


	// Registers a timer which will update our pipeline at a regular interval.
	_tick_timer.setInterval(25);
	_tick_timer.start();
	connect(&_tick_timer, SIGNAL(timeout()), this, SLOT(TimerTick()));
}
Client::~Client()
{
	if(_socket)
		delete _socket;
	
	if(_sender_pipeline)
		delete _sender_pipeline;

	if(_receiver_pipeline)
		delete _receiver_pipeline;

}

void Client::ConnectClicked()
{
	if(_socket->isOpen()) // Already connected
	{
        _window->OnMessageRecieved("[Warning]", "Already connected to a server!");
		return;
    }
	
    connect(_socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(Disconnected()));
    connect(_socket, SIGNAL(connected()), this, SLOT(Connected()));
	
    _socket->connectToHost(_server_ip, _server_tcp_port);

    if(!_socket->waitForConnected(500))
    {
        _window->OnMessageRecieved("", "Failed to connect to server.");
        _socket->close();
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
    _server_tcp_port = port;
	_server_udp_port = port;
}

void Client::SendMessage(const ConfigValue& msg_object)
{
    json::Writer json_writer;

    std::stringstream ss;
    json_writer.Write(msg_object, ss, false);

    std::string data = ss.str();

    // _window->OnMessageRecieved("", data.c_str());     //test för att se att rätt data skickas

    _socket->write(data.c_str(), data.size()+1);
}

void Client::SendHelloMessage(const QString &username)
{
    ConfigValue msg_object;
    std::string uname = username.toStdString(); // convert from Qstring to std::string

    net_client::CreateHelloMsg(msg_object, uname.c_str());
    SendMessage(msg_object);
}

void Client::SendChatMessage(const QString &username, const QString &message)
 {
    ConfigValue msg_object;
    std::string mess = message.toStdString();
    std::string uname = username.toStdString();

    net_client::CreateChatMsg(msg_object, uname.c_str(), mess.c_str());
    SendMessage(msg_object);
 }

void Client::MuteMic(bool toggled)
{

}

void Client::MuteVolume(bool toggled)
{

}
void Client::Connected()
{
    _window->SetUserName(_user_name);
	_window->Connected();
    SendHelloMessage(_user_name);
}
void Client::Disconnected()
{
	// Remove our pipelines
	if(_sender_pipeline)
	{
		delete _sender_pipeline;
		_sender_pipeline = NULL;
	}
	if(_receiver_pipeline)
	{
		delete _receiver_pipeline;
		_receiver_pipeline = NULL;
	}
	
    _window->OnMessageRecieved("", "Disconnected from server.");
	_window->Disconnected();
	_socket->close();
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
void Client::TimerTick()
{
	if(_receiver_pipeline)
		_receiver_pipeline->Tick();
	if(_sender_pipeline)
		_sender_pipeline->Tick();
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
	if(msg_object["udp_port"].IsNumber())
		_listen_udp_port = msg_object["udp_port"].AsInt();

	std::stringstream ss;
    ss << "You're now connnected successfully. UDP port: " << _listen_udp_port;

    _window->OnMessageRecieved("[Server]", QString::fromStdString(ss.str()));

	// Create our pipelines
	if(!_receiver_pipeline)
		_receiver_pipeline = new ReceiverPipeline(_listen_udp_port);
	
	if(!_sender_pipeline)
	{
		std::string host = _socket->peerAddress().toString().toStdString();
		if(_socket->peerAddress().isLoopback())
			host = "localhost";

		_sender_pipeline = new SenderPipeline(host, _server_udp_port);
	}
}

void Client::OnChatMsg(const ConfigValue& msg_object)
{
    std::string message = "";
    std::string _u_name = "";

    if(msg_object["chat_msg"].IsString())
        message = msg_object["chat_msg"].AsString();

    if(msg_object["username"].IsString())
        _u_name = msg_object["username"].AsString();

    QString mess = QString::fromStdString(message);
    QString uname = QString::fromStdString(_u_name);

    _window->OnMessageRecieved(uname, mess);
}

void Client::OnServerState(const ConfigValue& msg_object)
{

}

void Client::OnUserState(const ConfigValue& msg_object)
{
    std::string _u_name = "";
    std::string _p_channel = "";
    std::string _n_channel = "";

    if(msg_object["username"].IsString())
        _u_name = msg_object["username"].AsString();

    if(msg_object["prev_channel"].IsString())
        _p_channel = msg_object["prev_channel"].AsString();

    if(msg_object["new_channel"].IsString())
        _n_channel = msg_object["new_channel"].AsString();

    QString uname = QString::fromStdString(_u_name);
    QString _prev_channel = QString::fromStdString(_p_channel);
    QString _new_channel = QString::fromStdString(_n_channel);

    _window->OnUserStateChanged(uname, _prev_channel, _new_channel);
}

void Client::ChangeUserState(const QString &username, const QString &prevChannel, const QString &newChannel)
{
    ConfigValue msg_object;
    std::string _prev_channel = prevChannel.toStdString();
    std::string _new_channel = newChannel.toStdString();
    std::string _u_name = username.toStdString();

    net_client::CreateUserStateMsg(msg_object, _u_name.c_str(), _prev_channel.c_str(), _new_channel.c_str());

    SendMessage(msg_object);
}
