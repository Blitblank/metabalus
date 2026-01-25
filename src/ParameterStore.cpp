
#include "ParameterStore.h"

#include <iostream>
#include "yaml-cpp/yaml.h" // TODO: using yaml.h outside of ConfigInterface feels spaghetti to me
#include <filesystem>

ParameterStore::ParameterStore(ConfigInterface* config) : config_(config) {
    resetToDefaults();
}

// set parameter value
void ParameterStore::set(ParamId id, float value) {
    values_[static_cast<size_t>(id)].store(value, std::memory_order_relaxed);
}

// set a whole envelope of parameters
void ParameterStore::set(EnvelopeId id, float depth, float a, float d, float s, float r) {

    EnvelopeParam params = ENV_PARAMS[static_cast<size_t>(id)];
    set(params.depth, depth);
    set(params.a, a);
    set(params.d, d);
    set(params.s, s);
    set(params.r, r);
}

// get a single parameter
float ParameterStore::get(ParamId id) const {
    return values_[static_cast<size_t>(id)].load(std::memory_order_relaxed);
}

void ParameterStore::resetToDefaults() {
    for(size_t i = 0; i < PARAM_COUNT; i++) {
        values_[i].store(PARAM_DEFS[i].def, std::memory_order_relaxed);
    }

    loadParameterProfile("config/profiles/default.yaml");

}

void ParameterStore::loadParameterProfile(std::string filepath) {

    // TODO: abstract the actual yaml interfacing to the ConfigInterface instead of here
    // TODO: update ui based on changes that happen not in the ui
    // it will require some architecture rework :(

    // load file
    filepath = std::filesystem::absolute(filepath).string();
    YAML::Node config;
    try {
        config = YAML::LoadFile(filepath);
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    
    // check version
    int version = config["version"].as<int>(); // yaml-cpp parses unquoted hex as integers
    if(version < CONFIG_VERSION) {
        std::cout << "Parameter profile version " << version << "is outdated below the compatible version " << CONFIG_VERSION << std::endl;
        return;
    } else {
        //std::cout << version << std::endl;
    }
    
    // start setting some values
    YAML::Node osc1Volume = config["Osc1Volume"];
    YAML::Node fCutoff = config["FilterCutoff"];
    YAML::Node fResonance = config["FilterResonance"];
    set(EnvelopeId::Osc1Volume, osc1Volume[0][0].as<float>(), osc1Volume[1][0].as<float>(), osc1Volume[2][0].as<float>(), osc1Volume[3][0].as<float>(), osc1Volume[4][0].as<float>());
    set(EnvelopeId::FilterCutoff, fCutoff[0][0].as<float>(), fCutoff[1][0].as<float>(), fCutoff[2][0].as<float>(), fCutoff[3][0].as<float>(), fCutoff[4][0].as<float>());
    set(EnvelopeId::FilterResonance, fResonance[0][0].as<float>(), fResonance[1][0].as<float>(), fResonance[2][0].as<float>(), fResonance[3][0].as<float>(), fResonance[4][0].as<float>());

}
