
#include "Oscillator.h"

void Oscillator::setWavetable(uint8_t waveTableId) {
    activeWavetable_ = waveTableId;
}

void Oscillator::setSampleRate(float sampleRate) {
    sampleRate_ = sampleRate;
}

float Oscillator::frequency() {
    return frequency_;
}

float Oscillator::process(uint8_t note, bool& scopeTrigger) {
    frequency_ = noteToFrequency(note);
    return process(frequency_, scopeTrigger);
}

float Oscillator::process(float frequency, bool& scopeTrigger) {

    float sampleOut = 0.0f;
    float pitchOffset = 0.5f;
    float phaseInc = pitchOffset * 2.0f * M_PI * frequency / sampleRate_;

    switch (activeWavetable_) {
    case 0: // sine
        sampleOut = std::sin(phase_) / 0.707f;
        break;
    case 1: // square
        sampleOut = (phase_ >= M_PI) ? 1.0f : -1.0f;
        break;
    case 2: // saw
        sampleOut = ((phase_ / M_PI) - 1.0f) / 0.577f;
        break;
    case 3: // triangle
        if(phase_ <= M_PI/2.0f) {
            sampleOut = phase_ * 2.0f/M_PI;
        } else if(phase_ <= 3.0f*M_PI/2.0f) {
            sampleOut = phase_ * -2.0f/M_PI + 2.0f;
        } else {
            sampleOut = phase_ * 2.0f/M_PI - 4.0f;
        }
        sampleOut /= 0.577f; 
        break;
    default: // unreachable
        break;
    }

    phase_ += phaseInc;
    if (phase_ > 2.0f * M_PI) {
        phase_ -= 2.0f * M_PI;
        scopeTrigger = true;
    }

    return sampleOut;
}


inline float Oscillator::noteToFrequency(uint8_t note) {
    return SYNTH_PITCH_STANDARD * pow(2.0f, static_cast<float>(note - SYNTH_MIDI_HOME) / static_cast<float>(SYNTH_NOTES_PER_OCTAVE));
}
