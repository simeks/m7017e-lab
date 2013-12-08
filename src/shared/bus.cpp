#include "shared/common.h"

#include "bus.h"

Bus::Bus(GstBus* bus) : _bus(bus), _listener(NULL)
{
	// Increment the reference count to specify that we are using this object
	gst_object_ref(_bus);
}
Bus::~Bus()
{
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
		GError* error;
		gchar* debug_info;

		gst_message_parse_error(msg, &error, &debug_info);
		
		std::string error_msg = error->message;

		// Notify our listener about the error
		_listener->Error(error_msg);

#ifdef DEBUG
		if(debug_info)
		{
			debug::Printf("[Debug] Error debug info: %s", debug_info);
		}
#endif

		g_clear_error(&error);
		g_free(debug_info);
	}
	else if(msg->type == GST_MESSAGE_EOS)
	{
		// Notify our listener about the EOS
		_listener->EndOfStream();
	}

}

void Bus::SetListener(PipelineListener* listener)
{
	_listener = listener;
}
