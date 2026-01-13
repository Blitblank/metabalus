
#pragma once

#include "Oscillator.h"
#include "Envelope.h"
#include "Filter.h"
#include "../ParameterStore.h"

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

// TODO: you get it, also in a yml config
#define SYNTH_PITCH_STANDARD 440.0f // frequency of home pitch
#define SYNTH_MIDI_HOME 69 // midi note index of home pitch
#define SYNTH_NOTES_PER_OCTAVE 12

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

private:

    float sampleRate_ = 44100.0f;

    inline float noteToFrequency(uint8_t note);
    inline float getParam(ParamId id);

    uint8_t note_ = 0;
    float velocity_ = 1.0f;
    bool active_ = false;

    // here's where the actual sound generation happens
    // TODO: put this in an oscillator class
    float frequency_ = 220.0f;
    float phase_ = 0.0f;
    //Oscillator osc_; // example

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

};
