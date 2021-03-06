#include "shared/common.h"

#include "channelpipeline.h"

#include <sstream>

#include <WinSock2.h>

namespace socket_util
{
	void InitializeWinSock()
	{
		static bool s_initializd = false;
		if(!s_initializd)
		{
			WSAData data;
			WSAStartup(MAKEWORD(2,2), &data);

			s_initializd = true;
		}
	}

	SOCKET CreateSocket(int port)
	{
		InitializeWinSock();

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
		addr.sin_port = htons(port);

		SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		bind(s, (sockaddr*)&addr, sizeof(addr));

		return s;
	}


};


ChannelPipeline::ChannelPipeline(int udp_port)
{
	_pipeline = gst_pipeline_new("channel_pipeline");
	
	GstBus* bus = gst_element_get_bus(_pipeline);
	_bus = new Bus(bus);
	_bus->SetListener(this);
	
	
	GstElement* udpsrc = gst_element_factory_make("udpsrc", NULL);

	SOCKET recv_socket = socket_util::CreateSocket(udp_port);
	g_object_set(G_OBJECT(udpsrc), "sockfd", (gint)recv_socket, NULL);    
	
	GstCaps* caps = gst_caps_from_string("application/x-rtp, media=(string)audio, clock-rate=(int)44100, encoding-name=(string)SPEEX");
	g_object_set(G_OBJECT(udpsrc), "caps", caps, NULL);
    gst_caps_unref (caps);

	_udp_sink = gst_element_factory_make("multiudpsink", NULL);
	
	SOCKET send_socket = socket_util::CreateSocket(0);
	g_object_set(G_OBJECT(_udp_sink), "sockfd", (gint)send_socket, NULL);   

	// Add our elements to the pipeline (The sources will be added at a later time)
	gst_bin_add_many(GST_BIN(_pipeline), udpsrc, _udp_sink, NULL);

	// Link elements: [udpsrc*] -> [multiudpsink]
	// * sources will be added at a later time.
	if(!gst_element_link_many(udpsrc, _udp_sink, NULL))
	{
		debug::Printf("[Error] ServerPipeline: Failed to link elements.\n");
	}
	
	gst_element_set_state(_pipeline, GST_STATE_PLAYING);
}
ChannelPipeline::~ChannelPipeline()
{
	if(_pipeline)
		g_object_unref(_pipeline);

	delete _bus;
}
void ChannelPipeline::Tick()
{
	_bus->Poll();
}
void ChannelPipeline::AddReceiver(int user_id, const std::string& addr, int port)
{
	Receiver recv;
	recv.user_id = user_id;
	recv.address = addr;
	recv.port = port;

	_receivers.push_back(recv);

	UpdateReceivers();
}
void ChannelPipeline::RemoveReceiver(int user_id)
{
	for(std::vector<Receiver>::iterator it = _receivers.begin(); it != _receivers.end(); ++it)
	{
		if(it->user_id == user_id)
		{
			_receivers.erase(it);
			break;
		}
	}
	UpdateReceivers();
}

void ChannelPipeline::Error(const std::string& error)
{
	debug::Printf("[Error] ChannelPipeline: %s\n", error.c_str());
}

void ChannelPipeline::UpdateReceivers()
{
	std::stringstream ss;
	for(std::vector<Receiver>::iterator it = _receivers.begin(); it != _receivers.end(); ++it)
	{
		if(it->address == "localhost")
			ss << "127.0.0.1:" << it->port;
		else
			ss << it->address << ":" << it->port;
		
		if((it+1) != _receivers.end())
			ss << ",";
	}

	g_object_set(G_OBJECT(_udp_sink), "clients", ss.str().c_str(), NULL);
}
