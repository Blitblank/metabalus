
#pragma once

#include "../ParameterStore.h"
#include "../NoteQueue.h"
#include "Envelope.h"
#include "ScopeBuffer.h"
#include "Filter.h"

#include <vector>
#include <atomic>

struct SmoothedParam {
    float current = 0.0f;
    float target = 0.0f;
    float gain = 0.001f;

    inline void update() { current += gain * (target - current); }
};

class Synth {

public:
    Synth(const ParameterStore& params);
    ~Synth() = default;

    // generates a buffer of audio samples nFrames long
    void process(float* out, uint32_t nFrames, uint32_t sampleRate);

    // handles note events
    void handleNoteEvent(const NoteEvent& event);

    // setters
    void setSampleRate(uint32_t sampleRate);
    void setScopeBuffer(ScopeBuffer* scope) { scope_ = scope; }

private:

    // update internal params to the paramStore. not an immediate sync, its buffered for smoothing 
    void updateParams(); // (might remove the smoothing later)

    // small getter that abstracts all the static casts and such
    inline float getParam(ParamId);

    // for calculating frequency based on midi note id
    inline float noteToFrequency(uint8_t note);

    // finds the active voice
    void updateCurrentNote(); 

    const ParameterStore& paramStore_;
    // smoothed params creates a buffer in case the thread controlling paramStore gets blocked
    std::array<SmoothedParam, PARAM_COUNT> params_;
    uint32_t sampleRate_;
    
    // for the scope
    ScopeBuffer* scope_ = nullptr;

    // TODO: might make this a fixed array where index=midi-note and the value=velocity
    // so non-zero elements are the ones currently being played
    std::vector<uint8_t> heldNotes_;

    // here's where the actual sound generation happens
    // TODO: put this in an oscillator class
    float frequency_ = 220.0f;
    float phase_ = 0.0f;

    // envelopes !!
    Envelope gainEnvelope_;
    Envelope cutoffEnvelope_;
    Envelope resonanceEnvelope_;

    Filter filter1_;
    Filter filter2_;

};
