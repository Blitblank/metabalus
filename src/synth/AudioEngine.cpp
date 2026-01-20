
#include "AudioEngine.h"

#include <iostream>

AudioEngine::AudioEngine() : synth_(params_) {
    if(audio_.getDeviceCount() < 1) {
        throw std::runtime_error("No audio devices found");
    }

    // TODO: get audio configurations
    synth_.setSampleRate(sampleRate_);

    synth_.setScopeBuffer(&scope_);

}

AudioEngine::~AudioEngine() {
    stop();
}

bool AudioEngine::start() {

    // initialize the audio engine
    RtAudio::StreamParameters params;
    params.deviceId = audio_.getDefaultOutputDevice();
    params.nChannels = channels_; // we're doing two duplicate channels for pseudo-mono
    params.firstChannel = 0;

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_MINIMIZE_LATENCY;

    // TODO: error check this please
    audio_.openStream(&params, nullptr, RTAUDIO_FLOAT32, sampleRate_, &bufferFrames_, &AudioEngine::audioCallback, this, &options);
    audio_.startStream();

    // sanity check
    std::cout << "sample rate: " << sampleRate_ << "  buffer frames: " << bufferFrames_ << std::endl;
    return true;

}

void AudioEngine::stop() {

    if(audio_.isStreamRunning()) audio_.stopStream();
    if(audio_.isStreamOpen()) audio_.closeStream();
}

// called by RtAudio continuously, sends outputBuffer to audio drivers
int32_t AudioEngine::audioCallback(void* outputBuffer, void*, uint32_t nFrames, double, RtAudioStreamStatus status, void* userData) {
    
    // error if process is too slow for the callback. If this is consistent, then need to optimize synth.process() or whatever cascades from it
    if (status) std::cerr << "Stream underflow" << std::endl;

    // populate audio buffer
    return static_cast<AudioEngine*>(userData)->process(static_cast<float*>(outputBuffer), nFrames);
}

int32_t AudioEngine::process(float* out, uint32_t nFrames) {

    // pass note handling to synth
    NoteEvent event;
    while(noteQueue_.pop(event)) {
        synth_.handleNoteEvent(event);
    }

    // pass sample generation to synth
    synth_.process(out, nFrames, sampleRate_);

    return 0;
}
