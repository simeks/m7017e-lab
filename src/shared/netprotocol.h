#ifndef __NET_PROTOCOL_H__
#define __NET_PROTOCOL_H__

/* All communication between the server and the clients through TCP are done by sending JSON messages.
*	Below are the definitions of all these messages.
*/

/**
Server messages:

// Welcome message that the server sends to a user after receiving a Hello message.
{
	"msg_type" : "NET_WELCOME",
	"udp_port" : <UDP port for gstreamer>,
}

// Message the server sends when broadcasting a chat message
{
	"msg_type" : "NET_CHAT_MSG",
	"username" : "<username>",
	"chat_msg" : "<chat message>",
}

// Message containing a snapshot of the servers current state, all channels and users
{
	"msg_type" : "NET_SERVER_STATE",
	"channels" : [
		{ "id" : <channel id>, "parent_id" : <parent id>, "name" : "<channel name>" } 
		...
	],
	"users" : [
		{ "id" : <user id>, "name" : "<user name>", "channel" : <channel id> }
		...
	]
}

// Message indicating a change in a users state
{
	"msg_type" : "NET_USER_STATE",
	"user_id" : <user id>,
	"username" : "<username>",
	"online" : <true/false>, 
	"channel" : <channel id>
}

*/

/**
Client messages:

// Hello message that the client sends to the server when connecting.
{
	"msg_type" : "NET_HELLO",
	"username" : "<username>"
}

// Message the client sends to the server when sending a chat message
{
	"msg_type" : "NET_CHAT_MSG",
	"chat_msg" : "<chat message>",
}

*/


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
	void CreateChatMsg(ConfigValue& msg_object, const std::string& message);
	
};

#endif // __NET_PROTOCOL_H__
