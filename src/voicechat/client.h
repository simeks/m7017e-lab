#ifndef CLIENT_H
#define CLIENT_H

#include "qt/mainwindow.h"
#include <QObject>
#include <QTcpSocket>
#include "shared/bus.h"
#include <QDebug>
#include <QTimer>

#include "shared/messagecallbackhandler.h"

class MainWindow;
class ConfigValue;

class ReceiverPipeline;
class SenderPipeline;

class Client : public QObject
{
    Q_OBJECT

public:
    /// Constructor
    /// @param window Pointer to the window handling this Player.
    Client(MainWindow* window);
    ~Client();

    void ConnectClicked();

	/// Sets which the IP that the client should connect to.
    void SetServerIp(QString serverIP);
	
	/// Sets which port on the server to connect to.
    void SetServerPort(int port);

	/// Sets the username for this client.
    void SetUserName(QString userName);

	/// Sends a chat message to the server.
	/// @param message The message to send.
    void SendChatMessage(const QString &message);
    
	/// Sends a hello message to the server identifying ourself.
	/// @param username The requested username.
	void SendHelloMessage(const QString &username);

	/// Changes the audio channel.
	/// @param new_channel The ID of the channel to move to.
	void ChangeChannel(int new_channel);

private slots:
	/// @brief The socket successfully connected
	void Connected();

	/// @brief The socket disconnected.
	void Disconnected();


	/// @brief Ready to read from the socket.
	void ReadyRead();

	void TimerTick();

private:
	typedef void (Client::*MessageCallback)(const ConfigValue&);

	/// Sends the specified message object to the server.
	void SendMessage(const ConfigValue& msg_object);
    
	/// @brief Registers a callback for the specified message type.
	///	The registered callback will be called every time we recieve a packet of the specified type.
	void RegisterCallback(const std::string& msg_type, MessageCallback callback);


	/// @brief Processes a message.
	void ProcessMessage(const std::string& message);

	// Callback methods

	void OnWelcomeMsg(const ConfigValue& msg_object);
	void OnChatMsg(const ConfigValue& msg_object);
	void OnServerState(const ConfigValue& msg_object);
	void OnUserConnected(const ConfigValue& msg_object);
	void OnUserDisconnected(const ConfigValue& msg_object);
	void OnUserChangedChannel(const ConfigValue& msg_object);
	void OnChannelInfo(const ConfigValue& msg_object);


private:
    MainWindow* _window;
    QString _server_ip;
    QString _user_name;
    int _server_tcp_port;
    int _server_udp_port; // The port to send audio to
    int _listen_udp_port; // The port for receiving audio from.
    QTcpSocket* _socket;
	QTimer _tick_timer;
	int _user_id; // This clients user id, -1 if not connected or not authed.
	int _current_channel;
	int _channel_udp_port;

	ReceiverPipeline* _receiver_pipeline;
	SenderPipeline* _sender_pipeline;

	MessageCallbackHandler<Client> _callback_handler;
};


#endif // CLIENT_H

