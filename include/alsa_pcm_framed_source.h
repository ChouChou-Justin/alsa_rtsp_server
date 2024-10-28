#pragma once

#include <liveMedia.hh>
#include "alsa_capture.h"

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
};

} // namespace alsa_rtsp