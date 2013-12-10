#ifndef __UDPSOURCE_H__
#define __UDPSOURCE_H__

#include <gst/gst.h>

/// An element for receiving streaming data by UDP, 
///		this element can be connected to a ChannelPipeline.
class UDPSource
{
public:
	/// Constructor
	/// @param udp_port Specifies which udp port to listen on.
	UDPSource(int udp_port);
	~UDPSource();


private:
	int _udp_port;

	GstElement* _udpsrc;
	GstElement* _queue;
};


#endif // __UDPSOURCE_H__
