#include "live555_rtsp_server_manager.h"
#include "alsa_pcm_media_subsession.h"
#include "logger.h"

namespace alsa_rtsp {

Live555RtspServerManager::Live555RtspServerManager(UsageEnvironment* env, alsaCapture* capture, int port)
    : env_(env)
    , capture_(capture)
    , port_(port)
    , rtspServer_(nullptr)
    , sms_(nullptr) {
}

bool Live555RtspServerManager::initialize() {
    rtspServer_ = RTSPServer::createNew(*env_, port_, nullptr);
    if (rtspServer_ == nullptr) {
        logMessage("Failed to create RTSP server: " + std::string(env_->getResultMsg()));
        return false;
    }
    logMessage("Created RTSP server on port " + std::to_string(port_));

    sms_ = ServerMediaSession::createNew(*env_,
        "alsaStream",
        "Audio Stream from alsa capture",
        "Session streamed by alsa RTSP Server",
        True);  // append RTSP URL

    alsaPcmMediaSubsession* audioSubsession = 
        alsaPcmMediaSubsession::createNew(*env_, capture_, True);

    if (audioSubsession == nullptr) {
        logMessage("Failed to create audio subsession");
        return false;
    }

    sms_->addSubsession(audioSubsession);
    rtspServer_->addServerMediaSession(sms_);

    char* url = rtspServer_->rtspURL(sms_);
    logMessage("Stream URL: " + std::string(url));
    delete[] url;

    return true;
}

void Live555RtspServerManager::runEventLoop(char* shouldExit) {
    logMessage("Starting RTSP server event loop");
    env_->taskScheduler().doEventLoop(shouldExit);
}

void Live555RtspServerManager::cleanup() {
    logMessage("Cleaning up RTSP server");
    Medium::close(rtspServer_);
}

} // namespace alsa_rtsp