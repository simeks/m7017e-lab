#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include <gst/gst.h>
#include <Qstring>

class Bus;

/// @brief Class representing a gstreamer pipeline.
class Pipeline
{
public:
	/// @brief Pipeline constructor
	/// @param pipeline_factory Name of the factory to use when creating the gstreamer pipeline. (E.g. playbin2)
	Pipeline(const char* pipeline_factory);
	~Pipeline();

	/// Sets which window the pipeline should output to.
	/// @param window_handle Window handle for the window that should work as canvas.
	void SetOutput(guintptr window_handle);

	/// @brief Sets the pipeline state.
	void SetState(GstState state);
	
	/// @brief Queries the pipeline for the current position in nanoseconds of the current stream.
	/// @param duration This parameter will hold the resulting position, in nanoseconds.
	/// @return True if the query were performed successfully.
	bool QueryPosition(int64_t* duration);

	/// @brief Queries the pipeline for the total duration in nanoseconds of the current stream.
	/// @param duration This parameter will hold the resulting duration, in nanoseconds.
	/// @return True if the query were performed successfully.
	bool QueryDuration(int64_t* duration);

	void Pipeline::SetUri(QString fileName);


private:
	GstElement* _pipeline;
	Bus* _bus;
};


#endif //__PIPELINE_H__
