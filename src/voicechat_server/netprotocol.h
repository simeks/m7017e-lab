#ifndef __SERVER_NETPROTOCOL_H__
#define __SERVER_NETPROTOCOL_H__


class ConfigValue;
class User;
class Channel;

/// @brief Utilities for formatting network messages
namespace net_server
{
	/// @brief Formats a welcome message.
	/// @param msg_object This is the object that will contain the message.
	/// @param user_id Unique ID for the new user.
	/// @param udp_port The UDP port used for streaming audio.
	void CreateWelcomeMsg(ConfigValue& msg_object, int user_id, int udp_port);

	/// @brief Formats a chat message.
	/// @param msg_object This is the object that will contain the message.
	/// @param username Name of the sender.
	/// @param message The actual chat message.
	void CreateChatMsg(ConfigValue& msg_object, const std::string& username, const std::string& message);

	/// @brief Formats a server state message
	/// @param msg_object This is the object that will contain the message.
	/// @param channels A list of all channels.
	/// @param users A list of all users.
	void CreateServerStateMsg(ConfigValue& msg_object, const std::vector<Channel*>& channels, const std::vector<User*>& users);

	/// @brief Formats a user connected message
	/// @param msg_object This is the object that will contain the message.
	/// @param user The connected user.
	void CreateUserConnectedMsg(ConfigValue& msg_object, User* user);

	/// @brief Formats a user disconnected message
	/// @param msg_object This is the object that will contain the message.
	/// @param user The disconnected user.
	void CreateUserDisconnectedMsg(ConfigValue& msg_object, User* user);

	/// @brief Formats a user changed channel message
	/// @param msg_object This is the object that will contain the message.
	/// @param user The user that changed channel.
	void CreateUserChangedChannelMsg(ConfigValue& msg_object, User* user);

	/// @brief Creates a message for a specific user when it enters a channel.
	/// @param channel_id Id of the channel.
	/// @param udp_port The udp port the client should transmit audio to.
	void CreateChannelInfoMsg(ConfigValue& msg_object, int channel_id, int udp_port);

};

#endif // __SERVER_NETPROTOCOL_H__
