#include "shared/common.h"

#include "user.h"
#include "server.h"

User::User(Server* server, QTcpSocket* socket) 
	: _server(server),
	_socket(socket)
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
	char data[256];
	data[255] = '\0';

	while(_socket->read(data, 255))
	{
		debug::Printf("Data: %s\n", data);
	}
}