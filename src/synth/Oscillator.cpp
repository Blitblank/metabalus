
#include "Oscillator.h"

Oscillator::Oscillator(WavetableController* wavetable) : wavetable_(wavetable) {

}

void Oscillator::setWavetable(uint8_t waveTableId) {
    activeWavetable_ = waveTableId;
}

void Oscillator::setSampleRate(float sampleRate) {
    sampleRate_ = sampleRate;
}

float Oscillator::frequency() {
    return frequency_;
}

float Oscillator::process(uint8_t note, float detune, bool& scopeTrigger) {
    frequency_ = noteToFrequency(note, detune);
    return process(frequency_, scopeTrigger);
}

float Oscillator::process(float frequency, bool& scopeTrigger) {

    float pitchOffset = 0.5f;
    float phaseInc = pitchOffset * 2.0f * M_PI * frequency / sampleRate_;

    float sampleOut = wavetable_->sample(activeWavetable_, phase_);

    phase_ += phaseInc;
    if (phase_ > 2.0f * M_PI) {
        phase_ -= 2.0f * M_PI;
        scopeTrigger = true;
    }

    return sampleOut;
}

inline float Oscillator::noteToFrequency(uint8_t note, float detune) {
    return SYNTH_PITCH_STANDARD * pow(2.0f, (static_cast<float>(note - SYNTH_MIDI_HOME) + detune) / static_cast<float>(SYNTH_NOTES_PER_OCTAVE));
}
