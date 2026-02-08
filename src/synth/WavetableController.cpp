
#include "WavetableController.h"

#include <cmath>
#include <iostream>
#include <fstream>

WavetableController::WavetableController() {
    // load from files

    init();

    //std::cout << "wavetable init" << std::endl;

}

void WavetableController::init() {

    // find wavetable files
    std::vector<std::filesystem::path> wavetableFiles;
    for(std::filesystem::directory_entry entry : std::filesystem::directory_iterator(wavetablesRoot_)) {
        if(std::filesystem::is_regular_file(entry.status())) {
            wavetableFiles.push_back(entry.path());
        }
    }
    uint32_t wavetableCount = wavetableFiles.size();
    wavetables_.resize(wavetableCount);

    // load the wavetable files
    for(int i = 0; i < wavetableCount; i++) {
        std::cout << "loading wavetable file [" << i << "]: " << wavetableFiles[i] << std::endl;
        std::ifstream inputFile(wavetableFiles[i], std::ios::in | std::ios::binary);
        if(!inputFile) std::cout << "error opening file" << std::endl;
        inputFile.read(reinterpret_cast<char*>(wavetables_[i].data()), SYNTH_WAVETABLE_SIZE * sizeof(float));
    }

    // wavetable data structure is best explained in scripts/generate_wavetable.py

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