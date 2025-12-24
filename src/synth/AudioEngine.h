
#pragma once

#include <RtAudio.h>
#include <stdint.h>
#include <atomic>

#include "Synth.h"

class AudioEngine {

public:
    AudioEngine();
    ~AudioEngine();

    bool start();
    void stop();
    ParameterStore* parameters() { return &params_; }

private: 
    static int32_t audioCallback(void* outputBuffer, void* inputBuffer, uint32_t nFrames, double streamTime, RtAudioStreamStatus status, void* userData);
    int32_t process(float* out, uint32_t nFrames);

    ParameterStore params_;
    Synth synth_;

    // TODO: id like a yml config file or something for these
    RtAudio audio_;
    uint32_t sampleRate_ = 44100;
    uint32_t bufferFrames_ = 256; // time per buffer = BF/SR (256/44100 = 5.8ms)
    uint32_t channels_ = 2; // stereo

};
