
#pragma once

#include <cstdint>
#include <array>
#include <atomic>

enum class ParamId : uint16_t {
    Osc1Frequency,
    Osc1Gain,
    Osc1VolumeEnvA,
    Osc1VolumeEnvD,
    Osc1VolumeEnvS,
    Osc1VolumeEnvR,
    FilterCutoffEnvA,
    FilterCutoffEnvD,
    FilterCutoffEnvS,
    FilterCutoffEnvR,
    FilterResonanceEnvA,
    FilterResonanceEnvD,
    FilterResonanceEnvS,
    FilterResonanceEnvR,
    // ... and so on
    // this list could be like 200 long if I really wanted to
    Count // for sizing
};

struct ParamDef {
    float def;
    float min;
    float max;
};

// TODO: make these configurable via yml file too
// TODO: and then when I have full on profile saving there will be a default profile to load from
constexpr std::array<ParamDef, static_cast<size_t>(ParamId::Count)> PARAM_DEFS {{
    { 100.0f, 20.0f, 600.0f},   // Osc1Freq
    { 0.8f, 0.0f, 1.0f},        // Osc1Gain
    { 10.0f, 0.0f, 1000.0f},    // Osc1VolumeEnvA,
    { 10.0f, 0.0f, 1000.0f},    // Osc1VolumeEnvD,
    { 10.0f, 0.0f, 1000.0f},    // Osc1VolumeEnvS,
    { 10.0f, 0.0f, 1000.0f},    // Osc1VolumeEnvR,
    { 10.0f, 0.0f, 1000.0f},    // FilterCutoffEnvA,
    { 10.0f, 0.0f, 1000.0f},    // FilterCutoffEnvD,
    { 10.0f, 0.0f, 1000.0f},    // FilterCutoffEnvS,
    { 10.0f, 0.0f, 1000.0f},    // FilterCutoffEnvR,
    { 10.0f, 0.0f, 1000.0f},    // FilterResonanceEnvA,
    { 10.0f, 0.0f, 1000.0f},    // FilterResonanceEnvD,
    { 10.0f, 0.0f, 1000.0f},    // FilterResonanceEnvS,
    { 10.0f, 0.0f, 1000.0f},    // FilterResonanceEnvR,
}};

constexpr size_t PARAM_COUNT = static_cast<size_t>(ParamId::Count);

class ParameterStore {

public:

    ParameterStore() { resetToDefaults(); }
    ~ParameterStore() = default;

    // TODO: move into implementation file ? idk
    void set(ParamId id, float value) {
        values_[static_cast<size_t>(id)].store(value, std::memory_order_relaxed);
    }
    float get(ParamId id) const {
        return values_[static_cast<size_t>(id)].load(std::memory_order_relaxed);
    }

    void resetToDefaults() {
        for(size_t i = 0; i < PARAM_COUNT; i++) {
            values_[i].store(PARAM_DEFS[i].def, std::memory_order_relaxed);
        }
    }

private:

    std::array<std::atomic<float>, PARAM_COUNT> values_;

};
