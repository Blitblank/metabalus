
#pragma once

#include <cstdint>

class Filter {
public:

    enum class Type : uint16_t {
        BiquadLowpass,
        BiquadNotch,
        BiquadBandpass,
        BiquadHighpass
    };

    Filter() = default;
    ~Filter() = default;

    void setSampleRate(float sampleRate);
    void setParams(Type type, float frequency, float q);
    float biquadProcess(float in);
    void resetSate();

    // TODO: add more filter types here
    // high pass
    // band pass
    // notch
    // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
    // instead of making different calculate functions, have an enum which specifies the filter type
    // one public calculate which the enum is passed through and a private calculate for the specific types, switch statement to choose

private:

    void calculateCoefficients();

    Type type_ = Type::BiquadLowpass;
    float sampleRate_ = 44100.0f;

    float frequency_ = 6000.0f;
    float q_ = 0.707f;

    // biquad filter structure
    float a1_, a2_, b0_, b1_, b2_;
    float z1_, z2_;

};