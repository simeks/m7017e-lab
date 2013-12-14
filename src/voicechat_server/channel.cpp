#include "shared/common.h"

#include "channel.h"
#include "user.h"


Channel::Channel(int channel_id, int parent_id, const std::string& channel_name, Server* server) 
	: _server(server),
	_id(channel_id),
	_parent_id(parent_id),
	_name(channel_name)
{
}
Channel::~Channel()
{
}

void Channel::AddUser(User* user)
{
}
void Channel::RemoveUser(User* user)
{
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

