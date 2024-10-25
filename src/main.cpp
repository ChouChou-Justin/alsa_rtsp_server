#include <csignal>
#include <chrono>
#include <string>
#include "alsa_capture.h"
#include "live555_rtsp_server_manager.h"
#include "logger.h"

using namespace alsa_rtsp;

char shouldExit = 0;
std::chrono::steady_clock::time_point streamStartTime;

void signalHandler(int signum) {
    logMessage("Interrupt signal (" + std::to_string(signum) + ") received");
    shouldExit = 1;
}

int main(int argc, char* argv[]) {
    // Set up signal handling
    std::signal(SIGINT, signalHandler);
    streamStartTime = std::chrono::steady_clock::now();

    // Create Live555 environment
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

    // Initialize ALSA capture
    alsaCapture* audioCapture = new alsaCapture(AUDIO_DEVICE, AUDIO_SAMPLE_RATE, AUDIO_CHANNELS, AUDIO_BIT_DEPTH);
    
    if (!audioCapture->initialize()) {
        *env << "Failed to initialize ALSA capture\n";
        exit(1);
    }

    if (!audioCapture->startCapture()) {
        *env << "Failed to start ALSA capture\n";
        exit(1);
    }
    logMessage("Successfully initialized and started ALSA capture.");

    // Initialize RTSP server
    Live555RtspServerManager rtspManager(env, audioCapture);
    if (!rtspManager.initialize()) {
        *env << "Failed to initialize RTSP server manager\n";
        exit(1);
    }

    // Run the event loop
    rtspManager.runEventLoop(&shouldExit);

    // Cleanup
    rtspManager.cleanup();
    delete audioCapture;
    delete scheduler;
    env->reclaim();

    logMessage("Application shutdown complete");
    return 0;
}
