
#include "Voice.h"
#include <cmath>
#include <iostream>
#include <random>

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
    gain *= (100.0f - static_cast<float>(note_)) * 0.005f + 0.75;

    // sample generation
    uint8_t osc1Wave = (static_cast<uint8_t>(std::round(getParam(ParamId::Osc1WaveSelector1))));
    for(Oscillator& o : oscillators_) {
        o.setWavetable(osc1Wave);
    }

    // calculate the note/pitch of the oscillators
    bool temp = false;
    uint8_t osc1NoteOffset = static_cast<uint8_t>(std::round((SYNTH_NOTES_PER_OCTAVE) * (getParam(ParamId::Osc1OctaveOffset) + getParam(ParamId::MasterOctaveOffset)) + getParam(ParamId::Osc1SemitoneOffset) + getParam(ParamId::MasterSemitoneOffset)));
    uint8_t osc2NoteOffset = static_cast<uint8_t>(std::round((SYNTH_NOTES_PER_OCTAVE) * (getParam(ParamId::Osc2OctaveOffset) + getParam(ParamId::MasterOctaveOffset)) + getParam(ParamId::Osc2SemitoneOffset) + getParam(ParamId::MasterSemitoneOffset)));
    uint8_t osc3NoteOffset = static_cast<uint8_t>(std::round((SYNTH_NOTES_PER_OCTAVE) * (getParam(ParamId::Osc3OctaveOffset) + getParam(ParamId::MasterOctaveOffset)) + getParam(ParamId::Osc3SemitoneOffset) + getParam(ParamId::MasterSemitoneOffset)));
    // sample oscillators
    float osc1 = oscillators_[0].process(osc1NoteOffset + note_, (getParam(ParamId::Osc1PitchOffset) + getParam(ParamId::MasterPitchOffset))/100.0f, scopeTrigger);
    float osc2 = oscillators_[1].process(osc2NoteOffset + note_, (getParam(ParamId::Osc2PitchOffset) + getParam(ParamId::MasterPitchOffset))/100.0f, temp);
    float osc3 = oscillators_[2].process(osc3NoteOffset + note_, (getParam(ParamId::Osc3PitchOffset) + getParam(ParamId::MasterPitchOffset))/100.0f, temp);

    // TODO: implement controls for noise
    //float scale = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); // Range [0.0, 1.0]
    //float noise = -1.0f + 2.0f * scale;
    // these values didn't sound good so I commented them out before I get controls for them

    // mix oscillators
    // TODO: implement gain controls for the other oscillators
    float sampleOut = (osc1 + osc2*0.4f + osc3*0.15f) * gain; // pre-filtered noise

    // filter sample
    float baseFreq = oscillators_[0].frequency();
    float cutoffFreq = cutoffEnv * pow(2.0f, getParam(ParamId::FilterCutoffDepth)) * baseFreq * velocityGain;
    float resonance = resonanceEnv * getParam(ParamId::FilterResonanceDepth) * velocityGain;
    filter1_.setParams(Filter::Type::BiquadLowpass, cutoffFreq, resonance);
    filter2_.setParams(Filter::Type::BiquadLowpass, cutoffFreq, resonance);
    float filteredSample = filter1_.biquadProcess(sampleOut);
    // sampleOut = filter2_.biquadProcess(sampleOut); // TODO: for some reason second filter is unstable only on windows 🤷

    //filteredSample += noise*0.125f; // post-filtered noise

    return filteredSample;
}