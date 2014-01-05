#include "client.h"
#include <pjsua-lib/pjsua.h>
#include "qt/mainwindow.h"

/*
#define SIP_DOMAIN	"example.com"
#define SIP_USER	"alice"
#define SIP_PASSWD	"secret"
*/

Client::Client(MainWindow* window) :
    _window(window)
{
	
    pjsua_acc_id acc_id;
    pj_status_t status;


    // Create pjsua
    status = pjsua_create();
    if (status != PJ_SUCCESS)
    {
        //Error in pjsua_create()
        return;
    }


	// Init pjsua
    pjsua_config cfg;
	pjsua_config_default(&cfg);

	cfg.cb.on_call_state = &on_call_state;
	cfg.cb.on_incoming_call = &on_incoming_call;

	status = pjsua_init(&cfg, NULL, NULL);
	if (status != PJ_SUCCESS)
	{
		// Error in pjsua_init()
		pjsua_destroy();
		return;
	}


	// Start pjsua
	status = pjsua_start();
	if (status != PJ_SUCCESS)
	{
		// Error starting pjsua
		pjsua_destroy();
		return;
	}


	// Add UDP transport.
    pjsua_transport_config transport_cfg;
    pjsua_transport_config_default(&transport_cfg);
    transport_cfg.port = 5060;

    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transport_cfg, NULL);
    if (status != PJ_SUCCESS)
	{
		//Error creating transport
	}
}

// Callback called by the library upon receiving incoming call
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                 pjsip_rx_data *rdata)
{
    pjsua_call_info call_info;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &call_info);

	// öppna incoming_call_dialog

    // Answer incoming calls with 200/OK
    //pjsua_call_answer(call_id, 200, NULL, NULL);
}

// Callback called by the library when call's state has changed
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info call_info;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(call_id, &call_info);
}






    /*

    // Register to SIP server by creating SIP account.
    pjsua_acc_config cfg;

    pjsua_acc_config_default(&cfg);
    cfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
    cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
    cfg.cred_count = 1;
    cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
    cfg.cred_info[0].scheme = pj_str("digest");
    cfg.cred_info[0].username = pj_str(SIP_USER);
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    cfg.cred_info[0].data = pj_str(SIP_PASSWD);

    status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
    if (status != PJ_SUCCESS) error_exit("Error adding account", status);

    // If URL is specified, make call to the URL.
    if (argc > 1) {
    pj_str_t uri = pj_str(argv[1]);
    status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
    if (status != PJ_SUCCESS) error_exit("Error making call", status);
    }

    //Wait until user press "q" to quit.
    for (;;) {
    char option[10];

    puts("Press 'h' to hangup all calls, 'q' to quit");
    if (fgets(option, sizeof(option), stdin) == NULL) {
        puts("EOF while reading stdin, will quit now..");
        break;
    }

    if (option[0] == 'q')
        break;

    if (option[0] == 'h')
        pjsua_call_hangup_all();
    }

    // Destroy pjsua
    pjsua_destroy();

	*/
