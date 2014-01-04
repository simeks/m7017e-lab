#ifndef SENDERPIPELINE_H
#define SENDERPIPELINE_H

#include <gst/gst.h>
#include "shared/bus.h"


#ifdef _WIN32
#include <WinSock2.h>
#endif


struct GstRtpSsrcDemux;

class ReceiverPipeline;
class SenderPipeline : public PipelineListener
{
public:
	/// Constructor
	/// @param host Which host the pipeline should send the data to.
	/// @param udp_port Port at the target host.
	/// @param receiver_pipeline This clients receiver pipeline.
    SenderPipeline(const std::string& host, int udp_port, ReceiverPipeline* receier_pipeline);
    ~SenderPipeline();

	void Tick();
	
	/// Notifies that an error as occured in the pipeline.
	void Error(const std::string& );
	
private:
	// Called when there's a new pad that needs to be connected from our ssrc demuxer
	static void new_ssrc_pad_cb(GstRtpSsrcDemux *demux, guint ssrc, GstPad *pad, gpointer user_data);

	ReceiverPipeline* _receier_pipeline;
	std::string _dest_host; // Destination host
	int _dest_port; // destination udp port

    GstElement* _pipeline;

    Bus* _bus;

};

#endif // SENDERPIPELINE_H
