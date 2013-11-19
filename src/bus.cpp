#include "bus.h"

namespace 
{
	void bus_error_callback(GstBus* bus, GstMessage* msg, void* data)
	{

	}


};

Bus::Bus(GstBus* bus) : _bus(bus)
{
	// Increment the reference count to specify that we are using this object
	gst_object_ref(_bus);

	// Register callbacks for signals
	gst_bus_add_signal_watch(_bus);
	g_signal_connect(G_OBJECT(_bus), "message::error", (GCallback)bus_error_callback, this);
}
Bus::~Bus()
{
	// Remove the signal watch added in Bus::Bus
	gst_bus_remove_signal_watch(_bus); 

	// Release the bus
	gst_object_unref(_bus);
	_bus = NULL;
}


