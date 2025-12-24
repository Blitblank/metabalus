
#pragma once

#include "../ParameterStore.h"

#include <atomic>

struct SmoothedParam {
    float current = 0.0f;
    float target = 0.0f;
    float gain = 0.001f;

    inline void update() {
        current += gain * (target - current);
    }
};

class Synth {

public:
    Synth(const ParameterStore& params);
    ~Synth() = default;

    void process(float* out, uint32_t nFrames, uint32_t sampleRate);
    void setSampleRate(uint32_t sampleRate) { sampleRate_ = sampleRate; }

private:

    void updateParams();
    inline float getParam(ParamId);

    const ParameterStore& paramStore_;
    // smoothed params creates a buffer in case the thread controlling paramStore gets blocked
    std::array<SmoothedParam, PARAM_COUNT> params_;
    uint32_t sampleRate_;

    float phase_ = 0.0f;
};
