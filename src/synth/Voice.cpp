
#include "Voice.h"
#include <cmath>
#include <iostream>

Voice::Voice(SmoothedParam* params, WavetableController* wavetable) : params_(params), wavetable_(wavetable) {

    oscillators_.fill(Oscillator(wavetable_));
}

// cascade sample rate to all descendant objects
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
    for(Oscillator& o : oscillators_) {
        o.setSampleRate(sampleRate);
    }
}

inline float Voice::getParam(ParamId id) {
    return params_[static_cast<size_t>(id)].current;
}

void Voice::noteOn(int midiNote, float velocity) {
    note_ = midiNote;
    velocity_ = velocity;
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
    active_ = false;
}

bool Voice::isActive() {
    return active_;
}

// generates a single sample, called from synth.process()
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
    
    // process all envelopes
    float gainEnv = gainEnvelope_.process();
    float cutoffEnv = cutoffEnvelope_.process();
    float resonanceEnv = resonanceEnvelope_.process();

    // calculate the change that the velocity will make
    // TODO: make velocity parameters configurable, probably also for filterCutoff and filterResonance
    float velocityGain = std::lerp(velocityCenter, velocity_, velocitySensitivity);

    float gain = gainEnv * getParam(ParamId::Osc1VolumeDepth) * velocityGain;

    // sample generation
    uint8_t osc1Wave = (static_cast<uint8_t>(std::round(getParam(ParamId::Osc1WaveSelector1))));
    for(Oscillator& o : oscillators_) {
        o.setWavetable(osc1Wave);
    }

    // TODO: oscillator pitch offset
    bool temp = false;
    float osc1 = oscillators_[0].process(note_, scopeTrigger);
    float osc2 = oscillators_[1].process(static_cast<uint8_t>(note_+12), temp);
    float osc3 = oscillators_[2].process(static_cast<uint8_t>(note_+19), temp);
    
    float sampleOut = (osc1 + osc2*0.5f + osc3*0.25f) * gain;

    // filter sample
    float baseFreq = oscillators_[0].frequency();
    float cutoffFreq = cutoffEnv * pow(2.0f, getParam(ParamId::FilterCutoffDepth)) * baseFreq * velocityGain;
    float resonance = resonanceEnv * getParam(ParamId::FilterResonanceDepth) * velocityGain;
    filter1_.setParams(Filter::Type::BiquadLowpass, cutoffFreq, resonance);
    filter2_.setParams(Filter::Type::BiquadLowpass, cutoffFreq, resonance);
    sampleOut = filter1_.biquadProcess(sampleOut);
    sampleOut = filter2_.biquadProcess(sampleOut);

    return sampleOut;
}