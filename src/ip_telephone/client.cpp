#include "client.h"
#include <pjsua-lib/pjsua.h>
#include "qt/mainwindow.h"
#include <QDebug>

//#define SIP_DOMAIN	"iptel.org"
//#define SIP_USER	"joohanforsling"
//#define SIP_PASSWD	"hejhej"

/// pjsip callbacks
namespace
{
	Client* g_client;

	// Callback called by the library upon receiving incoming call
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

};



Client::Client(MainWindow* window) :
    _window(window)
{
	g_client = this;

}
Client::~Client()
{
	g_client = NULL;
}


void Client::OnIncomingCall(pjsua_acc_id acc_id, pjsua_call_id call_id,
					pjsip_rx_data *rdata)
{
	pjsua_call_info call_info;

	_call_id = call_id;

	PJ_UNUSED_ARG(acc_id);
	PJ_UNUSED_ARG(rdata);

	pjsua_call_get_info(call_id, &call_info);
	
	// Open incoming call dialog
	_window->ShowIncomingCallDialog();
}
void Client::OnCallState(pjsua_call_id call_id, pjsip_event *e)
{
	pjsua_call_info call_info;
	
	PJ_UNUSED_ARG(e);

	pjsua_call_get_info(call_id, &call_info);
}

void Client::AnswerIncomingCall()
{
	// Answer incoming calls with 200/OK
	pjsua_call_answer(_call_id, 200, NULL, NULL);
}

void Client::MakeCall(std::string uri)
{
	pj_str_t _uri = pj_str((char*) uri.c_str());

	status = pjsua_call_make_call(acc_id, &_uri, 0, NULL, NULL, NULL);
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
	pjsua_call_hangup(_call_id, NULL, NULL, NULL);
}

void Client::DeclineCall()
{
	pjsua_call_hangup(_call_id, 0, NULL, NULL);
}

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
		// Error starting pjsua
		pjsua_destroy();
		return;
	}
	else
	{
		qDebug() << "Starting pjsua SUCCESS";
	}

	AddTransportPJ();

	CreateSipAccount();

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
	/* Register to SIP server by creating SIP account. */
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