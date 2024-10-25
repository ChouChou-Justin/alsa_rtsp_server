#include "alsa_pcm_framed_source.h"
#include "logger.h"

namespace alsa_rtsp {

alsaPcmFramedSource* alsaPcmFramedSource::createNew(UsageEnvironment& env, alsaCapture* capture) {
    return new alsaPcmFramedSource(env, capture);
}

alsaPcmFramedSource::alsaPcmFramedSource(UsageEnvironment& env, alsaCapture* capture)
    : FramedSource(env), fCapture(capture) {
        // Use the full buffer size
    fFrameSize = fCapture->getBufferSize();
    // Add safety check
    if (fFrameSize > (1024 * 1024 * 10)) { // 10MB limit
        handleClosure();
        return;
    }
    fBuffer = new char[fFrameSize];
}

alsaPcmFramedSource::~alsaPcmFramedSource() {
    delete[] fBuffer;
}

void alsaPcmFramedSource::doGetNextFrame() {
    if (!isCurrentlyAwaitingData()) return;

    // Calculate optimal packet size (20ms of audio)
    unsigned int samplesPerPacket = (fCapture->getSampleRate() * 20) / 1000;  // 20ms worth of samples
    unsigned int bytesPerPacket = samplesPerPacket * fCapture->getChannels() * (fCapture->getBitDepth() / 8);
    
    // Ensure we don't exceed buffer limits
    if (bytesPerPacket > fMaxSize) {
        bytesPerPacket = fMaxSize;
        samplesPerPacket = bytesPerPacket / (fCapture->getChannels() * (fCapture->getBitDepth() / 8));
    }

    // Read frames
    int frames = fCapture->readFrames(fBuffer, NUM_OF_FRAMES_PER_PERIOD);
    if (frames < 0) {
        handleClosure();
        return;
    }

    // Calculate actual frame size in bytes
    fFrameSize = frames * fCapture->getChannels() * (fCapture->getBitDepth() / 8);
    
    // Get current time
    struct timeval tvNow;
    gettimeofday(&tvNow, NULL);
    fPresentationTime = tvNow;

    // Calculate duration based on frames and sample rate
    fDurationInMicroseconds = (unsigned int)((frames * 1000000) / fCapture->getSampleRate());

    // Handle buffer size limits
    if (fFrameSize > fMaxSize) {
        fNumTruncatedBytes = fFrameSize - fMaxSize;
        fFrameSize = fMaxSize;
    } else {
        fNumTruncatedBytes = 0;
    }

    // Copy data
    memcpy(fTo, fBuffer, fFrameSize);

    // Signal completion    
    FramedSource::afterGetting(this);
    }

} // namespace alsa_rtsp