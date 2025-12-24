
#pragma once

#include <RtAudio.h>
#include <stdint.h>
#include <atomic>

#include "Synth.h"

class AudioEngine {

public:
    AudioEngine();
    ~AudioEngine();

    // starts the audio stream. returns true on success and false on failure
    bool start();

    // stops the audio stream.
    void stop();

    // params getter
    ParameterStore* parameters() { return &params_; }

private:

    // RtAudio binding for passing samples
    static int32_t audioCallback(void* outputBuffer, void* inputBuffer, uint32_t nFrames, double streamTime, RtAudioStreamStatus status, void* userData);

    // calls the synth.process to generate a buffer audio samples
    int32_t process(float* out, uint32_t nFrames);

    ParameterStore params_;
    Synth synth_;

    // TODO: id like a yml config file or something for these
    RtAudio audio_;
    uint32_t sampleRate_ = 44100;
    uint32_t bufferFrames_ = 256; // time per buffer = BF/SR (256/44100 = 5.8ms)
    uint32_t channels_ = 2; // stereo

};
