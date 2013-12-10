#include "shared/common.h"

#include "channel_pipeline.h"


ChannelPipeline::ChannelPipeline()
{
	_pipeline = gst_pipeline_new("channel_pipeline");
	
	GstBus* bus = gst_element_get_bus(_pipeline);
	_bus = new Bus(bus);
	_bus->SetListener(this);

	_src_adder = gst_element_factory_make("adder", NULL);
	_udp_sink = gst_element_factory_make("multiudpsink", NULL);

	GstElement* queue = gst_element_factory_make("queue", NULL);

	// Add our elements to the pipeline (The sources will be added at a later time)
	gst_bin_add_many(GST_BIN(_pipeline), _src_adder, queue, _udp_sink, NULL);

	// Link elements: [udpsrc*] -> [adder] -> [queue] -> [multiudpsink]
	// * sources will be added at a later time.
	if(!gst_element_link_many(_src_adder, queue, _udp_sink, NULL))
	{
		debug::Printf("[Error] ChannelPipeline: Failed to link elements.\n");
	}
}
ChannelPipeline::~ChannelPipeline()
{
	if(_pipeline)
		g_object_unref(_pipeline);

	delete _bus;
}

void ChannelPipeline::Error(const std::string& error)
{
	debug::Printf("[Error] ChannelPipeline: %s\n", error.c_str());
}