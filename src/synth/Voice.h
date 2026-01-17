
#pragma once

#include "Oscillator.h"
#include "Envelope.h"
#include "Filter.h"
#include "../ParameterStore.h"

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

// TODO: make configurable
#define SYNTH_OSCILLATOR_COUNT 3

struct SmoothedParam {
    float current = 0.0f;
    float target = 0.0f;
    float gain = 0.001f;

    inline void update() { current += gain * (target - current); }
};

class Voice {
public:

    Voice() = default;
    Voice(SmoothedParam* params);
    ~Voice() = default;

    void setSampleRate(float sampleRate);

    void noteOn(int midiNote, float velocity);
    void noteOff();

    bool isActive();

    float process(float* params, bool& scopeTrigger);

    uint8_t note() { return note_; }
    float frequency() { return oscillators_[0].frequency(); }

private:

    float sampleRate_ = 44100.0f;

    inline float noteToFrequency(uint8_t note);
    inline float getParam(ParamId id);

    uint8_t note_ = 0;
    float velocity_ = 1.0f;
    bool active_ = false;
    float phase_ = 0.0f;

    std::array<Oscillator, 3> oscillators_;

    // envelopes !!
    // TODO: foreach envelope in vector<Envelope> envelopes_
    Envelope gainEnvelope_;
    Envelope cutoffEnvelope_;
    Envelope resonanceEnvelope_;

    // filters
    Filter filter1_;
    Filter filter2_;

    // paramstore pointer
    SmoothedParam* params_;

    // TODO: add a parameter in the paramstore for this
    float velocitySensitivity = 0.7f;
    float velocityCenter = 2.0f;

};
