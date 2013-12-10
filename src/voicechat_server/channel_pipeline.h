#ifndef __CHANNEL_PIPELINE_H__
#define __CHANNEL_PIPELINE_H__

#include <gst/gst.h>

#include "shared/bus.h"

class UDPSource;

/// GStreamer pipeline that listens for multiple incoming streams, 
///		then mixes those streams and passes them to one more receivers.
class ChannelPipeline : public PipelineListener
{
public:
	ChannelPipeline();
	~ChannelPipeline();

	/// @brief Adds a receiver to this pipeline.
	///	Everything going through this pipeline will be sent to the specified receiver.
	void AddReceiver();

	/// @brief Removes a receiver from the pipeline.
	/// The specified receiver will no longer receive UDP packets for the stream.
	void RemoveReceiver();


	/// @brief Connects an udp source to this pipeline.
	/// The incoming stream from this source will be mixed with all the other sources and then passed to the receivers.
	/// @sa DisconnectUDPSource
	void ConnectUDPSource(UDPSource* source);

	/// @brief Disconnects an udp source from the pipeline.
	/// @sa ConnectUDPSource
	void DisconnectUDPSource(UDPSource* source);
	
	/// Notifies that an error as occured in the pipeline./*
	void Error(const std::string& );
private:
	GstElement* _pipeline;
	GstElement* _src_adder; // Adder that mixes all the udp sources.
	GstElement* _udp_sink;

	Bus* _bus;
};


#endif // __CHANNEL_PIPELINE_H__
