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
	Pipeline();
	~Pipeline();

	/// Sets which window the pipeline should output to.
	/// @param window_handle Window handle for the window that should work as canvas.
	void SetOutput(uintptr_t window_handle);

	/// @brief Sets the pipeline state.
	void SetState(GstState state);
	
	/// @brief Queries the pipeline for the current position in nanoseconds of the current stream.
	/// @param position This parameter will hold the resulting position, in nanoseconds.
	/// @return True if the query were performed successfully.
	bool QueryPosition(int64_t* position);

	/// @brief Queries the pipeline for the total duration in nanoseconds of the current stream.
	/// @param duration This parameter will hold the resulting duration, in nanoseconds.
	/// @return True if the query were performed successfully.
	bool QueryDuration(int64_t* duration);
	
	/// @brief Seeks to the specified position
	/// @return True if the operation was successful, false if not.
	bool Seek(int64_t position);

	/// @brief Sets the playback rate
	/// @param rate The playback rate, a negative value means the playback will go in reverse.
	/// @return True if the operation was successful, false if not.
	bool SetRate(double rate);


	void Pipeline::SetUri(const char* uri);


private:
	GstElement* _pipeline;
	Bus* _bus;
};


#endif //__PIPELINE_H__
