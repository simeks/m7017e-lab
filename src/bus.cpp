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

	/// @brief Callback invoked when the pipeline reaches end of stream.
	void bus_eos_callback(GstBus* gst_bus, GstMessage* msg, void* data)
	{
		debug::Printf("message::eos");
	}

	void bus_state_change_callback(GstBus* gst_bus, GstMessage* msg, void* data)
	{
		debug::Printf("message::state-changed"); 
	}

	void bus_application_callback(GstBus* gst_bus, GstMessage* msg, void* data)
	{
		debug::Printf("message::application"); 
	}
};

Bus::Bus(GstBus* bus) : _bus(bus)
{
	// Increment the reference count to specify that we are using this object
	gst_object_ref(_bus);

	// Register callbacks for signals
	gst_bus_add_signal_watch(_bus);
	g_signal_connect(G_OBJECT(_bus), "message::error", (GCallback)bus_error_callback, this);
	g_signal_connect(G_OBJECT(_bus), "message::eos", (GCallback)bus_eos_callback, this);
	g_signal_connect(G_OBJECT(_bus), "message::state-changed", (GCallback)bus_eos_callback, this);
	g_signal_connect(G_OBJECT(_bus), "message::application", (GCallback)bus_application_callback, this);

}
Bus::~Bus()
{
	// Remove the signal watch added in Bus::Bus
	gst_bus_remove_signal_watch(_bus); 

	// Release the bus
	gst_object_unref(_bus);
	_bus = NULL;
}


