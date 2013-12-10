#ifndef RECIEVERPIPELINE_H
#define RECIEVERPIPELINE_H

#include <gst/gst.h>
#include "../shared/bus.h"

class RecieverPipeline : public PipelineListener
{
public:
    RecieverPipeline();
    ~RecieverPipeline();



private:
    GstElement* _pipeline;
    GstElement* _udpsrc;
    GstElement* _queue;
    GstElement* _decoder;
    GstElement* _audioSink;
    GstElement* _converter;
    GstElement* _resample;
    Bus* _bus;

};

#endif // RECIEVERPIPELINE_H
