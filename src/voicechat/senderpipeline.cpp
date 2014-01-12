#include "shared/common.h"
#include "senderpipeline.h"
#include "receiverpipeline.h"

#include <sstream>

#include <WinSock2.h>
namespace socket_util
{

	void InitializeWinSock()
	{
		static bool s_initializd = false;
		if(!s_initializd)
		{
			WSAData data;
			WSAStartup(MAKEWORD(2,2), &data);

			s_initializd = true;
		}
	}

	SOCKET CreateSocket(int port)
	{
		InitializeWinSock();

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
		addr.sin_port = htons(port);

		SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		bind(s, (sockaddr*)&addr, sizeof(addr));

		return s;
	}

};

// Called when there's a new pad that needs to be connected from our ssrc demuxer
void SenderPipeline::new_ssrc_pad_cb(GstRtpSsrcDemux*, guint ssrc, GstPad *pad, gpointer user_data)
{
	SenderPipeline* pipeline = (SenderPipeline*)user_data;
	gst_element_set_state(pipeline->_pipeline, GST_STATE_PAUSED);

	// Create a udp sink
    GstElement* udpsink = gst_element_factory_make("udpsink", NULL);
	SOCKET send_socket = socket_util::CreateSocket(0);
	g_object_set(G_OBJECT(udpsink), "sockfd", (gint)send_socket, NULL);   
	// Set up udpsink
	g_object_set(G_OBJECT(udpsink), "host", pipeline->_dest_host.c_str(), NULL);
	g_object_set(G_OBJECT(udpsink), "port", pipeline->_dest_port, NULL);
	g_object_set(G_OBJECT(udpsink), "sync", FALSE, NULL);
	g_object_set(G_OBJECT(udpsink), "async", FALSE, NULL);

	// Add to pipeline
	gst_bin_add(GST_BIN(pipeline->_pipeline), udpsink);

	// Connect the new pad to our udpsink
	GstPad* sinkpad = gst_element_get_static_pad(udpsink, "sink");
	gst_pad_link(pad, sinkpad);
	g_object_unref(sinkpad);

	debug::Printf("SenderPipeline: My SSRC %u\n", ssrc);
	pipeline->_receier_pipeline->SetSSRC(ssrc);

	gst_element_sync_state_with_parent(udpsink);
	gst_element_set_state(pipeline->_pipeline, GST_STATE_PLAYING);

}

SenderPipeline::SenderPipeline(const std::string& host, int udp_port, ReceiverPipeline* receiver_pipeline)
	: _receier_pipeline(receiver_pipeline),
	_dest_host(host),
	_dest_port(udp_port)
{
	debug::Printf("SenderPipeline: %s : %d\n", host.c_str(), udp_port);
	
    // Create the pipeline
    _pipeline = gst_pipeline_new ("sender_pipeline");

	// Create our elements for taking audio input and sending it to the server.
	GstElement* rtpbin = gst_element_factory_make("gstrtpbin", NULL);
	
#ifdef _WIN32
    GstElement* audio_src = gst_element_factory_make("dshowaudiosrc", NULL);
#else
	GstElement* audio_src = gst_element_factory_make("autoaudiosrc", NULL);
#endif
	
	GstElement* encoder = gst_element_factory_make("speexenc", NULL);
	GstElement* rtppay = gst_element_factory_make("rtpspeexpay", NULL);


    // Add the elements to the pipeline and link them together
    gst_bin_add_many(GST_BIN (_pipeline), rtpbin, audio_src, encoder, rtppay, NULL);
	// Link: [Audio src] -> [Encoder] -> [RTP payloader]
    gst_element_link_many(audio_src, encoder, rtppay, NULL);

	// Connect rtppay src to the sink for session 0 in the rtpbin
	GstPad* sinkpad = gst_element_get_request_pad(rtpbin, "send_rtp_sink_0");
	GstPad* srcpad = gst_element_get_static_pad(rtppay, "src");

	// Link: [rtppay] -> [rtpbin.send_rtp_sink_0]
	gst_pad_link(srcpad, sinkpad);

	gst_object_unref(srcpad);
	gst_object_unref(sinkpad);
	
	// An ugly way to retrieve the SSRC this sender will get:
	//	We create a ssrc demuxer and when the pipeline notifies about a new pad in the demuxer we save the SSRC.
	GstElement* rtpssrcdemux = gst_element_factory_make("gstrtpssrcdemux", NULL);
    gst_bin_add(GST_BIN (_pipeline), rtpssrcdemux);
	g_signal_connect(rtpssrcdemux, "new-ssrc-pad", G_CALLBACK (new_ssrc_pad_cb), this);

	// Then connect the rtpbin src to the SSRC demuxer
	sinkpad = gst_element_get_static_pad(rtpssrcdemux, "sink");
	srcpad = gst_element_get_static_pad(rtpbin, "send_rtp_src_0");

	// [rtpbin.send_rtp_src_0] -> [rtpssrcdemux] (and later -> [udpsink])
	gst_pad_link(srcpad, sinkpad);
	
	gst_object_unref(srcpad);
	gst_object_unref(sinkpad);

	// The rest of the elements will be linked later in callback new_ssrc_pad_cb.


    // Get the bus for the newly created pipeline.
    GstBus* bus = gst_element_get_bus(_pipeline);

    // Create our bus object.
    _bus = new Bus(bus);
	_bus->SetListener(this);

    // Unreference the bus here, all further use of the bus will be from the Bus object.
    gst_object_unref(bus);
	
	gst_element_set_state(_pipeline, GST_STATE_PLAYING);
}
SenderPipeline::~SenderPipeline()
{
    delete _bus;
    _bus = NULL;

    if(_pipeline)
    {
        gst_element_set_state(_pipeline, GST_STATE_PAUSED);
        gst_element_set_state(_pipeline, GST_STATE_NULL);
        gst_object_unref(_pipeline);
        _pipeline = NULL;
    }
}

void SenderPipeline::Tick()
{
	_bus->Poll();
}

void SenderPipeline::Error(const std::string& error)
{
	debug::Printf("[Error] SenderPipeline: %s\n", error.c_str());
}
