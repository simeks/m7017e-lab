#ifndef RECIEVERPIPELINE_H
#define RECIEVERPIPELINE_H

#include <gst/gst.h>
#include "../shared/bus.h"

class ReceiverPipeline : public PipelineListener
{
public:
    ReceiverPipeline (int udp_port);
    ~ReceiverPipeline ();
	
	void Tick();

	void Error(const std::string& error);

private:
    GstElement* _pipeline;
    GstElement* _udpsrc;
    GstElement* _queue;
    GstElement* _decoder;
    GstElement* _audio_sink;
    GstElement* _converter;
    GstElement* _resample;
    Bus* _bus;

};

#endif // RECIEVERPIPELINE_H
