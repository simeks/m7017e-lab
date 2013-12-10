#ifndef SENDERPIPELINE_H
#define SENDERPIPELINE_H

#include <gst/gst.h>
#include "../shared/bus.h"

class SenderPipeline : public PipelineListener
{
public:
    SenderPipeline();
    ~SenderPipeline();



private:
    GstElement* _pipeline;
    GstElement* _queue;
    GstElement* _encoder;
    GstElement* _queue2;
    GstElement* _udpsink;
    GstElement* _alsaSrc;
    Bus* _bus;
};

#endif // SENDERPIPELINE_H
