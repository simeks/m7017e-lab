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
	~Client();
	
	// Callback called by the library upon receiving incoming call
	void OnIncomingCall(pjsua_acc_id acc_id, pjsua_call_id call_id,
					 pjsip_rx_data *rdata);
	// Callback called by the library when call's state has changed
	void OnCallState(pjsua_call_id call_id, pjsip_event *e);

	void OnCallMediaState(pjsua_call_id call_id);

	void AnswerIncomingCall();
	void MakeCall(std::string uri);
	void InterruptCall();
	void HangUpActiveCall();
	void DeclineCall();
	void SetUser(std::string username, std::string password, std::string domain);
	void InitializePJ();
	void AddTransportPJ();
	void CreateSipAccount();

private:

    MainWindow* _window;
	pjsua_call_id _call_id;
	pjsua_acc_id acc_id;
    pj_status_t status;

	std::string _username;
	std::string _password;
	std::string _domain;
};

#endif // CLIENT_H
