#include "shared/common.h"
#include "shared/netprotocol.h"
#include "shared/configvalue.h"
#include "shared/json.h"

#include "server.h"
#include "user.h"


Server::Server(int tcp_port, int udp_port_min, int udp_port_max)
	: _tcp_port(tcp_port)
{
	// Fill our list of free udp ports
	for(int p = udp_port_min; p <= udp_port_max; ++p)
		_free_udp_ports.push_back(p);

	_tcp_server = new QTcpServer(this);

	connect(_tcp_server, SIGNAL(newConnection()), this, SLOT(NewConnection()));

	if(!_tcp_server->listen(QHostAddress::Any, tcp_port))
	{
		debug::Printf("[Error] Failed to start server.\n");
	}
}
Server::~Server()
{
	// Release any connected users
	for(std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		(*it)->Socket()->close();
		delete (*it);
	}
	_users.clear();

	_tcp_server->close();
	delete _tcp_server;
}

void Server::BroadcastMessage(const ConfigValue& msg_object)
{
	// Send the data to all users
	for(std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		(*it)->SendMessage(msg_object);
	}
}

void Server::UserDisconnected(User* user)
{
	debug::Printf("User \"%s\" disconnected.\n", user->Socket()->peerAddress().toString().toLocal8Bit().constData());

	// Remove the user from our user list.
	std::vector<User*>::iterator it = std::find(_users.begin(), _users.end(), user);
	if(it != _users.end())
	{
		// Release the users udp port
		_free_udp_ports.push_back((*it)->UdpPort());

		_users.erase(it);
		
		// Free the user object
		delete user;
	}
}

void Server::NewConnection()
{
	QTcpSocket* client_socket = _tcp_server->nextPendingConnection();

	debug::Printf("New connection from %s.\n", client_socket->peerAddress().toString().toLocal8Bit().constData());

	// Assign an udp port to the user
	if(_free_udp_ports.empty())
	{
		// No available ports, therefore we need to reject this connect.
		debug::Printf("No free udp ports available, rejecting connection.\n");
		client_socket->close();
		return;
	}

	int udp_port = _free_udp_ports.back();
	_free_udp_ports.pop_back();

	User* user = new User(this, client_socket, udp_port);
	_users.push_back(user);

}


