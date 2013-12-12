#ifndef RECIEVERPIPELINE_H
#define RECIEVERPIPELINE_H

#include <gst/gst.h>
#include "shared/bus.h"


class ReceiverPipeline : public PipelineListener
{
public:
    ReceiverPipeline (int udp_port);
    ~ReceiverPipeline ();
	
	void Tick();

	void Error(const std::string& error);

private:
    GstElement* _pipeline;
    Bus* _bus;
	
};

#endif // RECIEVERPIPELINE_H
