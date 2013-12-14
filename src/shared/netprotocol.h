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
	"user_id" : <Id of the new user>,
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

// Message indicating a that a new user connected.
{
	"msg_type" : "NET_USER_CONNECTED",
	"user_id" : <user id>,
	"username" : "<username>",
}

// Message indicating a user disconnecting.
{
	"msg_type" : "NET_USER_DISCONNECTED",
	"user_id" : <user id>,
}

// Message indicating a user changing channel.
{
	"msg_type" : "NET_USER_CHANGED_CHANNEL",
	"user_id" : <user id>,
	"channel" : <channel id>,
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

// Message the client sends when it wants to change to another channel.
{
	"msg_type" : "NET_CHANGE_CHANNEL",
	"channel_id" : <channel_id>
}

*/


#endif // __NET_PROTOCOL_H__
