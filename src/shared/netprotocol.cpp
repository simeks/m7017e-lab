#include "common.h"

#include "netprotocol.h"
#include "configvalue.h"



void net_client::CreateHelloMsg(ConfigValue& msg_object, const std::string& username)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_HELLO");
	msg_object["username"].SetString(username.c_str());
}

void net_client::CreateChatMsg(ConfigValue& msg_object, const std::string& username, const std::string& message)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_CHAT_MSG");
	msg_object["chat_msg"].SetString(message.c_str());
	msg_object["username"].SetString(username.c_str());
}

void net_client::CreateUserStateMsg(ConfigValue& msg_object, const std::string& username, const std::string& prevChannel, const std::string& newChannel)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_USER_STATE");
	msg_object["username"].SetString(username.c_str());
	msg_object["prev_channel"].SetString(prevChannel.c_str());
	msg_object["new_channel"].SetString(newChannel.c_str());

}