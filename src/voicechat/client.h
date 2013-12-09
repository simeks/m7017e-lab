#ifndef CLIENT_H
#define CLIENT_H

#include "../shared/bus.h"
#include "qt/mainwindow.h"
#include <QObject>
#include <QTcpSocket>



class MainWindow;

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






private:
    MainWindow* _window;
    QString _server_ip;
    QString _user_name;
    int _server_port;
    QTcpSocket* socket;
};


#endif // CLIENT_H

