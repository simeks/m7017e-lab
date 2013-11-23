#include "common.h"

#include "bus.h"

namespace 
{
	/// @brief Callback handling error message coming from the bus.
	void bus_error_callback(GstBus* gst_bus, GstMessage* msg, void* data)
	{
		GError* error;
		gchar* debug_info;

		gst_message_parse_error(msg, &error, &debug_info);
		debug::Printf("Error: %s, (Element: %s)", error->message, GST_OBJECT_NAME(msg->src));
		if(debug_info)
		{
			debug::Printf("Debug info: %s", debug_info);
		}

		g_clear_error(&error);
		g_free(debug_info);
	}

};

Bus::Bus(GstBus* bus) : _bus(bus)
{
	// Increment the reference count to specify that we are using this object
	gst_object_ref(_bus);

	// Register callbacks for signals
	//gst_bus_add_signal_watch(_bus); 
	//g_signal_connect(G_OBJECT(_bus), "message::error", G_CALLBACK(bus_error_callback), this);
	//g_signal_connect(G_OBJECT(_bus), "message::eos", G_CALLBACK(bus_eos_callback), this);
	//g_signal_connect(G_OBJECT(_bus), "message::state-changed", G_CALLBACK(bus_eos_callback), this);
	//g_signal_connect(G_OBJECT(_bus), "message::application", G_CALLBACK(bus_application_callback), this);

}
Bus::~Bus()
{
	// Remove the signal watch added in Bus::Bus
	gst_bus_remove_signal_watch(_bus); 

	// Release the bus
	gst_object_unref(_bus);
	_bus = NULL;
}

void Bus::Poll()
{
	g_assert(_bus);

	GstMessage* msg;
	while((msg = gst_bus_pop(_bus)) != NULL)
	{
		Parse(msg);
		gst_message_unref(msg);
	}
}

void Bus::Parse(GstMessage* msg)
{
	if(msg->type == GST_MESSAGE_ERROR)
	{
		debug::Printf("--error--");
	}
	else if(msg->type == GST_MESSAGE_EOS)
	{
		debug::Printf("--eos--");
	}
}
