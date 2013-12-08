#include "shared/common.h"
#include "shared/netprotocol.h"

#include "user.h"
#include "server.h"

User::User(Server* server, QTcpSocket* socket) 
	: _server(server),
	_socket(socket),
	_authed(false)
{
	connect(_socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));
	connect(_socket, SIGNAL(disconnected()), this, SLOT(Disconnected()));
}
User::~User()
{
	if(_socket->isOpen())
		_socket->close();
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
	uint8_t msg;

	while(_socket->read((char*)&msg, 1))
	{
		switch(msg)
		{
		case net_client_msg::NET_HELLO:
			ReadHelloMsg();
			break;
		};
	}
}

void User::ReadHelloMsg()
{
	_name = "";
	
	// Read the name character by character until we hit '\0' (String terminator)
	char c;
	while(_socket->read((char*)&c, 1))
	{
		if(c == '\0')
			break;

		_name += c;
	}

	if(_name == "") // If the name is empty, set to default name
		_name = "Unnamed";

	// Mark the user as authed, meaning it is allowed to actually do stuff.
	_authed = true;

	// Send a welcome message to the new client
	SendWelcomeMsg();
}

void User::ReadChatMsg()
{
	std::string message = "";
	
	// Read the name character by character until we hit '\0' (String terminator)
	char c;
	while(_socket->read((char*)&c, 1))
	{
		if(c == '\0')
			break;

		message += c;
	}

	if(_authed) // Only send if actually authed
		_server->SendChatMessage(_name, message);
}

void User::SendWelcomeMsg()
{
	uint8_t msg = net_server_msg::NET_WELCOME;
	_socket->write((char*)&msg, 1);
	
	int udp_port = 0; // TODO: Set this later
	_socket->write((char*)&udp_port, 4);
	_socket->flush();
}
