#ifndef __SERVER_H__
#define __SERVER_H__

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class User;
class ConfigValue;
class Server : public QObject
{
	Q_OBJECT;

public:
	Server(int tcp_port, int udp_port_min, int udp_port_max);
	~Server();
	
	/// @brief Broadcasts a  message to all users.
	/// @param msg_object The object to send.
	void BroadcastMessage(const ConfigValue& msg_object);

	/// @brief Notifies the server about a disconnected user.
	void UserDisconnected(User* user);


private slots:
	void NewConnection();

private:
	int _tcp_port;
	std::vector<int> _free_udp_ports; // List of available udp ports we can assign to new clients

	QTcpServer* _tcp_server;

	std::vector<User*> _users;
};


#endif // __SERVER_H__
