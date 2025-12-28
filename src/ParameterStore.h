
#pragma once

#include <cstdint>
#include <array>
#include <atomic>

enum class ParamId : uint16_t {
    Osc1Frequency,
    Osc1WaveSelector1,
    Osc1WaveSelector2,
    Osc1VolumeDepth,
    Osc1VolumeEnvA,
    Osc1VolumeEnvD,
    Osc1VolumeEnvS,
    Osc1VolumeEnvR,
    FilterCutoffDepth,
    FilterCutoffEnvA,
    FilterCutoffEnvD,
    FilterCutoffEnvS,
    FilterCutoffEnvR,
    FilterResonanceDepth,
    FilterResonanceEnvA,
    FilterResonanceEnvD,
    FilterResonanceEnvS,
    FilterResonanceEnvR,
    // ... and so on
    // this list could be like 200 long if I really wanted to
    Count // for sizing
};

enum class EnvelopeId : uint16_t {
    Osc1Volume,
    Osc2Volume,
    Osc3Volume,
    FilterCutoff,
    FilterResonance,
    Count
};

struct EnvelopeParam {
    ParamId depth;
    ParamId a;
    ParamId d;
    ParamId s;
    ParamId r;
};

constexpr std::array<EnvelopeParam, static_cast<size_t>(EnvelopeId::Count)> ENV_PARAMS {{
    { ParamId::Osc1VolumeDepth, ParamId::Osc1VolumeEnvA, ParamId::Osc1VolumeEnvD, ParamId::Osc1VolumeEnvS, ParamId::Osc1VolumeEnvR }, // Osc1Volume
    { ParamId::Osc1VolumeDepth, ParamId::Osc1VolumeEnvA, ParamId::Osc1VolumeEnvD, ParamId::Osc1VolumeEnvS, ParamId::Osc1VolumeEnvR }, // Osc2Volume (not implemented)
    { ParamId::Osc1VolumeDepth, ParamId::Osc1VolumeEnvA, ParamId::Osc1VolumeEnvD, ParamId::Osc1VolumeEnvS, ParamId::Osc1VolumeEnvR }, // Osc3Volume (not implemented)
    { ParamId::FilterCutoffDepth, ParamId::FilterCutoffEnvA, ParamId::FilterCutoffEnvD, ParamId::FilterCutoffEnvS, ParamId::FilterCutoffEnvR }, // FilterCutoff
    { ParamId::FilterResonanceDepth, ParamId::FilterResonanceEnvA, ParamId::FilterResonanceEnvD, ParamId::FilterResonanceEnvS, ParamId::FilterResonanceEnvR }, // FilterResonance
}};

struct ParamDefault {
    float def;
    float min;
    float max;
};

// TODO: make these configurable via yml file too
// later TODO: and then when I have full on profile saving there will be a default profile to load from
constexpr std::array<ParamDefault, static_cast<size_t>(ParamId::Count)> PARAM_DEFS {{
    { 100.0f, 20.0f,  600.0f},   // Osc1Freq
    {   0.0f,  0.0f,    0.0f},   // Osc1WaveSelector1
    {   1.0f,  0.0f,    0.0f},   // Osc1WaveSelector2
    {   1.0f,  0.0f,    2.0f},   // Osc1VolumeDepth
    {  0.05f,  0.0f,    2.0f},   // Osc1VolumeEnvA
    {   0.2f,  0.0f,    2.0f},   // Osc1VolumeEnvD
    {   0.7f,  0.0f,    1.0f},   // Osc1VolumeEnvS
    {   0.2f,  0.0f,    2.0f},   // Osc1VolumeEnvR
    {   1.0f,  0.0f,    8.0f},   // FilterCutoffDepth
    {  0.05f,  0.0f,    2.0f},   // FilterCutoffEnvA
    {  0.05f,  0.0f,    2.0f},   // FilterCutoffEnvD
    {   2.0f,  0.0f,    1.0f},   // FilterCutoffEnvS
    {  0.05f,  0.0f,    2.0f},   // FilterCutoffEnvR
    { 1.414f,  0.0f,    8.0f},   // FilterResonanceDepth
    {  0.05f,  0.0f,    2.0f},   // FilterResonanceEnvA
    {  0.05f,  0.0f,    2.0f},   // FilterResonanceEnvD
    {   0.5f,  0.0f,    1.0f},   // FilterResonanceEnvS
    {  0.05f,  0.0f,    2.0f},   // FilterResonanceEnvR
}};

constexpr size_t PARAM_COUNT = static_cast<size_t>(ParamId::Count);

class ParameterStore {

public:

    ParameterStore();
    ~ParameterStore() = default;

    void set(ParamId id, float value);
    void set(ParamId id, int32_t value) { set(id, static_cast<float>(value)); }
    void set(EnvelopeId id, float depth, float a, float d, float s, float r);
    float get(ParamId id) const;
    int32_t getInt(ParamId id) const { return static_cast<int32_t>(get(id)); }
    void resetToDefaults();

private:

    std::array<std::atomic<float>, PARAM_COUNT> values_;

};
