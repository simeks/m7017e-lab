#include "receiverpipeline.h"
#include "shared/common.h"

#include <sstream>

ReceiverPipeline ::ReceiverPipeline (int udp_port) : _pipeline(NULL), _bus(NULL)
{
    // Create the pipeline
    _pipeline = gst_pipeline_new ("receiver_pipeline");
		
    // Create the elements
    _udpsrc = gst_element_factory_make("udpsrc", NULL);
    _queue = gst_element_factory_make("queue", NULL);
    _audio_sink = gst_element_factory_make ("autoaudiosink", NULL);
    _decoder = gst_element_factory_make ("mad", NULL);
    _converter = gst_element_factory_make ("audioconvert", NULL); // good to have after the decoder
    _resample = gst_element_factory_make ("audioresample", NULL); // good to have after the decoder

	std::stringstream uri;
	uri << "udp://localhost:" << udp_port;
	g_object_set(G_OBJECT(_udpsrc), "uri", uri.str().c_str(), NULL);    
	
	GstCaps* caps = gst_caps_from_string("audio/x-raw-int, endianness=(int)1234,signed=(boolean)true, width=(int)16, depth=(int)16, rate=(int)44100, channels=(int)1");
 	g_object_set(G_OBJECT(_udpsrc), "caps", caps, NULL);
    gst_caps_unref (caps);

    // Add the elements to the pipeline and link them together
    //gst_bin_add_many (GST_BIN (_pipeline), _udpsrc, _decoder, _converter, _resample, _audio_sink, NULL);
    //gst_element_link_many (_udpsrc, _decoder, _converter, _resample, _audio_sink, NULL);
    gst_bin_add_many (GST_BIN (_pipeline), _udpsrc, _audio_sink, NULL);
    gst_element_link_many (_udpsrc, _audio_sink, NULL);

    // Get the bus for the newly created pipeline.
    GstBus* bus = gst_element_get_bus(_pipeline);

    // Create our bus object.
    _bus = new Bus(bus);
	_bus->SetListener(this);

    // Unreference the bus here, all further use of the bus will be from the Bus object.
    gst_object_unref(bus);
	
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
