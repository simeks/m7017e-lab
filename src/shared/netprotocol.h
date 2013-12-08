#ifndef __NET_PROTOCOL_H__
#define __NET_PROTOCOL_H__

/// @brief Message definitions for messages from the server.
namespace net_server_msg
{
	enum NetMsg
	{
		NET_WELCOME = 0x0, /// int udp_port
		NET_CHAT_MSG = 0x1, /// string user_name, string message
	};
};

/// @brief Message definitions for messages from the client.
namespace net_client_msg
{
	enum NetMsg
	{
		NET_HELLO = 0x0, /// string name
		NET_CHAT_MSG = 0x1, /// string message
	};

};

#endif // __NET_PROTOCOL_H__
