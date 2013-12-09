#include "../shared/common.h"

#include "client.h"
#include "qt/mainwindow.h"

Client::Client(MainWindow* window) :
    _window(window)
{

}
Client::~Client()
{

}

void Client::ConnectClicked()
{
    socket = new QTcpSocket(this);
    socket->connectToHost(_server_ip, _server_port);

    _window->Connected();
}

void Client::SetServerIp(QString serverIP)
{
    _server_ip = serverIP;
}

void Client::SetUserName(QString userName)
{
    _user_name = userName;
}

void Client::SetServerPort(int port)
{
    _server_port = port;
}
