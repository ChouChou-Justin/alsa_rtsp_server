#pragma once

#include <liveMedia.hh>
#include "alsa_capture.h"
#include "constants.h"

namespace alsa_rtsp {

class alsaPcmFramedSource : public FramedSource {
public:
    static alsaPcmFramedSource* createNew(UsageEnvironment& env, alsaCapture* capture);

protected:
    alsaPcmFramedSource(UsageEnvironment& env, alsaCapture* capture);
    ~alsaPcmFramedSource();

private:
    void doGetNextFrame() override;

    alsaCapture* fCapture;
    char* fBuffer;
    struct timeval fInitialTime;
    unsigned long long fCurTimestamp;

    // RTP timing constants
    static const unsigned int TIMESTAMP_INCREMENT = 90000/AUDIO_SAMPLE_RATE*NUM_OF_FRAMES_PER_PERIOD;  // (90000/16000)*320 or 90000/50
};

} // namespace alsa_rtsp