#include "../shared/common.h"

#include "client.h"
#include "qt/mainwindow.h"
#include "../shared/configvalue.h"
#include "../shared/json.h"

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

    if(socket->waitForConnected(500))
    {
        _window->Connected();
        sendHelloMessage(_user_name);
    }
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

void Client::sendHelloMessage(const QString &username)
{
    ConfigValue msg_object;
    std::string uname = username.toLocal8Bit().constData(); // convert from Qstring to std::string

    net_client::CreateHelloMsg(msg_object, uname.c_str());

    json::Writer json_writer;

    std::stringstream ss;
    json_writer.Write(msg_object, ss, false);

    std::string data = ss.str();

    socket->write(data.c_str(), data.size()+1);
    socket->flush();

}

void Client::sendChatMessage(const QString &message)
 {
    ConfigValue msg_object;
    std::string mess = message.toLocal8Bit().constData();

    net_client::CreateChatMsg(msg_object, mess.c_str());

    json::Writer json_writer;

    std::stringstream ss;
    json_writer.Write(msg_object, ss, false);

    std::string data = ss.str();

    socket->write(data.c_str(), data.size()+1);
    socket->flush();

 }
