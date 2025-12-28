
#include "Filter.h"

#include <cmath>
#include <iostream>

#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

void Filter::setSampleRate(float sampleRate) {
    sampleRate_ = sampleRate;
    calculateCoefficients();
}

void Filter::setParams(Type type, float frequency, float q) {
    type_ = type;
    frequency_ = std::min(frequency, sampleRate_ / 2.0f * 0.999f);
    q_ = q;
    calculateCoefficients();
}

float Filter::biquadProcess(float in) {

    // calculate filtered sample
    float out = b0_ * in + z1_;
    
    // update states
    z1_ = b1_ * in - a1_ * out + z2_;
    z2_ = b2_ * in - a2_ * out;

    return out;
}

void Filter::calculateCoefficients() {

    if(q_ < 0.001f) q_ = 0.001f;

    float omega = 2.0f * M_PI * frequency_ / sampleRate_;
    float sinOmega = std::sin(omega);
    float cosOmega = std::cos(omega);
    float alpha = sinOmega / (2.0f * q_);

    float b0, b1, b2, a0, a1, a2;
    
    switch (type_) {
      case Type::BiquadLowpass:
        b0 =  (1.0f - cosOmega) * 0.5f;
        b1 =   1.0f - cosOmega;
        b2 =  (1.0f - cosOmega) * 0.5f;
        a0 =   1.0f + alpha;
        a1 =  -2.0f * cosOmega;
        a2 =   1.0f - alpha;
        break;

      case Type::BiquadHighpass:
        b0 =  (1.0f + cosOmega) * 0.5f;
        b1 = -(1.0f + cosOmega);
        b2 =  (1.0f + cosOmega) * 0.5f;
        a0 =   1.0f + alpha;
        a1 =  -2.0f * cosOmega;
        a2 =   1.0f - alpha;
        break;

      case Type::BiquadBandpass:
        b0 =   sinOmega * 0.5f;
        b1 =   0.0f;
        b2 =  -sinOmega * 0.5f;
        a0 =   1.0f + alpha;
        a1 =  -2.0f * cosOmega;
        a2 =   1.0f - alpha;
        break;
    }

    // Normalize
    b0_ = b0 / a0;
    b1_ = b1 / a0;
    b2_ = b2 / a0;
    a1_ = a1 / a0;
    a2_ = a2 / a0;
    
}

void Filter::resetSate() {
    z1_ = 0.0f;
    z2_ = 0.0f;
}
