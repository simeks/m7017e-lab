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

	if(_receiver_pipeline)
		delete _receiver_pipeline;

	if(_sender_pipeline)
		delete _sender_pipeline;
}

void Client::ConnectClicked()
{
	if(_socket->isOpen()) // Already connected
	{
		_window->OnMessageRecieved("[Warning] Already connected to a server!");
		return;
	}
	
	connect(_socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));
	connect(_socket, SIGNAL(disconnected()), this, SLOT(Disconnected()));
	connect(_socket, SIGNAL(connected()), this, SLOT(Connected()));
	
    _socket->connectToHost(_server_ip, _server_tcp_port);

    if(!_socket->waitForConnected(500))
    {
		_window->OnMessageRecieved("Failed to connect to server.");
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
	
    _socket->write(data.c_str(), data.size()+1);
}

void Client::SendHelloMessage(const QString &username)
{
    ConfigValue msg_object;
    std::string uname = username.toStdString(); // convert from Qstring to std::string

    net_client::CreateHelloMsg(msg_object, uname.c_str());
    SendMessage(msg_object);
}

void Client::SendChatMessage(const QString &message)
 {
    ConfigValue msg_object;
    std::string mess = message.toStdString();

    net_client::CreateChatMsg(msg_object, mess.c_str());
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
	
	_window->OnMessageRecieved("Disconnected from server.");
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
	ss << "[Server] You're now connnected successfully. UDP port: " << _listen_udp_port;

	_window->OnMessageRecieved(QString::fromStdString(ss.str()));

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

    if(msg_object["chat_msg"].IsString())
        message = msg_object["chat_msg"].AsString();

    QString mess = QString::fromStdString(message);

    _window->OnMessageRecieved(mess);
}

void Client::OnServerState(const ConfigValue& msg_object)
{

}

void Client::OnUserState(const ConfigValue& msg_object)
{

}
