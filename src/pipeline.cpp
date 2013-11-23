#include "common.h"

#include "pipeline.h"
#include "bus.h"

#include <gst/interfaces/xoverlay.h>



Pipeline::Pipeline() : _pipeline(NULL), _bus(NULL)
{
	// Initialize gstreamer
	gst_init(NULL, NULL);

	// Create pipeline
	_pipeline = gst_element_factory_make(
		"playbin2", // Name of the pipeline factory to use
		NULL // Specifying NULL here means gstreamer will generate an unique name for us.
	);

	if(!_pipeline)
	{
		debug::Printf("[Error] Failed to create pipeline.\n");
		return;
	}

	// Get the bus for the newly created pipeline.
	GstBus* bus = gst_element_get_bus(_pipeline);

	// Create our bus object.
	_bus = new Bus(bus);

	// Unreference the bus here, all further use of the bus will be from the Bus object.
	gst_object_unref(bus);

	g_object_set(_pipeline, "uri", "http://docs.gstreamer.com/media/sintel_trailer-480p.webm", NULL);
	gst_element_set_state(_pipeline, GST_STATE_READY);
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

void Pipeline::SetOutput(guintptr window_handle)
{
	g_assert(_pipeline);

	gst_x_overlay_set_window_handle(GST_X_OVERLAY(_pipeline), window_handle);
}

void Pipeline::SetState(GstState state)
{
	g_assert(_pipeline);

	if(gst_element_set_state(_pipeline, state) == GST_STATE_CHANGE_FAILURE)
		debug::Printf("Failed to set state\n");
}

bool Pipeline::QueryPosition(int64_t* position)
{
	g_assert(_pipeline);

	// GST_FORMAT_TIME gives us the position in nanoseconds.
	GstFormat fmt = GST_FORMAT_TIME;

	// Query the pipeline for the position
	return gst_element_query_position(_pipeline, &fmt, position) == TRUE;
}

bool Pipeline::QueryDuration(int64_t* duration)
{
	g_assert(_pipeline);

	// GST_FORMAT_TIME gives us the duration in nanoseconds.
	GstFormat fmt = GST_FORMAT_TIME;

	// Query the pipeline for the total duration
	return gst_element_query_duration(_pipeline, &fmt, duration) == TRUE;
}

bool Pipeline::Seek(int64_t position)
{
	g_assert(_pipeline);
	return gst_element_seek_simple(_pipeline, 
		GST_FORMAT_TIME, // Specifies that the position is specified in nanoseconds.
		(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),	// Flags: specifies to flush the pipeline and that we should seek to the 
																		//	nearest keyframe, this might be faster but less accurate.
		position) == TRUE;
}

bool Pipeline::SetRate(double rate)
{
	g_assert(_pipeline);

	// Sets the playback rate for the pipeline.
	return gst_element_seek(_pipeline, rate, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, 
		GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE) == TRUE;
}

