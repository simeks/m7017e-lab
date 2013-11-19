#ifndef __BUS_H__
#define __BUS_H__

#include <gst/gst.h>

class Bus
{
public:
	Bus(GstBus* bus);
	~Bus();

private:
	GstBus* _bus;

};


#endif // __BUS_H__
