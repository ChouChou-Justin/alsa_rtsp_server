#pragma once

namespace alsa_rtsp {

// Audio device parameters
constexpr const char* AUDIO_DEVICE = "plughw:2,0";
constexpr unsigned int AUDIO_SAMPLE_RATE = 16000;
constexpr unsigned int AUDIO_CHANNELS = 1;
constexpr unsigned int AUDIO_BIT_DEPTH = 16;

// Buffer management
constexpr unsigned int NUM_OF_PERIODS_IN_BUFFER = 64;  // Number of periods in buffer
constexpr unsigned int NUM_OF_FRAMES_PER_PERIOD = 320; // Number of frames in one period

// Derived calculations for documentation

// Bitrate = AUDIO_SAMPLE_RATE × AUDIO_CHANNELS × AUDIO_BIT_DEPTH = 16000 × 1 × 16 = 256,000 bits/second (256 kbps)

// Buffer size (bytes) = NUM_OF_FRAMES_PER_PERIOD × AUDIO_CHANNELS × (AUDIO_BIT_DEPTH/8) = 320 × 1 × (16/8) = 640 bytes
// Buffer size (frames) = NUM_OF_FRAMES_PER_PERIOD × NUM_OF_PERIODS_IN_BUFFER = 320 × 64 = 20480 frames
// [Total Buffer]: (640 bytes)
// [Period 1][Period 2][Period 3]...[Period 64]            NUM_OF_PERIODS_IN_BUFFER determines how many periods fit in the total buffer
// [320 frames][320 frames][320 frames]...[320 frames] Each period is NUM_OF_FRAMES_PER_PERIOD frames long
// [2 bytes][2 bytes][2 bytes]...[2 bytes]                 Frame size (in bytes) = AUDIO_CHANNELS × (AUDIO_BIT_DEPTH/8) = 1 * (16/8) = 2 bytes
// Larger NUM_OF_PERIODS_IN_BUFFER = more protection against XRUNs but higher latency
// Smaller NUM_OF_PERIODS_IN_BUFFER = lower latency but higher risk of XRUNs

// Frame duration = 1 / AUDIO_SAMPLE_RATE = 1/16000
// Desired latency = NUM_OF_FRAMES_PER_PERIOD / AUDIO_SAMPLE_RATE = 320/16000 = 0.02 seconds (20ms)

} // namespace alsa_rtsp
