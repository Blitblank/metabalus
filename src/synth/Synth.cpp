
#include "Synth.h"

#include <iostream>
#include <cmath>

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

// TODO: you get it, also in a yml config
#define SYNTH_PITCH_STANDARD 440.0f // frequency of home pitch
#define SYNTH_MIDI_HOME 69 // midi note index of home pitch
#define SYNTH_NOTES_PER_OCTAVE 12

Synth::Synth(const ParameterStore& params) : paramStore_(params) {

}

void Synth::updateParams() {
    for(size_t i = 0; i < PARAM_COUNT; i++) {
        params_[i].target = paramStore_.get(static_cast<ParamId>(i));
    }
}

void Synth::setSampleRate(uint32_t sampleRate) { 
    sampleRate_ = sampleRate;
    filter_.setSampleRate(static_cast<float>(sampleRate));
}

inline float Synth::getParam(ParamId id) {
    return params_[static_cast<size_t>(id)].current;
}

inline float Synth::noteToFrequency(uint8_t note) {
    return SYNTH_PITCH_STANDARD * pow(2.0f, static_cast<float>(note - SYNTH_MIDI_HOME) / static_cast<float>(SYNTH_NOTES_PER_OCTAVE));
}

void Synth::handleNoteEvent(const NoteEvent& event) {
    if(event.type == NoteEventType::NoteOn) {
        // add note to activeNotes list
        if (std::find(heldNotes_.begin(), heldNotes_.end(), event.note) == heldNotes_.end()) {
            heldNotes_.push_back(event.note);
            gainEnvelope_.noteOn();
            cutoffEnvelope_.noteOn();
            resonanceEnvelope_.noteOn();
            // TODO: envelopes in an array so we can loop over them
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
        gainEnvelope_.noteOff(); // TODO: move somewhere else when polyphony
        cutoffEnvelope_.noteOff();
        resonanceEnvelope_.noteOff();
        return;
    }

    uint8_t note = heldNotes_.back();
    frequency_ = noteToFrequency(note);
}

void Synth::process(float* out, uint32_t nFrames, uint32_t sampleRate) {
    
    // yeah really only need to update this once per buffer if its ~6ms latency
    updateParams();

    float sampleOut = 0.0f;
    bool triggered = false;

    for (uint32_t i = 0; i < nFrames; i++) {

        // updates internal buffered parameters for smoothing
        for(auto& p : params_) p.update(); // TODO: profile this

        // process all envelopes
        // should be easy enough if all the envelopes are in an array to loop over them
        gainEnvelope_.set(getParam(ParamId::Osc1VolumeEnvA), getParam(ParamId::Osc1VolumeEnvD), getParam(ParamId::Osc1VolumeEnvS), getParam(ParamId::Osc1VolumeEnvR));
        cutoffEnvelope_.set(getParam(ParamId::FilterCutoffEnvA), getParam(ParamId::FilterCutoffEnvD), getParam(ParamId::FilterCutoffEnvS), getParam(ParamId::FilterCutoffEnvR));
        resonanceEnvelope_.set(getParam(ParamId::FilterResonanceEnvA), getParam(ParamId::FilterResonanceEnvD), getParam(ParamId::FilterResonanceEnvS), getParam(ParamId::FilterResonanceEnvR));
        float gainEnv = gainEnvelope_.process();
        float cutoffEnv = cutoffEnvelope_.process();
        float resonanceEnv = resonanceEnvelope_.process();
        // TODO: envelope is shared between all notes so this sequence involves a note change but only one envelope attack:
        // NOTE_A_ON > NOTE_B_ON > NOTE_A_OFF and note B starts playing part-way through note A's envelope 

        // skip if no active notes
        if(!gainEnvelope_.isActive()) {
            out[2*i] = 0.0f;
            out[2*i+1] = 0.0f;
            scope_->push(0.0f);
            continue;
            // TODO: should I have a write() function ? 
            // maybe we change the synth.process into just returning a single float and the write can be in audioEngine
        }
        
        // TODO: make pitchOffset variable for each oscillator (maybe three values like octave, semitone offset, and pitch offset in cents)
        float pitchOffset = 1.0f;
        float phaseInc = pitchOffset * 2.0f * M_PI * frequency_ / static_cast<float>(sampleRate);

        float gain = gainEnv * getParam(ParamId::Osc1VolumeDepth);

        // sample generation
        // TODO: wavetables
        // TODO: wavetables should be scaled by their RMS for equal loudness (prelim standard = 0.707)
        float sineSample = std::sin(phase_);
        float squareSample = (phase_ >= M_PI) ? 0.707f : -0.707f;
        float sawSample = ((phase_ / M_PI) - 1.0f) / 0.577f * 0.707f;
        // switch statement will be replaced with an array index for our array of wavetables
        switch (static_cast<int32_t>(std::round(getParam(ParamId::Osc1WaveSelector1)))) {
        case 0:
            sampleOut = sineSample * gain;
            break;
        case 1:
            sampleOut = squareSample * gain;
            break;
        case 2:
            sampleOut = sawSample * gain;
            break;
        case 3:
            // TODO: no triable wave yet :(
            sampleOut = sineSample * gain;
            break;
        default: // unreachable
            break;
        }

        // filter sample
        float cutoffFreq = cutoffEnv * pow(2.0f, getParam(ParamId::FilterCutoffDepth)) * frequency_;
        filter_.setParams(Filter::Type::BiquadLowpass, cutoffFreq, resonanceEnv * getParam(ParamId::FilterResonanceDepth));
        sampleOut = filter_.biquadProcess(sampleOut);

        // write to buffer
        out[2*i] = sampleOut; // left
        out[2*i+1] = sampleOut; // right

        // write to scope buffer
        if (scope_) {
            scope_->push(sampleOut); // visualization tap
        }

        // sampling business
        phase_ += phaseInc;
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