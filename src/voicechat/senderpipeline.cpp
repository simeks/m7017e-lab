#include "senderpipeline.h"
#include "shared/common.h"

#include <sstream>


SenderPipeline::SenderPipeline(const std::string& host, int udp_port)
{
    // Create the pipeline
    _pipeline = gst_pipeline_new ("sender_pipeline");

    // Create the elements
    GstElement* audio_src = gst_element_factory_make("audiotestsrc", NULL);
    GstElement* udpsink = gst_element_factory_make("udpsink", NULL);
	GstElement* rtpbin = gst_element_factory_make("gstrtpbin", NULL);
	GstElement* rtppay = gst_element_factory_make("rtpspeexpay", NULL);
	GstElement* encoder = gst_element_factory_make("speexenc", NULL);

	g_object_set(G_OBJECT(udpsink), "host", host.c_str(), NULL);
	g_object_set(G_OBJECT(udpsink), "port", udp_port, NULL);
	g_object_set(G_OBJECT(udpsink), "sync", FALSE, NULL);
	g_object_set(G_OBJECT(udpsink), "async", FALSE, NULL);

    // Add the elements to the pipeline and link them together
    gst_bin_add_many (GST_BIN (_pipeline), audio_src, encoder, rtppay, rtpbin, udpsink, NULL);
	// Connect audio src tp rtppay
    gst_element_link_many(audio_src, encoder, rtppay, NULL);
	// Connect rtppay src to the sink for session 0 in the rtpbin
	GstPad* sinkpad = gst_element_get_request_pad(rtpbin, "send_rtp_sink_0");
	GstPad* srcpad = gst_element_get_static_pad(rtppay, "src");
	gst_pad_link(srcpad, sinkpad);

	gst_object_unref(srcpad);
	gst_object_unref(sinkpad);

	// Then connect the rtpbin src to the udpsink
	sinkpad = gst_element_get_static_pad(udpsink, "sink");
	srcpad = gst_element_get_static_pad(rtpbin, "send_rtp_src_0");
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
	
	gst_element_set_state(_pipeline, GST_STATE_PLAYING);
}
SenderPipeline::~SenderPipeline()
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
void SenderPipeline::Tick()
{
	_bus->Poll();
}

void SenderPipeline::Error(const std::string& error)
{
	debug::Printf("[Error] SenderPipeline: %s\n", error.c_str());
}