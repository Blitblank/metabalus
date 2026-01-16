
#include "Synth.h"

#include <iostream>
#include <cmath>

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

Synth::Synth(const ParameterStore& params) : paramStore_(params) {
    voices_.fill(Voice(params_.data()));
}

void Synth::updateParams() {
    for(size_t i = 0; i < PARAM_COUNT; i++) {
        params_[i].target = paramStore_.get(static_cast<ParamId>(i));
    }
}

void Synth::setSampleRate(uint32_t sampleRate) { 
    sampleRate_ = sampleRate;

    for(Voice& v : voices_) {
        v.setSampleRate(static_cast<float>(sampleRate));
    }
}

inline float Synth::getParam(ParamId id) {
    return params_[static_cast<size_t>(id)].current;
}

// called by audio engine before process (because it has the reference to the noteEvent store)
// this function consumes the noteEvents in the noteEvent store (whether produced by MIDI or keyboard)
void Synth::handleNoteEvent(const NoteEvent& event) {

    lastTime = event.timestamp;

    if(event.type == NoteEventType::NoteOn) {

        // TODO: this should get fixed if we tie voices to midi notes
        // end all voices already playing this note
        for(Voice& v : voices_) {
            if(v.isActive() && v.note() == event.note) {
                v.noteOff();
                break;
            }
        }

        // find inactive voice and start it with the given note
        for(Voice& v : voices_) {
            if(!v.isActive()) {
                v.noteOn(event.note, event.velocity);
                break;
            }
        }

    } else {

        // find voice associated with note event and end it
        for(Voice& v : voices_) {
            if(v.isActive() && v.note() == event.note) {
                v.noteOff();
                break;
            }
        }

    }

}

void Synth::process(float* out, uint32_t nFrames, uint32_t sampleRate) {
    
    // yeah really only need to update this once per buffer if its ~6ms latency
    updateParams();

    float sampleOut = 0.0f;
    bool triggered = false;
    bool once = false;

    // some things need to be done only once per buffer instead of once per sample
    
    // find the lowest active voice for scope triggering
    int lowestVoice = 0;
    float lowestFreq = 100000.0f;
    for(int i = 0; i < voices_.size(); i++) {
        if(!voices_[i].isActive()) continue;
        float currentFreq = voices_[i].frequency();
        if(currentFreq < lowestFreq) {
            lowestVoice = i;
            lowestFreq = currentFreq;
        }
    }

    for (uint32_t i = 0; i < nFrames; i++) {

        // updates internal buffered parameters for smoothing
        for(auto& p : params_) p.update(); // TODO: profile this

        // assemble float array of parameters so that its easier for voices to retrieve
        float params[PARAM_COUNT] = {0.0f};
        for(int i = 0; i < PARAM_COUNT; i++) {
            params[i] = params_[i].current;
        } // maybe take this outside the loop if performance is an issue

        // foreach voice, process...
        float mix = 0.0f;
        for(int i = 0; i < voices_.size(); i++) {
            bool temp = false;
            mix += voices_[i].process(params, temp);
            if(i == lowestVoice) triggered = temp;
        }
        mix /= 4.0f; // for number of voices to prevent clipping
        mix = tanh(mix); // really prevents clipping
        // TODO: these saturation function work kinda like magic, use them elsewhere

        sampleOut = mix;

        // write to buffer
        out[2*i] = sampleOut; // left
        out[2*i+1] = sampleOut; // right

        // write to scope buffer
        if (scope_) {
            scope_->push(sampleOut); // visualization tap
        }

        // triggering business
        if(triggered && (!once)) {
            scope_->setTrigger(i); // this is where we consider the start of a waveform
            once = true;
            // TODO: investigate triggering accross buffers when a single wave period transcends a single audio buffer
        }
    }

}