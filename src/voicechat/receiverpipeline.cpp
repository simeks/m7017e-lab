#include "receiverpipeline.h"
#include "shared/common.h"

#include <sstream>

namespace receiver_pipeline
{
	// Called when there's a new pad that needs to be connected in our rtpbin
	static void pad_added_cb(GstElement* rtpbin, GstPad* new_pad, gpointer user_data)
	{
		GstElement* pipeline = (GstElement*)user_data;
		//gst_element_set_state(pipeline, GST_STATE_PAUSED);
		
		debug::Printf("New payload: %s\n", GST_PAD_NAME (new_pad));

		GstElement* depay = gst_element_factory_make("rtpL16depay", NULL);
		GstElement* audiosink = gst_element_factory_make("autoaudiosink", NULL);

		// Add the elements to our pipeline
		gst_bin_add_many(GST_BIN(pipeline), depay, audiosink, NULL);
		
		// Change the state of our new elements
		gst_element_sync_state_with_parent(depay);
		gst_element_sync_state_with_parent(audiosink);

		// Link the elements
		gst_element_link_many(depay, audiosink, NULL);

		// Connect the new rtp src to to the depay sink
		GstPad* sinkpad = gst_element_get_static_pad(depay, "sink");
		
		debug::Printf("New pad caps is %s \n", gst_caps_to_string (gst_pad_get_caps (new_pad)));
		debug::Printf("Depay pad caps is %s \n", gst_caps_to_string (gst_pad_get_caps (sinkpad)));

		gst_pad_link(new_pad, sinkpad);
		gst_object_unref(sinkpad);
		
		//gst_element_set_state(pipeline, GST_STATE_PLAYING);
	}
};

ReceiverPipeline::ReceiverPipeline (int udp_port) : _pipeline(NULL), _bus(NULL)
{
    // Create the pipeline
    _pipeline = gst_pipeline_new ("receiver_pipeline");
		
    // Create the elements
    GstElement* udpsrc = gst_element_factory_make("udpsrc", NULL);
    GstElement* rtpbin = gst_element_factory_make("gstrtpbin", NULL);

	std::stringstream uri;
	uri << "udp://localhost:" << 12345;
	g_object_set(G_OBJECT(udpsrc), "uri", uri.str().c_str(), NULL);    

	GstCaps* caps = gst_caps_from_string("application/x-rtp, media=(string)audio, clock-rate=(int)44100, encoding-name=(string)L16, encoding-params=(string)1, channels=(int)1, payload=(int)96");
 	g_object_set(G_OBJECT(udpsrc), "caps", caps, NULL);
    gst_caps_unref (caps);

    // Add the elements to the pipeline and link them together
    gst_bin_add_many (GST_BIN (_pipeline), udpsrc, rtpbin, NULL);

	// Connect udpsrc to the sink for session0 in the rtpbin
	GstPad* srcpad = gst_element_get_static_pad(udpsrc, "src");
	GstPad* sinkpad = gst_element_get_request_pad(rtpbin, "recv_rtp_sink_0");
	gst_pad_link(srcpad, sinkpad);
	gst_object_unref(srcpad);
	gst_object_unref(sinkpad);

	GstElement* rtcp_src = gst_element_factory_make("udpsrc", NULL);
	uri.clear();
	uri << "udp://localhost:" << (int)12346;
	g_object_set(G_OBJECT(rtcp_src), "uri", uri.str().c_str(), NULL); 

	GstElement* rtcp_sink = gst_element_factory_make("udpsink", NULL);
	g_object_set(G_OBJECT(rtcp_sink), "port", 12347, NULL);
	g_object_set(G_OBJECT(rtcp_sink), "host", "localhost", NULL);
	g_object_set(G_OBJECT(rtcp_sink), "async", FALSE, NULL);
	g_object_set(G_OBJECT(rtcp_sink), "sync", FALSE, NULL);

	gst_bin_add_many(GST_BIN(_pipeline), rtcp_src, rtcp_sink, NULL);

	srcpad = gst_element_get_static_pad(rtcp_src, "src");
	sinkpad = gst_element_get_request_pad(rtpbin, "recv_rtcp_sink_0");
	gst_pad_link(srcpad, sinkpad);
	gst_object_unref(srcpad);
	gst_object_unref(sinkpad);

	srcpad = gst_element_get_request_pad(rtpbin, "send_rtcp_src_0");
	sinkpad = gst_element_get_static_pad(rtcp_sink, "sink");
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
	
    g_signal_connect(rtpbin, "pad-added", G_CALLBACK (receiver_pipeline::pad_added_cb), _pipeline);

	gst_element_set_state(_pipeline, GST_STATE_PLAYING);
}
ReceiverPipeline ::~ReceiverPipeline ()
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

void ReceiverPipeline::Tick()
{
	_bus->Poll();
}
void ReceiverPipeline ::Error(const std::string& error)
{
	debug::Printf("[Error] SenderPipeline: %s\n", error.c_str());
}
