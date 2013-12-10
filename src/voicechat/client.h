#ifndef CLIENT_H
#define CLIENT_H

#include "qt/mainwindow.h"
#include "../shared/netprotocol.h"
#include <QObject>
#include <QTcpSocket>
#include "../shared/bus.h"
#include <QDebug>


#include "../shared/messagecallbackhandler.h"

class MainWindow;
class ConfigValue;


class Client : public QObject
{
    Q_OBJECT

public:
    /// Constructor
    /// @param window Pointer to the window handling this Player.
    Client(MainWindow* window);
    ~Client();

    void ConnectClicked();
    void SetServerIp(QString serverIP);
    void SetUserName(QString userName);
    void SetServerPort(int port);
    void SendChatMessage(const QString &message);
    void SendHelloMessage(const QString &message);
    void SendMessage(const ConfigValue& msg_object);
    void MuteVolume(bool toggled);
    void MuteMic(bool toggled);

private slots:
	/// @brief The socket disconnected.
	void Disconnected();

	/// @brief Ready to read from the socket.
	void ReadyRead();

private:
	typedef void (Client::*MessageCallback)(const ConfigValue&);
	
	/// @brief Registers a callback for the specified message type.
	///	The registered callback will be called every time we recieve a packet of the specified type.
	void RegisterCallback(const std::string& msg_type, MessageCallback callback);


	/// @brief Processes a message.
	void ProcessMessage(const std::string& message);

	// Callback methods

	void OnWelcomeMsg(const ConfigValue& msg_object);
	void OnChatMsg(const ConfigValue& msg_object);
	void OnServerState(const ConfigValue& msg_object);
	void OnUserState(const ConfigValue& msg_object);


private:
    MainWindow* _window;
    QString _server_ip;
    QString _user_name;
    int _server_port;
    QTcpSocket* _socket;
	
	MessageCallbackHandler<Client> _callback_handler;
};


#endif // CLIENT_H

