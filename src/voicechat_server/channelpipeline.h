#ifndef __CHANNELPIPELINE_H__
#define __CHANNELPIPELINE_H__

#include <gst/gst.h>

#include "shared/bus.h"

class UDPSource;

/// GStreamer pipeline that listens for incoming streams and passes them to the clients.
class ChannelPipeline : public PipelineListener
{
public:
	ChannelPipeline(int udp_port);
	~ChannelPipeline();

	void Tick();

	/// @brief Adds a receiver to this pipeline.
	///	Everything going through this pipeline will be sent to the specified receiver.
	/// @param user_id User id, this is used to identify the receiver.
	/// @param addr The address for the receiver.
	/// @param port Destination UDP port.
	void AddReceiver(int user_id, const std::string& addr, int port);

	/// @brief Removes a receiver from the pipeline.
	/// The specified receiver will no longer receive UDP packets for the stream.
	/// @param user_id The user id for the user that should be removed.
	void RemoveReceiver(int user_id);
	
	/// Notifies that an error as occured in the pipeline.
	void Error(const std::string& );
private:
	/// Updates the receivers in the udp sink
	void UpdateReceivers();

	struct Receiver
	{
		std::string address;
		int port;

		int user_id;
	};

	GstElement* _pipeline;
	GstElement* _udp_sink;
	Bus* _bus;

	std::vector<Receiver> _receivers;
};


#endif // __CHANNELPIPELINE_H__
