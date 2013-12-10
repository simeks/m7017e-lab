#include "senderpipeline.h"
#include "../shared/common.h"

SenderPipeline::SenderPipeline()
{
    // Initialize Gstreamer
    gst_init(NULL, NULL);

    // Create the pipeline
    _pipeline = gst_pipeline_new ("my_pipeline");

    // Create the elements
    _queue = gst_element_factory_make("queue", NULL);
    _queue2 = gst_element_factory_make("queue", NULL);
    _udpsink = gst_element_factory_make("udpsink", NULL);
    _encoder = gst_element_factory_make("ffenc_mp2", NULL);
    _alsaSrc = gst_element_factory_make("alsasrc", NULL);

    // Add the elements to the pipeline and link them together
    gst_bin_add_many (GST_BIN (_pipeline), _alsaSrc, _queue, _encoder, _queue2, _udpsink, NULL);
    gst_element_link_many (_alsaSrc, _queue, _encoder, _queue2, _udpsink, NULL);

    // Get the bus for the newly created pipeline.
    GstBus* bus = gst_element_get_bus(_pipeline);

    // Create our bus object.
    _bus = new Bus(bus);

    // Unreference the bus here, all further use of the bus will be from the Bus object.
    gst_object_unref(bus);

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
