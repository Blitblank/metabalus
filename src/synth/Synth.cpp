
#include "Synth.h"

#include <iostream>
#include <cmath>

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

#define SYNTH_PITCH_STANDARD 432.0f // frequency of home pitch
#define SYNTH_MIDI_HOME 69 // midi note index of home pitch
#define SYNTH_NOTES_PER_OCTAVE 12

Synth::Synth(const ParameterStore& params) : paramStore_(params) {

}

void Synth::updateParams() {
    for(size_t i = 0; i < PARAM_COUNT; i++) {
        params_[i].target = paramStore_.get(static_cast<ParamId>(i));
    }
}

inline float Synth::getParam(ParamId id) {
    return params_[static_cast<size_t>(id)].current;
}

inline float Synth::noteToFrequency(uint8_t note) {
    return SYNTH_PITCH_STANDARD * pow(2.0f, static_cast<float>(note - SYNTH_MIDI_HOME) / static_cast<float>(SYNTH_NOTES_PER_OCTAVE));
}

// TODO: stop popping on note-offs
void Synth::handleNoteEvent(const NoteEvent& event) {
    if(event.type == NoteEventType::NoteOn) {
        // add note to activeNotes list
        if (std::find(heldNotes_.begin(), heldNotes_.end(), event.note) == heldNotes_.end()) {
            heldNotes_.push_back(event.note);
        }
    } else {
        // remove note from activeNotes list
        auto it = std::find(heldNotes_.begin(), heldNotes_.end(), event.note);
        if (it != heldNotes_.end()) {
            heldNotes_.erase(it);
        }
    }

    updateCurrentNote();
}

void Synth::updateCurrentNote() {
    if(heldNotes_.empty()) {
        noteActive_ = false;
        return;
    }

    uint8_t note = heldNotes_.back();
    frequency_ = noteToFrequency(note);
    noteActive_ = true;
}

void Synth::process(float* out, uint32_t nFrames, uint32_t sampleRate) {
    
    // yeah really only need to update this once per buffer if its ~6ms latency
    updateParams();

    float sampleOut = 0.0f;

    for (uint32_t i = 0; i < nFrames; i++) {

        // updates internal buffered parameters for smoothing
        for(auto& p : params_) p.update(); // TODO: profile this

        // skip if no note is being played
        // TODO: this will be handled by an idle envelope eventually
        // could also say gain = 0.0f; but w/e, this saves computing
        if(!noteActive_) {
            out[2*i] = 0.0f;
            out[2*i+1] = 0.0f;
            continue;
        }
        
        float phaseInc = 2.0f * M_PI * frequency_ / static_cast<float>(sampleRate);

        // sample generation
        float gain = getParam(ParamId::Osc1Gain);
        sampleOut = std::sin(phase_) * gain;

        // write to buffer
        out[2*i] = sampleOut; // left
        out[2*i+1] = sampleOut; // right

        // sampling business
        phase_ += phaseInc;
        if (phase_ > 2.0f * M_PI) phase_ -= 2.0f * M_PI;
    }

}