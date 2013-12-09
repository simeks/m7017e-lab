#ifndef __VOICECHATSERVER_CLIENT_H__
#define __VOICECHATSERVER_CLIENT_H__

#include <QTcpSocket>

class Server;
class ConfigValue;

/// @brief Class representing a user connected to the server.
class User : public QObject
{
	Q_OBJECT;
public:
	/// Constructor
	/// @param socket An open socket for the connection to this user.
	/// @param udp_port The udp port assigned to this user for media streaming.
	User(Server* server, QTcpSocket* socket, int udp_port);
	~User();

	/// @brief Sends a message object to this User.
	void SendMessage(const ConfigValue& msg_object);

	// @brief Returns the UDP port used by this user.
	int UdpPort() const;

	/// @brief Returns the TCP socket for this user.
	QTcpSocket* Socket();


private slots:
	/// @brief The socket disconnected.
	void Disconnected();

	/// @brief Ready to read from the socket.
	void ReadyRead();

private:
	typedef void (User::*MessageCallback)(const ConfigValue&);

	/// @brief Registers a callback for the specified message type.
	///	The registered callback will be called every time we recieve a packet of the specified type.
	void RegisterCallback(const std::string& msg_type, MessageCallback callback);

	/// @brief Processes a message.
	void ProcessMessage(const std::string& message);

	/// @brief Sends a welcome message to a client.
	void SendWelcomeMsg();


	/// Message callbacks

	/// @brief Reads a hello message from the socket.
	void OnHelloMsg(const ConfigValue& msg_object);

	/// @brief Reads a chat message from the socket.
	void OnChatMsg(const ConfigValue& msg_object);


private:
	Server* _server;
	QTcpSocket* _socket;

	int _udp_port;
	bool _authed;
	std::string _name;

	std::map<std::string, MessageCallback> _message_callbacks;
};


#endif // __VOICECHATSERVER_CLIENT_H__
