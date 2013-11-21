#include "common.h"

#include "pipeline.h"
#include "bus.h"

Pipeline::Pipeline(const char* pipeline_factory) : _pipeline(NULL), _bus(NULL)
{
	// Initialize gstreamer
	gst_init(NULL, NULL);

	// Create pipeline
	_pipeline = gst_element_factory_make(
		pipeline_factory, // Name of the pipeline factory to use
		NULL // Specifying NULL here means gstreamer will generate an unique name for us.
	);
	if(!_pipeline)
	{
		debug::Printf("[Error] Failed to create pipeline.");
		return;
	}

	// Get the bus for the newly created pipeline.
	GstBus* bus = gst_element_get_bus(_pipeline);

	// Create our bus object.
	_bus = new Bus(bus);

	// Unreference the bus here, all further use of the bus will be from the Bus object.
	gst_object_unref(bus);

	g_object_set(_pipeline, "uri", "http://docs.gstreamer.com/media/sintel_trailer-480p.webm", NULL);
	gst_element_set_state(_pipeline, GST_STATE_PLAYING);
}
Pipeline::~Pipeline()
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


