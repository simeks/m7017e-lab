#ifndef CLIENT_H
#define CLIENT_H

#include "qt/mainwindow.h"
#include <pjsua-lib/pjsua.h>

class MainWindow;

class Client : public QObject
{
    Q_OBJECT

public:
    Client(MainWindow* window);
	static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata);
	static void on_call_state(pjsua_call_id call_id, pjsip_event *e);


private:
    MainWindow* _window;
};

#endif // CLIENT_H
