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
    _window->Connected();
}
