#ifndef RECIEVERPIPELINE_H
#define RECIEVERPIPELINE_H

#include <gst/gst.h>
#include "shared/bus.h"

#include <map>
#include <QMutex>

class ReceiverPipeline : public PipelineListener
{
public:
	/// Constructor
	/// @param udp_port Specifies which port to listen for incoming data.
    ReceiverPipeline (int udp_port);
    ~ReceiverPipeline ();

	void Tick();

	void Error(const std::string& error);

	/// Sets the clients own SSRC, retrieved from the sender, this will make sure to mute the users own voice.
	void SetSSRC(uint32_t ssrc);

private:
	// Called when there's a new pad that needs to be connected in our rtpbin
	static void pad_added_cb(GstElement* rtpbin, GstPad* new_pad, gpointer user_data);

    GstElement* _pipeline;
	GstElement* _rtpbin;
	GstElement* _adder;
    Bus* _bus;
	
	uint32_t _ssrc;

	QMutex _pad_added_lock;

};

#endif // RECIEVERPIPELINE_H
