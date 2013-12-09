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
	/// @param udp_port The UDP port used for streaming audio.
	void CreateWelcomeMsg(ConfigValue& msg_object, int udp_port);

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

	/// @brief Formats a user state message
	/// @param msg_object This is the object that will contain the message.
	/// @param user The updated user.
	/// @param online Indicates whether the user is online or not, 
	///			setting this to false indicates to the clients that this user have gone offline.
	void CreateUserStateMsg(ConfigValue& msg_object, User* user, bool online);

};

#endif // __SERVER_NETPROTOCOL_H__
