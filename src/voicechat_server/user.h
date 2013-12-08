#ifndef __VOICECHATSERVER_CLIENT_H__
#define __VOICECHATSERVER_CLIENT_H__

#include <QTcpSocket>

class Server;

/// @brief Class representing a user connected to the server.
class User : public QObject
{
	Q_OBJECT;
public:
	/// Constructor
	/// @param socket An open socket for the connection to this user.
	User(Server* server, QTcpSocket* socket);
	~User();

	QTcpSocket* Socket();

private slots:
	/// @brief The socket disconnected.
	void Disconnected();

	/// @brief Ready to read from the socket.
	void ReadyRead();

private:
	Server* _server;
	QTcpSocket* _socket;


};


#endif // __VOICECHATSERVER_CLIENT_H__
