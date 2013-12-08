#include "shared/common.h"
#include "shared/netprotocol.h"

#include "server.h"
#include "user.h"


Server::Server(int tcp_port, int udp_port_min, int udp_port_max)
	: _tcp_port(tcp_port),
	_udp_port_min(udp_port_min),
	_udp_port_max(udp_port_max)
{
	_tcp_server = new QTcpServer(this);

	connect(_tcp_server, SIGNAL(newConnection()), this, SLOT(NewConnection()));

	if(!_tcp_server->listen(QHostAddress::Any, tcp_port))
	{
		debug::Printf("[Error] Failed to start server.\n");
	}
}
Server::~Server()
{
	_tcp_server->close();
	delete _tcp_server;
}

void Server::SendChatMessage(const std::string& sender, const std::string& message)
{
	char msg_header = net_server_msg::NET_CHAT_MSG;

	std::vector<User*>::iterator it, end;
	it = _users.begin(); end = _users.end();
	for( ; it != end; ++it)
	{
		(*it)->Socket()->write(&msg_header, 1);
		(*it)->Socket()->write(sender.c_str(), sender.size()+1); // +1 to include the '\0'.
		(*it)->Socket()->write(message.c_str(), message.size()+1); // +1 to include the '\0'.
		(*it)->Socket()->flush();
	}
}

void Server::UserDisconnected(User* user)
{
	debug::Printf("User \"%s\" disconnected.\n", user->Socket()->peerAddress().toString().toLocal8Bit().constData());

	// Remove the user from our user list.
	std::vector<User*>::iterator it = std::find(_users.begin(), _users.end(), user);
	if(it != _users.end())
	{
		_users.erase(it);
		
		// Free the user object
		delete user;
	}
}

void Server::NewConnection()
{
	QTcpSocket* client_socket = _tcp_server->nextPendingConnection();

	debug::Printf("New connection from %s.\n", client_socket->peerAddress().toString().toLocal8Bit().constData());

	User* user = new User(this, client_socket);
	_users.push_back(user);

}


