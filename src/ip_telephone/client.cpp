#include "client.h"
#include <pjsua-lib/pjsua.h>
#include "qt/mainwindow.h"
#include <QDebug>

/// pjsip callbacks
namespace
{
	Client* g_client;

	// Callback called by the library when receiving incoming call
	void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
		pjsip_rx_data *rdata)
	{
		if(!g_client)
			return;

		g_client->OnIncomingCall(acc_id, call_id, rdata);
	}

	// Callback called by the library when call's state has changed
	void on_call_state(pjsua_call_id call_id, pjsip_event *e)
	{
		if(!g_client)
			return;

		g_client->OnCallState(call_id, e);
	}

	// Callback called by the library when call's media state has changed
	void on_call_media_state(pjsua_call_id call_id)
	{
		if(!g_client)
			return;

		g_client->OnCallMediaState(call_id);
	}

};



Client::Client(MainWindow* window) :
	_window(window)
{
	g_client = this;
	_call_id = 0;
	_state = NOT_INITIALIZED;
}
Client::~Client()
{
	g_client = NULL;
	pjsua_call_hangup_all();
	pjsua_destroy();
}


void Client::OnIncomingCall(pjsua_acc_id acc_id, pjsua_call_id call_id,
							pjsip_rx_data *rdata)
{
	// Get info about the previous call
	pjsua_call_info prev_call_info;
	pjsua_call_get_info(_call_id, &prev_call_info);

	// If the previous call is active, hang up with a 486 BUSY response
	if(pjsua_call_is_active(_call_id))

	if((_state != READY) && pjsua_call_is_active(_call_id))
	{
		// 486/Busy
		pjsua_call_hangup(call_id, 486, NULL, NULL);
		qDebug() << "Busy";
		return;
	}

	//  Save the call id
	_call_id = call_id;

	// Get info about the incoming call
	pjsua_call_info call_info;
	pjsua_call_get_info(call_id, &call_info);

	PJ_UNUSED_ARG(acc_id);
	PJ_UNUSED_ARG(rdata);

	// Get the sip-address of the person who is calling
	pj_str_t incoming_uri = call_info.remote_info;
	std::string uri(incoming_uri.ptr, incoming_uri.slen);

	// Open incoming call dialog
	_window->ShowIncomingCallPanel(uri);

    // Start playing the ringtone
    _window->PlayIncomingCallSignal();

	_state = INCOMING_CALL;
}
void Client::OnCallState(pjsua_call_id call_id, pjsip_event *e)
{
	PJ_UNUSED_ARG(e);

	// Get info about the call
	pjsua_call_info call_info;
	pjsua_call_get_info(call_id, &call_info);

	// If Stop, Decline or Hang Up button is clicked
	if (call_info.state == PJSIP_INV_STATE_DISCONNECTED)
	{
		// Hide all panels and show sip-address textfield and Call button
		_window->HideIncomingCallPanel();
		_window->HideActiveCallPanel();
		_window->HideCallingPanel();
		_window->ShowMainWindow();

        _window->StopPlayingRingtones();

		_state = READY;
	}

	// If Answer button is clicked
	if(call_info.last_status == 200 && call_info.state == PJSIP_INV_STATE_CONFIRMED)
	{
		// Show the Active-call panel and hide the Calling panel and Incoming-call panel
		_window->ShowActiveCallPanel();
		_window->HideCallingPanel();
		_window->HideIncomingCallPanel();

		_window->ShowActiveCallPanel();
		_window->HideCallingPanel();
		_window->HideIncomingCallPanel();

        _window->StopPlayingRingtones();

		_state = IN_CALL;
	}
}

void Client::AnswerIncomingCall()
{
	// Answer incoming calls with 200/OK
	pjsua_call_answer(_call_id, 200, NULL, NULL);
}

void Client::MakeCall(std::string uri)
{
	pj_str_t _uri = pj_str((char*) uri.c_str());

	// Make the call
	status = pjsua_call_make_call(acc_id, &_uri, 0, NULL, NULL, &_call_id);
	if (status != PJ_SUCCESS)
	{
		qDebug() << "Error making the call";
	}
	else
	{
		qDebug() << "Making the call SUCCESS";
	}
}

void Client::InterruptCall()
{
	// Interrupt the call when the Caller clicks on the Stop button
	pjsua_call_hangup(_call_id, NULL, NULL, NULL);
}

void Client::HangUpActiveCall()
{
	// Hang up the active call when the Hang Up button is clicked
	pjsua_call_hangup(_call_id, NULL, NULL, NULL);
}

void Client::DeclineCall()
{
	// 603/Decline when the Decline button is clicked
	pjsua_call_hangup(_call_id, 0, NULL, NULL);
}

// Get the sip-account details from the user
void Client::SetUser(std::string username, std::string password, std::string domain)
{
	_username = username;
	_password = password;
	_domain = domain;

	InitializePJ();
}

void Client::InitializePJ()
{
	// Create pjsua
	status = pjsua_create();
	if (status != PJ_SUCCESS)
	{
		qDebug() << "Error in pjsua_create()";
		return;
	}
	else
	{
		qDebug() << "pjsua_create() SUCCESS";
	}


	// Init pjsua
	pjsua_config cfg;
	pjsua_config_default(&cfg);

	cfg.cb.on_call_state = &on_call_state;
	cfg.cb.on_incoming_call = &on_incoming_call;
	cfg.cb.on_call_media_state = &on_call_media_state;

	status = pjsua_init(&cfg, NULL, NULL);
	if (status != PJ_SUCCESS)
	{
		qDebug() << "Error in pjsua_init()";
		pjsua_destroy();
		return;
	}
	else
	{
		qDebug() << "pjsua_init() SUCCESS";
	}


	// Start pjsua
	status = pjsua_start();
	if (status != PJ_SUCCESS)
	{
		qDebug() << "Error starting pjsua";
		pjsua_destroy();
		return;
	}
	else
	{
		qDebug() << "Starting pjsua SUCCESS";
	}

	AddTransportPJ();

	CreateSipAccount();

	_state = READY;
}

void Client::AddTransportPJ()
{
	// Add UDP transport.
	pjsua_transport_config transport_cfg;
	pjsua_transport_config_default(&transport_cfg);
	transport_cfg.port = 5060;

	status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transport_cfg, NULL);
	if (status != PJ_SUCCESS)
	{
		qDebug() << "Error creating transport";
		return;
	}
	else
	{
		qDebug() << "Creating transport SUCCESS";
	}
}

void Client::CreateSipAccount()
{
	// Create a SIP account and register to server
	pjsua_acc_config acc_cfg;

	pjsua_acc_config_default(&acc_cfg);

	std::string id("sip:" + _username + "@" + _domain);
	acc_cfg.id = pj_str((char *) id.c_str());

	std::string uri("sip:" + _domain);
	acc_cfg.reg_uri = pj_str((char *) uri.c_str());

	acc_cfg.cred_count = 1;
	acc_cfg.cred_info[0].realm = pj_str((char *) _domain.c_str());


	acc_cfg.cred_info[0].scheme = pj_str("digest");
	acc_cfg.cred_info[0].username = pj_str((char*) _username.c_str());
	acc_cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	acc_cfg.cred_info[0].data = pj_str((char *) _password.c_str());

	status = pjsua_acc_add(&acc_cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS)
	{
		qDebug() << "Error adding account";
		return;
	}
	else
	{
		qDebug() << "Adding account SUCCESS";
	}
}

void Client::OnCallMediaState(pjsua_call_id call_id)
{
	// Get info about the call
	pjsua_call_info call_info;
	pjsua_call_get_info(call_id, &call_info);

	if (call_info.media_status == PJSUA_CALL_MEDIA_ACTIVE) 
	{
		// Connect the call to the sound device when media is active.
		pjsua_conf_connect(call_info.conf_slot, 0);
		pjsua_conf_connect(0, call_info.conf_slot);
	}
}
