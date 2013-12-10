#include "shared/common.h"
#include "shared/configvalue.h"
#include "shared/json.h"

#include "netprotocol.h"
#include "server.h"
#include "user.h"
#include "channel.h"


Server::Server(int tcp_port, int udp_port_min, int udp_port_max)
	: _tcp_port(tcp_port),
	_next_uid(0)
{
	// Create our root channel, which will get ID 0
	CreateChannel("root", (uint32_t)-1); // -1 indicates that there is no parent channel

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

	// Release all existing channels
	for(std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		delete (*it);
	}
	_channels.clear();

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
	
	// Broadcast that a user disconnected.
	ConfigValue msg;
	net_server::CreateUserStateMsg(msg, user, false);

	BroadcastMessage(msg);

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

/// @brief Creates a new channel with the specified name.
void Server::CreateChannel(const std::string& name, int parent_channel)
{
	Channel* channel = new Channel(_next_uid++, parent_channel, name, this);
	_channels.push_back(channel);

	// Broadcast the changed state to all users
	ConfigValue msg;
	net_server::CreateServerStateMsg(msg, _channels, _users);

	BroadcastMessage(msg);
}

void Server::MoveUser(int user_id, int channel_id)
{
	User* user = GetUser(user_id);
	if(!user)
	{
		debug::Printf("[Error] Server::MoveUser: No user with id %d.", user_id);
		return; // Not a valid user, nothing to do.
	}

	Channel* channel = GetChannel(channel_id);
	if(!channel)
	{
		debug::Printf("[Error] Server::MoveUser: No channel with id %d.", channel_id);
		return; // Not a valid channel, nothing to do.
	}

	// First remove the user from any old channel
	if(channel_id >= 0)
	{
		Channel* old_channel = GetChannel(channel_id);
		assert(old_channel);

		old_channel->RemoveUser(user);
	}

	// Then add it to the new one
	channel->AddUser(user);
	// Notify the user about the new channel
	user->SetChannel(channel_id);
}

User* Server::GetUser(int id)
{
	for(std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if((*it)->Id() == id)
			return (*it);
	}
	return NULL;
}
Channel* Server::GetChannel(int id)
{
	for(std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if((*it)->Id() == id)
			return (*it);
	}
	return NULL;
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

	User* user = new User(_next_uid++, this, client_socket, udp_port);
	_users.push_back(user);

	// Broadcast that we have a new user
	ConfigValue msg;
	net_server::CreateUserStateMsg(msg, user, true);

	BroadcastMessage(msg);

	// Move the user to the root channel
	MoveUser(user->Id(), 0);
}


