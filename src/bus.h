#ifndef __BUS_H__
#define __BUS_H__

#include <gst/gst.h>

class PipelineListener;

/// Class responsible for polling messages from the pipeline.
class Bus
{
public:
	Bus(GstBus* bus);
	~Bus();

	/// Polls and handles messages on the bus. This should be called within a regular interval.
	void Poll();

	/// Sets a listener for the bus, this listener will receive callbacks whenever the bus gets a message.
	void SetListener(PipelineListener* listener);

private:
	void Parse(GstMessage* msg);

	GstBus* _bus;
	PipelineListener* _listener;

};


#endif // __BUS_H__
