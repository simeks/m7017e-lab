#ifndef CLIENT_H
#define CLIENT_H

#include "../shared/bus.h"
#include "qt/mainwindow.h"

class MainWindow;

class Client : public PipelineListener
{
public:
    /// Constructor
    /// @param window Pointer to the window handling this Player.
    Client(MainWindow* window);
    ~Client();

    void ConnectClicked();



private:

    MainWindow* _window;
};


#endif // CLIENT_H

