#ifndef RTSPIN_H_
#define RTSPIN_H_
/*
	rtspin.h

	(c) 2005 Thor Sigvaldason and Isaac Richards
	Part of the mythTV project
	
	Headers for the thread that actually receives rtsp and outputs to audio
	device
*/

#include <qthread.h>
#include <qmutex.h>
#include <qstring.h>

class Speakers;
class BasicTaskScheduler;
class UsageEnvironment;
class RTSPClient;
class MediaSession;
class MediaSubsession;
class AudioOutput;

class RtspIn: public QThread
{

  public:

    RtspIn(
            Speakers *owner, 
            const QString &l_rtsp_url, 
            unsigned l_rtp_incoming_buffer_size = 65535
          );
    ~RtspIn();

    void run();
    void stop();
    void handleAfterReading(unsigned frameSize, struct timeval p_time);
    void handleSourceClosure();

  private:

    void    cleanUp();
    void    warning(const QString &warn_message);
    void    log(const QString &log_message, int verbosity_level);
    
    
    bool        keep_going;
    QMutex      keep_going_mutex;
    QString     rtsp_url;
    Speakers   *parent;
    
    //
    //  liveMedia objects
    //
    
    BasicTaskScheduler *scheduler;
    UsageEnvironment   *env;
    RTSPClient         *rtsp_client;
    MediaSession       *media_session;
    MediaSubsession    *sub_session;
    char                blocking_flag;
    unsigned char      *rtp_incoming_buffer;
    unsigned            rtp_incoming_buffer_size;
    AudioOutput        *audio_output;
};

#endif  // rtspin_h_