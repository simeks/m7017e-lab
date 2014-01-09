#ifndef CLIENT_H
#define CLIENT_H

#include "qt/mainwindow.h"
#include <pjsua-lib/pjsua.h>

class MainWindow;

/// @brief Class responsible for handling incoming and outgoing calls.
class Client : public QObject
{
    Q_OBJECT

public:
    Client(MainWindow* window);
	~Client();
	
	/// @brief Callback called by the library when receiving incoming call
	void OnIncomingCall(pjsua_acc_id acc_id, pjsua_call_id call_id,
					 pjsip_rx_data *rdata);
	/// @biref Callback called by the library when call's state has changed
	void OnCallState(pjsua_call_id call_id, pjsip_event *e);

	/// @biref Callback called by the library when call's media state has changed
	void OnCallMediaState(pjsua_call_id call_id);

	/// @biref Called when the user clicks on the Answer button
	void AnswerIncomingCall();

	/// @biref Called when the user clicks on the Call button
	void MakeCall(std::string uri);

	/// @biref Called when the user clicks on the Stop button
	void InterruptCall();

	/// @biref Called when the user clicks on the Hang Up button
	void HangUpActiveCall();

	/// @biref Called when the user clicks on the Decline button
	void DeclineCall();

	/// @biref Get the user's sip-account details
	void SetUser(std::string username, std::string password, std::string domain);

	/// @biref Initialize pjsip
	void InitializePJ();

	/// @biref Add UDP transport
	void AddTransportPJ();

	/// @biref Create a sip-account and register to the server
	void CreateSipAccount();

private:
	enum State
	{
		NOT_INITIALIZED,
		READY, // Ready for incoming calls
		CALLING, // Currently calling
		INCOMING_CALL, // Currently having an incoming call waiting
		IN_CALL, // Currently in a call
	};

	State _state;

    MainWindow* _window;
	pjsua_call_id _call_id;
	pjsua_acc_id acc_id;
    pj_status_t status;

	std::string _username;
	std::string _password;
	std::string _domain;
};

#endif // CLIENT_H
