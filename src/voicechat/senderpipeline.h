#ifndef SENDERPIPELINE_H
#define SENDERPIPELINE_H

#include <gst/gst.h>
#include "shared/bus.h"


#ifdef _WIN32
#include <WinSock2.h>
#endif


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
    Bus* _bus;

	SOCKET _rtcp_socket;
};

#endif // SENDERPIPELINE_H
