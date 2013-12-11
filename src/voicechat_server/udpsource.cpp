#include "shared/common.h"

#include "udpsource.h"

UDPSource::UDPSource(int udp_port)
	: _udp_port(udp_port)
{
	_udpsrc = gst_element_factory_make("udpsrc", NULL);
	_queue = gst_element_factory_make("queue", NULL);

	g_object_set(G_OBJECT(_udpsrc), "port", _udp_port, NULL);
}
UDPSource::~UDPSource()
{
}
