
#pragma once

#include "WavetableController.h"

#include <cstdint>
#include <cmath>
#include <array>

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

#define SYNTH_TWELFTH_ROOT_TWO 1.05946309435929526463

// TODO: you get it, also in a yml config
#define SYNTH_PITCH_STANDARD 440.0f // frequency of home pitch
#define SYNTH_MIDI_HOME 69 // midi note index of home pitch
#define SYNTH_NOTES_PER_OCTAVE 12

class Oscillator {
public:

    Oscillator() = default;
    Oscillator(WavetableController* wavetable);
    ~Oscillator() = default;

    void setWavetable(uint8_t waveTableId);
    void setSampleRate(float sampleRate);
    float frequency();

    float process(uint8_t note, float detune, bool& scopeTrigger); // detune (-1, 1)
    float process(float frequency, bool& scopeTrigger);

private:

    float sampleRate_ = 44100.0f;

    inline float noteToFrequency(uint8_t note, float detune);

    // internal state tracking
    float phase_ = 0.0f;
    uint8_t activeWavetable_;
    float frequency_ = 220.0f;

    // wavetables
    WavetableController* wavetable_;

};
