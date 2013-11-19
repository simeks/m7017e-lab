#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include <gst/gst.h>

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

private:
	GstElement* _pipeline;
};


#endif //__PIPELINE_H__
