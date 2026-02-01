
#include "WavetableController.h"

#include <cmath>
#include <iostream>

WavetableController::WavetableController() {
    // load from files

    init();

    //std::cout << "wavetable init" << std::endl;

}

void WavetableController::init() {

    wavetables_.resize(4); // resize for however many files we find

    // don't really know how the files are gonna work
    // but I'd like two files- a yaml that contains metadata like name, length, range, datatype, etc.
    // and the main data be just a big array of that data type in a binary file
    // although having it all in a single bin makes the most sense with the metadata being in the header

    float phase = 0.0f;
    float phaseInc = 2.0f * M_PI / static_cast<float>(SYNTH_WAVETABLE_SIZE);

    for(int i = 0; i < SYNTH_WAVETABLE_SIZE; i++) {

        wavetables_[0][i] = std::sin(phase) / 0.707f; // sine
        wavetables_[1][i] = (phase >= M_PI) ? 1.0f : -1.0f; // square
        wavetables_[2][i] = ((phase / M_PI) - 1.0f) / 0.577f; // saw

        // triangle
        float tri = 0.0f;
        if(phase <= M_PI/2.0f) {
            tri = phase * 2.0f/M_PI;
        } else if(phase <= 3.0f*M_PI/2.0f) {
            tri = phase * -2.0f/M_PI + 2.0f;
        } else {
            tri = phase * 2.0f/M_PI - 4.0f;
        }
        wavetables_[3][i] = tri / 0.577f; 

        phase += phaseInc;
    }

}

float WavetableController::sample(uint8_t wavetableIndex, float phase) {

    float sampleValue = 0.0f;

    if(phase >= 0.0f) {
        while(phase >= 2.0f*M_PI) {
            phase -= 2.0f*M_PI;
        }
    } else {
        while(phase <= 0.0f*M_PI) {
            phase += 2.0f*M_PI;
        }
    }

    float scaledPhase = phase * static_cast<float>(SYNTH_WAVETABLE_SIZE) / (2.0f*M_PI);
    uint32_t index = static_cast<uint32_t>(round(scaledPhase));
    if(index >= SYNTH_WAVETABLE_SIZE) index = SYNTH_WAVETABLE_SIZE - 1;

    if(wavetableIndex >= 4) {
        wavetableIndex = 3;
    } else if(wavetableIndex < 0) {
        wavetableIndex = 0;
    }

    return wavetables_[wavetableIndex][index];
}