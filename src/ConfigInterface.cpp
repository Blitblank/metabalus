
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
        std::cout << "Parameter profile version " << version << std::endl;
    }

    // extract values from the config file
    std::array<ParamDefault, 5> osc1VolumeProfile = loadEnvProfile(&config, "Osc1Volume");
    std::array<ParamDefault, 5> fCutoffProfile = loadEnvProfile(&config, "FilterCutoff");
    std::array<ParamDefault, 5> fResonanceProfile = loadEnvProfile(&config, "FilterResonance");

    std::array<ParamDefault, 3> masterPitchOffsets = {{
        { config["MasterOctaveOffset"][0].as<float>(), config["MasterOctaveOffset"][1].as<float>(), config["MasterOctaveOffset"][2].as<float>() },
        { config["MasterSemitoneOffset"][0].as<float>(), config["MasterSemitoneOffset"][1].as<float>(), config["MasterSemitoneOffset"][2].as<float>() },
        { config["MasterPitchOffset"][0].as<float>(), config["MasterPitchOffset"][1].as<float>(), config["MasterPitchOffset"][2].as<float>() },
    }};
    std::array<ParamDefault, 3> osc1PitchOffsets = {{
        { config["Osc1OctaveOffset"][0].as<float>(), config["Osc1OctaveOffset"][1].as<float>(), config["Osc1OctaveOffset"][2].as<float>() },
        { config["Osc1SemitoneOffset"][0].as<float>(), config["Osc1SemitoneOffset"][1].as<float>(), config["Osc1SemitoneOffset"][2].as<float>() },
        { config["Osc1PitchOffset"][0].as<float>(), config["Osc1PitchOffset"][1].as<float>(), config["Osc1PitchOffset"][2].as<float>() },
    }};
    std::array<ParamDefault, 3> osc2PitchOffsets = {{
        { config["Osc2OctaveOffset"][0].as<float>(), config["Osc2OctaveOffset"][1].as<float>(), config["Osc2OctaveOffset"][2].as<float>() },
        { config["Osc2SemitoneOffset"][0].as<float>(), config["Osc2SemitoneOffset"][1].as<float>(), config["Osc2SemitoneOffset"][2].as<float>() },
        { config["Osc2PitchOffset"][0].as<float>(), config["Osc2PitchOffset"][1].as<float>(), config["Osc2PitchOffset"][2].as<float>() },
    }};
    std::array<ParamDefault, 3> osc3PitchOffsets = {{
        { config["Osc3OctaveOffset"][0].as<float>(), config["Osc3OctaveOffset"][1].as<float>(), config["Osc3OctaveOffset"][2].as<float>() },
        { config["Osc3SemitoneOffset"][0].as<float>(), config["Osc3SemitoneOffset"][1].as<float>(), config["Osc3SemitoneOffset"][2].as<float>() },
        { config["Osc3PitchOffset"][0].as<float>(), config["Osc3PitchOffset"][1].as<float>(), config["Osc3PitchOffset"][2].as<float>() },
    }};

    // TODO: remove this once all the parameters are set properly
    params_->resetToDefaults();

    // set the values in the paramstore
    params_->set(EnvelopeId::Osc1Volume, osc1VolumeProfile[0].def, osc1VolumeProfile[1].def, osc1VolumeProfile[2].def, osc1VolumeProfile[3].def, osc1VolumeProfile[4].def);
    params_->set(EnvelopeId::FilterCutoff, fCutoffProfile[0].def, fCutoffProfile[1].def, fCutoffProfile[2].def, fCutoffProfile[3].def, fCutoffProfile[4].def);
    params_->set(EnvelopeId::FilterResonance, fResonanceProfile[0].def, fResonanceProfile[1].def, fResonanceProfile[2].def, fResonanceProfile[3].def, fResonanceProfile[4].def);
    // TODO: why do I bother passing in 5 values independently when I can just do an array like in loadEnvProfile ?
    params_->set(ParamId::MasterOctaveOffset, masterPitchOffsets[0].def);
    params_->set(ParamId::MasterSemitoneOffset, masterPitchOffsets[1].def);
    params_->set(ParamId::MasterPitchOffset, masterPitchOffsets[2].def);
    params_->set(ParamId::Osc1OctaveOffset, osc1PitchOffsets[0].def);
    params_->set(ParamId::Osc1SemitoneOffset, osc1PitchOffsets[1].def);
    params_->set(ParamId::Osc1PitchOffset, osc1PitchOffsets[2].def);
    params_->set(ParamId::Osc2OctaveOffset, osc2PitchOffsets[0].def);
    params_->set(ParamId::Osc2SemitoneOffset, osc2PitchOffsets[1].def);
    params_->set(ParamId::Osc2PitchOffset, osc2PitchOffsets[2].def);
    params_->set(ParamId::Osc3OctaveOffset, osc3PitchOffsets[0].def);
    params_->set(ParamId::Osc3SemitoneOffset, osc3PitchOffsets[1].def);
    params_->set(ParamId::Osc3PitchOffset, osc3PitchOffsets[2].def);

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
