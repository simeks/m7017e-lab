#ifndef __BUS_H__
#define __BUS_H__

#include <gst/gst.h>

class PipelineListener;
class Bus
{
public:
	Bus(GstBus* bus);
	~Bus();

	/// Polls and handles messages on the bus. This should be called within a regular interval.
	void Poll();

	void SetListener(PipelineListener* listener);

private:
	void Parse(GstMessage* msg);

	GstBus* _bus;
	PipelineListener* _listener;

};


#endif // __BUS_H__
