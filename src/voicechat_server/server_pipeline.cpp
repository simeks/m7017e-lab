#include "shared/common.h"

#include "server_pipeline.h"

#include <sstream>

ServerPipeline::ServerPipeline(int udp_port)
{
	_pipeline = gst_pipeline_new("channel_pipeline");
	
	GstBus* bus = gst_element_get_bus(_pipeline);
	_bus = new Bus(bus);
	_bus->SetListener(this);
	
	std::stringstream uri;
	uri << "udp://localhost:" << udp_port;
	GstElement* udpsrc = gst_element_factory_make("udpsrc", NULL);
	g_object_set(G_OBJECT(udpsrc), "uri", uri.str().c_str(), NULL);    
	
	GstCaps* caps = gst_caps_from_string("application/x-rtp, media=(string)audio, clock-rate=(int)44100, encoding-name=(string)L16, encoding-params=(string)1, channels=(int)1, payload=(int)96");
	g_object_set(G_OBJECT(udpsrc), "caps", caps, NULL);
    gst_caps_unref (caps);

	_udp_sink = gst_element_factory_make("multiudpsink", NULL);

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
ServerPipeline::~ServerPipeline()
{
	if(_pipeline)
		g_object_unref(_pipeline);

	delete _bus;
}
void ServerPipeline::Tick()
{
	_bus->Poll();
}
void ServerPipeline::AddReceiver(int user_id, const std::string& addr, int port)
{
	Receiver recv;
	recv.user_id = user_id;
	recv.address = addr;
	recv.port = port;

	_receivers.push_back(recv);

	UpdateReceivers();
}
void ServerPipeline::RemoveReceiver(int user_id)
{
	for(std::vector<Receiver>::iterator it = _receivers.begin(); it != _receivers.end(); ++it)
	{
		if(it->user_id == user_id)
		{
			_receivers.erase(it);
			return;
		}
	}
	UpdateReceivers();
}

void ServerPipeline::Error(const std::string& error)
{
	debug::Printf("[Error] ChannelPipeline: %s\n", error.c_str());
}

void ServerPipeline::UpdateReceivers()
{
	std::stringstream ss;
	for(std::vector<Receiver>::iterator it = _receivers.begin(); it != _receivers.end(); ++it)
	{
		ss << it->address << ":" << it->port;
		
		if((it+1) != _receivers.end())
			ss << ",";
	}

	g_object_set(G_OBJECT(_udp_sink), "clients", ss.str().c_str(), NULL);
}
