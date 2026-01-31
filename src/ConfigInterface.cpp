
#include "ConfigInterface.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

ConfigInterface::ConfigInterface() {
    
    //std::cout << "Config constructor" << std::endl;

}

ConfigInterface::ConfigInterface(ParameterStore* params): params_(params) {

}

// lots of checking in this to make this safe
int ConfigInterface::getValue(ConfigFile file, std::string key, int defaultVal) {

    // assemble filepath
    std::string filepath = configRoot + "/" + filePaths[static_cast<int>(file)];
    filepath = fs::absolute(filepath).string();
    
    // attempt to open file
    YAML::Node config;
    try {

        YAML::Node config = YAML::LoadFile(filepath);

        // read key if it exists
        if(config[key]) {
            return config[key].as<int>(defaultVal);
        } else {
            return -1; // key does not exist
        }

    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    // unreachable
    return -1;

}

// ugly but if it works it works
void ConfigInterface::loadProfile(std::string filename) {

    // load file
    std::string filepath = "config/profiles/" + filename + ".yaml";
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
        std::cout << version << std::endl;
    }

    // extract values from the config file
    std::array<ParamDefault, 5> osc1VolumeProfile = loadEnvProfile(&config, "Osc1Volume");
    std::array<ParamDefault, 5> fCutoffProfile = loadEnvProfile(&config, "FilterCutoff");
    std::array<ParamDefault, 5> fResonanceProfile = loadEnvProfile(&config, "FilterResonance");

    // TODO: remove this once all the parameters are set properly
    params_->resetToDefaults();

    // set the values in the paramstore
    params_->set(EnvelopeId::Osc1Volume, osc1VolumeProfile[0].def, osc1VolumeProfile[1].def, osc1VolumeProfile[2].def, osc1VolumeProfile[3].def, osc1VolumeProfile[4].def);
    params_->set(EnvelopeId::FilterCutoff, fCutoffProfile[0].def, fCutoffProfile[1].def, fCutoffProfile[2].def, fCutoffProfile[3].def, fCutoffProfile[4].def);
    params_->set(EnvelopeId::FilterResonance, fResonanceProfile[0].def, fResonanceProfile[1].def, fResonanceProfile[2].def, fResonanceProfile[3].def, fResonanceProfile[4].def);
    // TODO: why do I bother passing in 5 values independently when I can just do an array ?
    // VVV look down there its so easy

    // TODO:
    // load wavetable settings
    // load oscillator pitch settings

}

std::array<ParamDefault, 5> ConfigInterface::loadEnvProfile(YAML::Node* node, std::string profile) {

    YAML::Node envelopeNode = (*node)[profile];

    std::array<ParamDefault, 5> paramProfile;

    for(int i = 0; i < paramProfile.size(); i++) {
        paramProfile[i] = { envelopeNode[i][0].as<float>(), envelopeNode[i][1].as<float>(), envelopeNode[i][2].as<float>() };
    }

    return paramProfile;
}

std::array<ParamDefault, 5> ConfigInterface::loadEnvProfile(std::string filename, std::string profile) {

    std::string filepath = "config/profiles/" + filename + ".yaml";
    filepath = std::filesystem::absolute(filepath).string();
    YAML::Node config = YAML::LoadFile(filepath);

    return loadEnvProfile(&config, profile);
}
