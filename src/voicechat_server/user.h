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
	User(Server* server, QTcpSocket* socket);
	~User();

	/// @brief Sends a message object to this User.
	void SendMessage(const ConfigValue& msg_object);

	QTcpSocket* Socket();

private slots:
	/// @brief The socket disconnected.
	void Disconnected();

	/// @brief Ready to read from the socket.
	void ReadyRead();

private:
	/// @brief Processes a message.
	void ProcessMessage(const std::string& message);


	/// @brief Reads a hello message from the socket.
	void ReadHelloMsg(const ConfigValue& msg_object);

	/// @brief Reads a chat message from the socket.
	void ReadChatMsg(const ConfigValue& msg_object);


	/// @brief Sends a welcome message to a client.
	void SendWelcomeMsg();

private:
	Server* _server;
	QTcpSocket* _socket;

	bool _authed;
	std::string _name;

};


#endif // __VOICECHATSERVER_CLIENT_H__
