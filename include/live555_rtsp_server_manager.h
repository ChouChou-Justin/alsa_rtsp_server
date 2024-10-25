#pragma once

// Include Live555 headers
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

// Include local headers
#include "alsa_capture.h"

// Forward declaration for v4l2_capture if needed
class V4L2Capture;

namespace alsa_rtsp {

class Live555RtspServerManager {
public:
    Live555RtspServerManager(UsageEnvironment* env, alsaCapture* capture, int port = 8554);
    
    bool initialize();
    void runEventLoop(char* shouldExit);
    void cleanup();

private:
    UsageEnvironment* env_;
    alsaCapture* capture_;
    int port_;
    RTSPServer* rtspServer_;
    ServerMediaSession* sms_;
    V4L2Capture* v4l2Capture_; // Add this if you need V4L2 capture functionality
};

} // namespace alsa_rtsp
