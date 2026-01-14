
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

void Synth::handleNoteEvent(const NoteEvent& event) {

    lastTime = event.timestamp;

    if(event.type == NoteEventType::NoteOn) {

        // TODO: find quietest voice and assign a note to it instead of just the first inactive one
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

    for (uint32_t i = 0; i < nFrames; i++) {

        // updates internal buffered parameters for smoothing
        for(auto& p : params_) p.update(); // TODO: profile this

        // assemble float array of parameters so that its easier for voices to retrieve
        float params[PARAM_COUNT] = {0.0f};
        for(int i = 0; i < PARAM_COUNT; i++) {
            params[i] = params_[i].current;
        }

        // foreach voice, process...
        float mix = 0.0f;
        for(Voice& v : voices_) {
            mix += v.process(params, triggered);
        }

        sampleOut = mix;

        // write to buffer
        out[2*i] = sampleOut; // left
        out[2*i+1] = sampleOut; // right

        // write to scope buffer
        if (scope_) {
            scope_->push(sampleOut); // visualization tap
        }

        // triggering business
        // TODO: get trigger info from voice (lowest frequency voice)
        float phase_ = 0.0f;
        if (phase_ > 2.0f * M_PI) {
            phase_ -= 2.0f * M_PI;
            if(!triggered) {
                scope_->setTrigger(i); // this is where we consider the start of a waveform
                triggered = true;
                // TODO: investigate triggering accross buffers when a single wave period transcends a single audio buffer
            }
        }
    }

}