
#pragma once

#include <RtAudio.h>
#include <stdint.h>
#include <atomic>

#include "Synth.h"
#include "../KeyboardController.h"

#if defined(_WIN32)
    #define AUDIO_API RtAudio::WINDOWS_WASAPI
#else
    #define AUDIO_API RtAudio::LINUX_ALSA
#endif

class AudioEngine {

public:
    AudioEngine();
    ~AudioEngine();

    // starts the audio stream. returns true on success and false on failure
    bool start();

    // stops the audio stream.
    void stop();

    // getters
    ParameterStore* parameters() { return &params_; }
    NoteQueue& noteQueue() { return noteQueue_; }
    ScopeBuffer& scopeBuffer() { return scope_; }

private:

    // RtAudio binding for passing samples
    static int32_t audioCallback(void* outputBuffer, void* inputBuffer, uint32_t nFrames, double streamTime, RtAudioStreamStatus status, void* userData);

    // calls the synth.process to generate a buffer of audio samples
    int32_t process(float* out, uint32_t nFrames);

    ParameterStore params_; // stores the control parameters
    NoteQueue noteQueue_; // stores note events for passing between threads
    Synth synth_; // generates audio
    ScopeBuffer scope_ { 1024 }; // stores audio samples for visualization

    RtAudio audio_{AUDIO_API}; // audio device
    // TODO: id like a yml config file or something for these
    uint32_t sampleRate_ = 44100;
    uint32_t bufferFrames_ = 512; // time per buffer = BF/SR (256/44100 = 5.8ms)
    uint32_t channels_ = 2; // stereo

};
