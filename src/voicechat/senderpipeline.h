#ifndef SENDERPIPELINE_H
#define SENDERPIPELINE_H

#include <gst/gst.h>
#include "../shared/bus.h"

class SenderPipeline : public PipelineListener
{
public:
    SenderPipeline(const std::string& host, int udp_port);
    ~SenderPipeline();

	void Tick();
	
	/// Notifies that an error as occured in the pipeline.
	void Error(const std::string& );

private:
    GstElement* _pipeline;
    GstElement* _queue;
    GstElement* _encoder;
    GstElement* _queue2;
    GstElement* _udpsink;
    GstElement* _audio_src;
    Bus* _bus;
};

#endif // SENDERPIPELINE_H
