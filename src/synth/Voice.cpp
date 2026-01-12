
#include "Voice.h"
#include <cmath>

Voice::Voice(std::array<SmoothedParam, PARAM_COUNT>* params) : params_(params) {

}

void Voice::setSampleRate(float sampleRate) {
    sampleRate_ = sampleRate; 

    // foreach envelope...
    gainEnvelope_.setSampleRate(sampleRate);
    cutoffEnvelope_.setSampleRate(sampleRate);
    resonanceEnvelope_.setSampleRate(sampleRate);

    // foreach filter...
    filter1_.setSampleRate(sampleRate);
    filter2_.setSampleRate(sampleRate);

    // then foreach oscillator
    //osc1_.setSampleRate(sampleRate);
}

inline float Voice::noteToFrequency(uint8_t note) {
    return SYNTH_PITCH_STANDARD * pow(2.0f, static_cast<float>(note - SYNTH_MIDI_HOME) / static_cast<float>(SYNTH_NOTES_PER_OCTAVE));
}

void Voice::noteOn(int midiNote, float velocity) {
    note_ = midiNote;
    velocity_ = velocity;
    frequency_ = noteToFrequency(midiNote);
    active_ = true;

    // TODO: for each envelope ...
    gainEnvelope_.noteOn();
    cutoffEnvelope_.noteOn();
    resonanceEnvelope_.noteOn();
}

void Voice::noteOff() {
    // again, foreach
    gainEnvelope_.noteOff();
    cutoffEnvelope_.noteOff();
    resonanceEnvelope_.noteOff();
}

bool Voice::isActive() {
    return active_;
}

float Voice::process(float* params, bool& scopeTrigger) {

    // process all envelopes
    // should be easy enough if all the envelopes are in an array to loop over them
    gainEnvelope_.set(getParam(ParamId::Osc1VolumeEnvA), getParam(ParamId::Osc1VolumeEnvD), getParam(ParamId::Osc1VolumeEnvS), getParam(ParamId::Osc1VolumeEnvR));
    cutoffEnvelope_.set(getParam(ParamId::FilterCutoffEnvA), getParam(ParamId::FilterCutoffEnvD), getParam(ParamId::FilterCutoffEnvS), getParam(ParamId::FilterCutoffEnvR));
    resonanceEnvelope_.set(getParam(ParamId::FilterResonanceEnvA), getParam(ParamId::FilterResonanceEnvD), getParam(ParamId::FilterResonanceEnvS), getParam(ParamId::FilterResonanceEnvR));

    // skip if no active notes
    if(!gainEnvelope_.isActive()) {
        
        return 0.0f;
    }
    
    float gainEnv = gainEnvelope_.process();
    float cutoffEnv = cutoffEnvelope_.process();
    float resonanceEnv = resonanceEnvelope_.process();
    // TODO: envelope is shared between all notes so this sequence involves a note change but only one envelope attack:
    // NOTE_A_ON > NOTE_B_ON > NOTE_A_OFF and note B starts playing part-way through note A's envelope 

    // TODO: make pitchOffset variable for each oscillator (maybe three values like octave, semitone offset, and pitch offset in cents)
    float pitchOffset = 1.0f;
    float phaseInc = pitchOffset * 2.0f * M_PI * frequency_ / static_cast<float>(sampleRate);

    float gain = gainEnv * getParam(ParamId::Osc1VolumeDepth);
    float sampleOut = 0.0f;

    // sample generation
    // TODO: move this into the oscillator class
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
    filter1_.setParams(Filter::Type::BiquadLowpass, cutoffFreq, resonanceEnv * getParam(ParamId::FilterResonanceDepth));
    filter2_.setParams(Filter::Type::BiquadLowpass, cutoffFreq, resonanceEnv * getParam(ParamId::FilterResonanceDepth));
    sampleOut = filter1_.biquadProcess(sampleOut);
    sampleOut = filter2_.biquadProcess(sampleOut);

    return sampleOut;
}