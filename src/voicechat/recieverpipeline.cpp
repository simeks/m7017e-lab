#include "recieverpipeline.h"
#include "../shared/common.h"

RecieverPipeline::RecieverPipeline() : _pipeline(NULL), _bus(NULL)
{
    // Initialize Gstreamer
    gst_init(NULL, NULL);


    // Create the pipeline
    _pipeline = gst_pipeline_new ("my_pipeline");

    // Create the elements
    _udpsrc = gst_element_factory_make("udpsrc", NULL);
    _queue = gst_element_factory_make("queue", NULL);
    _audioSink = gst_element_factory_make ("autoaudiosink", NULL);
    _decoder = gst_element_factory_make ("mad", NULL);
    _converter = gst_element_factory_make ("audioconvert", NULL); // good to have after the decoder
    _resample = gst_element_factory_make ("audioresample", NULL); // good to have after the decoder

    // Add the elements to the pipeline and link them together
    gst_bin_add_many (GST_BIN (_pipeline), _udpsrc, _decoder, _converter, _resample, _audioSink, NULL);
    gst_element_link_many (_udpsrc, _decoder, _converter, _resample, _audioSink, NULL);

    // Get the bus for the newly created pipeline.
    GstBus* bus = gst_element_get_bus(_pipeline);

    // Create our bus object.
    _bus = new Bus(bus);

    // Unreference the bus here, all further use of the bus will be from the Bus object.
    gst_object_unref(bus);

}
RecieverPipeline::~RecieverPipeline()
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
