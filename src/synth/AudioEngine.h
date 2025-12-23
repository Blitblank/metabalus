
#pragma once

#include <RtAudio.h>
#include <stdint.h>
#include <atomic>

class AudioEngine {

public:
    AudioEngine();
    ~AudioEngine();

    bool start();
    void stop();

    void setFrequency(float freq);

private: 
    static int32_t audioCallback(void* outputBuffer, void* inputBuffer, uint32_t nFrames, double streamTime, RtAudioStreamStatus status, void* userData);
    int32_t process(float* out, uint32_t nFrames);

    RtAudio audio_;
    uint32_t sampleRate_ = 44100;
    uint32_t bufferFrames_ = 256;

    std::atomic<float> targetFreq_{ 400.0f };
    float currentFreq_ = 440.0f;
    float phase_ = 0.0f;

};
