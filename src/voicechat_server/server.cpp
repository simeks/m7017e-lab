#include "shared/common.h"
#include "shared/configvalue.h"
#include "shared/json.h"

#include "netprotocol.h"
#include "server.h"
#include "user.h"
#include "channel.h"
#include "server_pipeline.h"


Server::Server(int tcp_port, int udp_port)
	: _tcp_port(tcp_port),
	_udp_port(udp_port),
	_next_uid(0),
	_tick_timer(this)
{
	// Create our root channel, which will get ID 0
	CreateChannel("root", -1); // -1 indicates that there is no parent channel
	CreateChannel("Room 1", 0);
	CreateChannel("Room 2", 0);
	CreateChannel("Room 3", 0);


	_tcp_server = new QTcpServer(this);

	connect(_tcp_server, SIGNAL(newConnection()), this, SLOT(NewConnection()));

	if(!_tcp_server->listen(QHostAddress::Any, tcp_port))
	{
		debug::Printf("[Error] Failed to start server.\n");
	}

	_pipeline = new ServerPipeline(_udp_port);

	// Registers a timer which will update our pipeline at a regular interval.
	_tick_timer.setInterval(25);
	_tick_timer.start();
	connect(&_tick_timer, SIGNAL(timeout()), this, SLOT(TimerTick()));
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
	delete _pipeline;
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
	net_server::CreateUserDisconnectedMsg(msg, user);

	BroadcastMessage(msg);

	// Remove the user from our user list.
	std::vector<User*>::iterator it = std::find(_users.begin(), _users.end(), user);
	if(it != _users.end())
	{
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
	debug::Printf("Moving user %s (ID: %d) to channel %s (ID: %d)\n", user->Name().c_str(), user_id, channel->Name().c_str(), channel_id);

	// First remove the user from any old channel
	if(user->Channel() >= 0)
	{
		Channel* old_channel = GetChannel(user->Channel());

		old_channel->RemoveUser(user);
	}


	// Then add it to the new one
	channel->AddUser(user);
	// Notify the user about the new channel
	user->SetChannel(channel_id);

	// Broadcast about the move
	ConfigValue msg;
	net_server::CreateUserChangedChannelMsg(msg, user);

	BroadcastMessage(msg);
}

void Server::WelcomeUser(User* user)
{
	// Send welcome message
	ConfigValue msg_object;
	net_server::CreateWelcomeMsg(msg_object, user->Id(), _udp_port);
	user->SendMessage(msg_object);

	// We also want to send the new user a snapshot of the current server state
	net_server::CreateServerStateMsg(msg_object, _channels, _users);
	user->SendMessage(msg_object);

	// Broadcast that we have a new user
	net_server::CreateUserConnectedMsg(msg_object, user);
	BroadcastMessage(msg_object);

	// Move the user to the root channel
	MoveUser(user->Id(), 0);

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

	int udp_port = _udp_port+_next_uid; // We generate a unique udp port for each user, this is mostly for testing purposes on localhost
	debug::Printf("New connection from %s. (Port: %d)\n", client_socket->peerAddress().toString().toLocal8Bit().constData(), udp_port);

	User* user = new User(_next_uid++, this, client_socket, udp_port);
	_users.push_back(user);

	std::string addr = user->Socket()->peerAddress().toString().toLocal8Bit().constData();
	if(user->Socket()->peerAddress().isLoopback()) // 127.0.0.1 doesn't seem to be working with gstreamer so we need to change to localhost (Issues with ipv4 vs ipv6 maybe?)
		addr = "localhost";

	_pipeline->AddReceiver(user->Id(), addr, _udp_port+user->Id());
}
void Server::TimerTick()
{
	if(_pipeline)
		_pipeline->Tick();
}


