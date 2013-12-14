#ifndef __CLIENT_NET_PROTOCOL_H__
#define __CLIENT_NET_PROTOCOL_H__



class ConfigValue;


/// @brief Message definitions for messages from the client.
namespace net_client
{
	/// @brief Formats a welcome message.
	/// @param msg_object This is the object that will contain the message.
	/// @param username Requested user name.
	void CreateHelloMsg(ConfigValue& msg_object, const std::string& username);

	/// @brief Formats a chat message.
	/// @param msg_object This is the object that will contain the message.
	/// @param message The actual chat message.
	void CreateChatMsg(ConfigValue& msg_object, const std::string& username, const std::string& message);
	
	/// @brief Formats a change channel message.
	/// @param msg_object This is the object that will contain the message.
	/// @param new_channel Which channel to change to.
	void CreateChangeChannelMsg(ConfigValue& msg_object, int new_channel);
	
};

#endif // __CLIENT_NET_PROTOCOL_H__
