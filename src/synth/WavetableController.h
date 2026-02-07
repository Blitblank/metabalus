
#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <filesystem>

#define SYNTH_WAVETABLE_SIZE 2048
#ifndef M_PI // I hate my stupid chungus life
    #define M_PI 3.14159265358979323846
#endif

typedef std::array<float, SYNTH_WAVETABLE_SIZE> Wavetable;

class WavetableController {

private:
    /* data */
public:

    WavetableController();
    ~WavetableController() = default;

    void init();

    // phase = [0, 2pi)
    float sample(uint8_t wavetableIndex, float phase);

private:

    std::vector<Wavetable> wavetables_;

    const std::filesystem::path wavetablesRoot_ = "./config/wavetables";

};


