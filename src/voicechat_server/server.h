#ifndef __SERVER_H__
#define __SERVER_H__

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>

class User;
class Channel;
class ConfigValue;
class ServerPipeline;
class Server : public QObject
{
	Q_OBJECT;

public:
	Server(int tcp_port, int udp_port);
	~Server();
	
	/// @brief Broadcasts a  message to all users.
	/// @param msg_object The object to send.
	void BroadcastMessage(const ConfigValue& msg_object);

	/// @brief Notifies the server about a disconnected user.
	void UserDisconnected(User* user);

	/// @brief Creates a new channel with the specified name.
	/// @param name Name of the new channel.
	/// @param parent_channel Specifies the parent channel, 0 means the root channel, -1 indicates that there is no parent channel.
	void CreateChannel(const std::string& name, int parent_channel = 0);

	/// @brief Moves a user to a specified channel.
	/// @param user_id User to move.
	/// @param channel_id Channel to move to.
	void MoveUser(int user_id, int channel_id);

	/// Welcomes a new user, sending necessary info. Also broadcasts info about the new user to all other clients.
	void WelcomeUser(User* user);

	/// @brief Returns the user with the specified id.
	/// @return The user, NULL if no user with the specified id was found.
	User* GetUser(int id);

	/// @brief Returns the channel with the specified id.
	/// @return The channel, NULL if no channel with the specified id was found.
	Channel* GetChannel(int id);

private slots:
	void NewConnection();
	void TimerTick();

private:
	int _tcp_port;
	int _udp_port;

	QTimer _tick_timer;

	ServerPipeline* _pipeline;

	QTcpServer* _tcp_server;

	std::vector<User*> _users;
	std::vector<Channel*> _channels;

	int _next_uid; // Used for generating new unique identifiers for server objects such as users and channels.
};


#endif // __SERVER_H__
