#include "shared/common.h"
#include "shared/configvalue.h"

#include "netprotocol.h"
#include "user.h"
#include "channel.h"


void net_server::CreateWelcomeMsg(ConfigValue& msg_object, int user_id, int udp_port)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_WELCOME");
	msg_object["user_id"].SetInt(udp_port);
	msg_object["udp_port"].SetInt(udp_port);
}

void net_server::CreateChatMsg(ConfigValue& msg_object, const std::string& username, const std::string& message)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_CHAT_MSG");
	msg_object["username"].SetString(username.c_str());
	msg_object["chat_msg"].SetString(message.c_str());
}
void net_server::CreateServerStateMsg(ConfigValue& msg_object, const std::vector<Channel*>& channels, const std::vector<User*>& users)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_SERVER_STATE");

	ConfigValue& channel_object = msg_object["channels"];
	channel_object.SetEmptyArray();

	for(std::vector<Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		ConfigValue& channel_entry = channel_object.Append();
		channel_entry.SetEmptyObject();
		channel_entry["id"].SetInt((*it)->Id());
		channel_entry["parent_id"].SetInt((*it)->ParentId());
		channel_entry["name"].SetString((*it)->Name().c_str());
	}

	ConfigValue& user_object = msg_object["users"];
	user_object.SetEmptyArray();

	for(std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it)
	{
		ConfigValue& user_entry = user_object.Append();
		user_entry.SetEmptyObject();
		user_entry["id"].SetInt((*it)->Id());
		user_entry["name"].SetString((*it)->Name().c_str());
		user_entry["channel"].SetInt((*it)->Channel());
	}

}

void net_server::CreateUserConnectedMsg(ConfigValue& msg_object, User* user)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_USER_CONNECTED");
	msg_object["user_id"].SetInt(user->Id());
	msg_object["username"].SetString(user->Name().c_str());
}
void net_server::CreateUserDisconnectedMsg(ConfigValue& msg_object, User* user)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_USER_DISCONNECTED");
	msg_object["user_id"].SetInt(user->Id());

}
void net_server::CreateUserChangedChannelMsg(ConfigValue& msg_object, User* user)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_USER_CHANGED_CHANNEL");
	msg_object["user_id"].SetInt(user->Id());
	msg_object["channel"].SetInt(user->Channel());

}
