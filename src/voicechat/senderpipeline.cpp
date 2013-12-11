#include "senderpipeline.h"
#include "../shared/common.h"

SenderPipeline::SenderPipeline(const std::string& host, int udp_port)
{
    // Create the pipeline
    _pipeline = gst_pipeline_new ("sender_pipeline");

    // Create the elements
    _queue = gst_element_factory_make("queue", NULL);
    _queue2 = gst_element_factory_make("queue", NULL);
    _udpsink = gst_element_factory_make("udpsink", NULL);
    _encoder = gst_element_factory_make("ffenc_mp2", NULL);
    _audio_src = gst_element_factory_make("audiotestsrc", NULL);
	
	g_object_set(G_OBJECT(_udpsink), "host", host.c_str(), NULL);
	g_object_set(G_OBJECT(_udpsink), "port", udp_port, NULL);

    // Add the elements to the pipeline and link them together
    //gst_bin_add_many (GST_BIN (_pipeline), _audio_src, _queue, _encoder, _queue2, _udpsink, NULL);
    //gst_element_link_many (_audio_src, _queue, _encoder, _queue2, _udpsink, NULL);
	
    gst_bin_add_many (GST_BIN (_pipeline), _audio_src, _udpsink, NULL);
    gst_element_link_many (_audio_src, _udpsink, NULL);

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