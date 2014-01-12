#include "shared/common.h"

#include "channel.h"
#include "channelpipeline.h"
#include "server.h"
#include "user.h"


Channel::Channel(int channel_id, int parent_id, const std::string& channel_name, Server* server, int udp_port) 
	: _server(server),
	_id(channel_id),
	_parent_id(parent_id),
	_name(channel_name),
	_udp_port(udp_port)
{
	_pipeline = new ChannelPipeline(udp_port);
}
Channel::~Channel()
{
	delete _pipeline;
}

void Channel::AddUser(User* user)
{
	std::string addr = user->Socket()->peerAddress().toString().toLocal8Bit().constData();
	if(user->Socket()->peerAddress().isLoopback()) // 127.0.0.1 doesn't seem to be working with gstreamer so we need to change to localhost (Issues with ipv4 vs ipv6 maybe?)
		addr = "127.0.0.1";

	_pipeline->AddReceiver(user->Id(), addr, _server->BaseUdpPort() + user->Id());
}
void Channel::RemoveUser(User* user)
{
	_pipeline->RemoveReceiver(user->Id());
}

const std::string& Channel::Name() const
{
	return _name;
}

int Channel::Id() const
{
	return _id;
}
int Channel::ParentId() const
{
	return _parent_id;
}

void Channel::Tick()
{
	_pipeline->Tick();
}
