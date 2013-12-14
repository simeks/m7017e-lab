#include "shared/common.h"
#include "shared/configvalue.h"

#include "netprotocol.h"



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

void net_client::CreateChangeChannelMsg(ConfigValue& msg_object, int channel_id)
{
	msg_object.SetEmptyObject();
	msg_object["msg_type"].SetString("NET_CHANGE_CHANNEL");
	msg_object["channel_id"].SetInt(channel_id);

}
