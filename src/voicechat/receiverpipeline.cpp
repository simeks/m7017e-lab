#include "shared/common.h"
#include "receiverpipeline.h"

#include <sstream>

// Called when there's a new pad that needs to be connected in our rtpbin
void ReceiverPipeline::pad_added_cb(GstElement* , GstPad* new_pad, gpointer user_data)
{
	debug::Printf("New pad: %s\n", gst_pad_get_name(new_pad));
	std::string pad_name = gst_pad_get_name(new_pad);
	if(pad_name == "recv_rtp_sink_0" ||
		pad_name == "send_rtp_sink_0" || 
		pad_name == "send_rtp_src_0")
	{
		return;
	}
	
	ReceiverPipeline* receiver_pipeline = (ReceiverPipeline*)user_data;
	
	// Pad names look like this: recv_rtp_src_<session-id>_<ssrc>_<payload>
	std::string ssrc_str = pad_name.substr(13);
	ssrc_str = ssrc_str.substr(ssrc_str.find('_')+1);
	uint32_t ssrc = strtoul(ssrc_str.c_str(), NULL, 0);
	
	if(receiver_pipeline->_ssrc != ssrc)
	{
		if(!receiver_pipeline->_adder)
		{
			receiver_pipeline->_adder = gst_element_factory_make("adder", NULL);

		#ifdef _WIN32
			// Use directsound directly on win32 because autoaudiosink only causes stuttering.
			GstElement* audiosink = gst_element_factory_make("directsoundsink", NULL); 
			g_object_set(G_OBJECT(audiosink), "sync", FALSE, NULL);
		#else
			GstElement* audiosink = gst_element_factory_make("autoaudiosink", NULL);
		#endif
	
			// Add the elements to our pipeline
			gst_bin_add_many(GST_BIN(receiver_pipeline->_pipeline), audiosink, receiver_pipeline->_adder, NULL);
			gst_element_sync_state_with_parent(audiosink);
			gst_element_sync_state_with_parent(receiver_pipeline->_adder);

			// Link the elements
			// [Adder] -> [Audiosink]
			gst_element_link(receiver_pipeline->_adder, audiosink);

			GstElement* depay = gst_element_factory_make("rtpspeexdepay", NULL);
			GstElement* decoder = gst_element_factory_make("speexdec", NULL);
			GstElement* queue = gst_element_factory_make("queue", NULL);
			gst_bin_add_many(GST_BIN(receiver_pipeline->_pipeline), depay, decoder, NULL);
			gst_element_sync_state_with_parent(depay);
			gst_element_sync_state_with_parent(decoder);

			// [Depay] -> [Decoder] -> [Adder] 
			gst_element_link_many(depay, decoder, receiver_pipeline->_adder, NULL);

			GstPad* sinkpad = gst_element_get_static_pad(depay, "sink");

			// [RtpBin] -> [Depay] 
			gst_pad_link(new_pad, sinkpad);
			gst_object_unref(sinkpad);
		}
		else
		{
			GstElement* depay = gst_element_factory_make("rtpspeexdepay", NULL);
			GstElement* decoder = gst_element_factory_make("speexdec", NULL);
			gst_bin_add_many(GST_BIN(receiver_pipeline->_pipeline), depay, decoder, NULL);
			gst_element_sync_state_with_parent(depay);
			gst_element_sync_state_with_parent(decoder);
			
			// [Depay] -> [Decoder] -> [Adder]
			gst_element_link_many(depay, decoder, receiver_pipeline->_adder, NULL);

			GstPad* sinkpad = gst_element_get_static_pad(depay, "sink");

			// [RtpBin] -> [Depay] -> [Decoder] -> [Adder]
			gst_pad_link(new_pad, sinkpad);
			gst_object_unref(sinkpad);
		}
	}
	else
	{
		GstElement* fakesink = gst_element_factory_make("fakesink", NULL);
		gst_bin_add_many(GST_BIN(receiver_pipeline->_pipeline), fakesink, NULL);

		GstPad* sinkpad = gst_element_get_static_pad(fakesink, "sink");
		gst_pad_link(new_pad, sinkpad);
		gst_object_unref(sinkpad);

	}
}
ReceiverPipeline::ReceiverPipeline(int udp_port) : _pipeline(NULL), _bus(NULL), _adder(NULL), _ssrc(-1)
{
    // Create the pipeline
    _pipeline = gst_pipeline_new ("receiver_pipeline");

	std::stringstream rtp_uri;
	rtp_uri << "udp://::1:" << udp_port;
	
	_rtpbin = gst_element_factory_make("gstrtpbin", NULL);
	GstElement* queue = gst_element_factory_make("queue", NULL);
    GstElement* udpsrc = gst_element_factory_make("udpsrc", NULL);
	g_object_set(G_OBJECT(udpsrc), "uri", rtp_uri.str().c_str(), NULL); 


	GstCaps* caps = gst_caps_from_string("application/x-rtp, media=(string)audio, clock-rate=(int)44100, encoding-name=(string)SPEEX");
 	g_object_set(G_OBJECT(udpsrc), "caps", caps, NULL);
    gst_caps_unref(caps);

    // Add the elements to the pipeline and link them together
    gst_bin_add_many (GST_BIN (_pipeline), _rtpbin, udpsrc, queue, NULL);
	gst_element_link(udpsrc, queue);
	
	// Retrieve the sink from the rtpbin for the specified session.
	GstPad* sinkpad = gst_element_get_request_pad(_rtpbin, "recv_rtp_sink_0");
	
	// Connect udpsrc to the sink for specfied session in the rtpbin
	GstPad* srcpad = gst_element_get_static_pad(queue, "src");

	// Link: [queue] -> [rtpbin.recv_rtp_sink_0]
	gst_pad_link(srcpad, sinkpad);
	gst_object_unref(srcpad);
	gst_object_unref(sinkpad);

    // Get the bus for the newly created pipeline.
    GstBus* bus = gst_element_get_bus(_pipeline);

    // Create our bus object.
    _bus = new Bus(bus);
	_bus->SetListener(this);

    // Unreference the bus here, all further use of the bus will be from the Bus object.
    gst_object_unref(bus);
	
    g_signal_connect(_rtpbin, "pad-added", G_CALLBACK (pad_added_cb), this);

	gst_element_set_state(_pipeline, GST_STATE_PLAYING);
}
ReceiverPipeline::~ReceiverPipeline ()
{
    delete _bus;
    _bus = NULL;

    if(_pipeline)
    {
        gst_element_set_state(_pipeline, GST_STATE_NULL);
        gst_object_unref(_pipeline);
        _pipeline = NULL;
    }
}

void ReceiverPipeline::SetSSRC(uint32_t ssrc)
{
	_ssrc = ssrc;
}

void ReceiverPipeline::Tick()
{
	_bus->Poll();
}
void ReceiverPipeline::Error(const std::string& error)
{
	debug::Printf("[Error] SenderPipeline: %s\n", error.c_str());
}
