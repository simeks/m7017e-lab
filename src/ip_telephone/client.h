#ifndef CLIENT_H
#define CLIENT_H

#include "qt/mainwindow.h"

class MainWindow;

class Client : public QObject
{
    Q_OBJECT

public:
    Client(MainWindow* window);


private:
    MainWindow* _window;
};

#endif // CLIENT_H
