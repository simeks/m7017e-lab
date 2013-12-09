#include "shared/common.h"

#include "channel.h"


Channel::Channel(int channel_id, int parent_id, const std::string& channel_name) 
	: _id(channel_id),
	_parent_id(parent_id),
	_name(channel_name)
{
}
Channel::~Channel()
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

