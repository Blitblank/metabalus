
#include "Synth.h"

#include <iostream>
#include <cmath>

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

Synth::Synth(const ParameterStore& params) : paramStore_(params) {

}

void Synth::updateParams() {
    for(size_t i = 0; i < PARAM_COUNT; i++) {
        params_[i].target = paramStore_.get(static_cast<ParamId>(i));
    }
}

inline float Synth::getParam(ParamId id) {
    return params_[static_cast<size_t>(id)].current;
}

void Synth::process(float* out, uint32_t nFrames, uint32_t sampleRate) {
    
    // yeah really only need to update this once per buffer if its ~6ms latency
    updateParams();

    for (uint32_t i = 0; i < nFrames; i++) {

        for(auto& p : params_) p.update(); // TODO: profile this

        // based on oscillator frequency
        float frequency = getParam(ParamId::Osc1Frequency); // this will come from a midi controller
        float phaseInc = 2.0f * M_PI * frequency / static_cast<float>(sampleRate);

        // sample generation
        float gain = getParam(ParamId::Osc1Gain);
        float sample = std::sin(phase_) * gain;

        // write to buffer
        out[2*i] = sample; // left
        out[2*i+1] = sample; // right

        // sampling business
        phase_ += phaseInc;
        if (phase_ > 2.0f * M_PI) phase_ -= 2.0f * M_PI;
    }

}