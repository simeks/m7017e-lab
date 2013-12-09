#include "shared/common.h"
#include "shared/netprotocol.h"
#include "shared/configvalue.h"
#include "shared/json.h"

#include "user.h"
#include "server.h"

User::User(Server* server, QTcpSocket* socket) 
	: _server(server),
	_socket(socket),
	_authed(false),	
	_name("Unnamed")

{
	connect(_socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));
	connect(_socket, SIGNAL(disconnected()), this, SLOT(Disconnected()));

	// Register callbacks
	RegisterCallback("NET_HELLO", &User::OnHelloMsg);
	RegisterCallback("NET_CHAT_MSG", &User::OnChatMsg);
}
User::~User()
{
	if(_socket->isOpen())
		_socket->close();
}

void User::SendMessage(const ConfigValue& msg_object)
{
	json::Writer json_writer;

	std::stringstream ss;
	json_writer.Write(msg_object, ss, false); // Convert our message to json

	std::string data = ss.str(); // The data to send.

	_socket->write(data.c_str(), data.size()+1); // +1 for the terminator (\0)
	_socket->flush();
}

QTcpSocket* User::Socket()
{
	return _socket;
}

void User::Disconnected()
{
	_server->UserDisconnected(this);
}
void User::ReadyRead()
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

void User::RegisterCallback(const std::string& msg_type, MessageCallback callback)
{
	_message_callbacks[msg_type] = callback;
}

void User::ProcessMessage(const std::string& message)
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

	// Call any callbacks notifying them about the new message
	std::map<std::string, MessageCallback>::iterator it = _message_callbacks.find(msg_type);
	if (it != _message_callbacks.end())
	{
		// Callback found, invoke it
		(this->*(it->second))(msg_object);
	}

}

void User::SendWelcomeMsg()
{
	ConfigValue msg_object;
	int udp_port = 0; // TODO: Set this later

	net_server::CreateWelcomeMsg(msg_object, udp_port);

	SendMessage(msg_object);
}

void User::OnHelloMsg(const ConfigValue& msg_object)
{
	if(msg_object["username"].IsString()) // If the name is empty, set to default name
		_name = msg_object["username"].AsString();

	// Mark the user as authed, meaning it is allowed to actually do stuff.
	_authed = true;

	// Send a welcome message to the new client
	SendWelcomeMsg();
}

void User::OnChatMsg(const ConfigValue& msg_object)
{
	std::string message = "";

	if(msg_object["message"].IsString())
		message = msg_object["message"].AsString();
	
	if(_authed && message.size()) // Only send if actually authed
	{
		ConfigValue msg_object;
		net_server::CreateChatMsg(msg_object, _name, message);

		_server->BroadcastMessage(msg_object);
	}
}

