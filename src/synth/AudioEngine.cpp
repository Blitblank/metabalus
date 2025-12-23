
#include "AudioEngine.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

AudioEngine::AudioEngine() {
    if(audio_.getDeviceCount() < 1) {
        throw std::runtime_error("No audio devices found");
    }
}

AudioEngine::~AudioEngine() {
    stop();
}

bool AudioEngine::start() {

    RtAudio::StreamParameters params;
    params.deviceId = audio_.getDefaultOutputDevice();
    params.nChannels = 2;
    params.firstChannel = 0;

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_MINIMIZE_LATENCY;

    /*
    try {
        audio_.openStream(&params, nullptr, RTAUDIO_FLOAT32, sampleRate_, &bufferFrames_, &AudioEngine::audioCallback, this, &options);
        audio_.startStream();
    } catch(RtAudioError& e) {
        std::cerr << e.getMessage() << std::endl;
        return false;
    }
    */
    audio_.openStream(&params, nullptr, RTAUDIO_FLOAT32, sampleRate_, &bufferFrames_, &AudioEngine::audioCallback, this, &options);
    audio_.startStream();

    std::cout << "sample rate: " << sampleRate_ << "  buffer frames: " << bufferFrames_ << std::endl;
    return true;

}

void AudioEngine::stop() {

    if(audio_.isStreamRunning()) audio_.stopStream();
    if(audio_.isStreamOpen()) audio_.closeStream();
}

void AudioEngine::setFrequency(float freq) {
    targetFreq_.store(freq, std::memory_order_relaxed);
}

int32_t AudioEngine::audioCallback( void* outputBuffer, void*, uint32_t nFrames, double, RtAudioStreamStatus status, void* userData) {
    
    if (status) std::cerr << "Stream underflow!" << std::endl;

    return static_cast<AudioEngine*>(userData)->process(static_cast<float*>(outputBuffer), nFrames);
}

int32_t AudioEngine::process(float* out, uint32_t nFrames) {
    const float sr = static_cast<float>(sampleRate_);
    float target = targetFreq_.load(std::memory_order_relaxed);
    float freqStep = (target - currentFreq_) / nFrames;

    for (uint32_t i = 0; i < nFrames; ++i) {
        currentFreq_ += freqStep;

        float phaseInc = 2.0f * M_PI * currentFreq_ / sr;

        out[2*i] = std::sin(phase_); // left
        out[2*i+1] = std::sin(phase_); // right

        phase_ += phaseInc;
        if (phase_ > 2.0f * M_PI) phase_ -= 2.0f * M_PI;
    }

    return 0;
}
