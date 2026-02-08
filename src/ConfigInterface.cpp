
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
YAML::Node ConfigInterface::loadProfile(std::string filename) {

    // load file
    std::string filepath = "config/profiles/" + filename + ".yaml";
    filepath = std::filesystem::absolute(filepath).string();
    YAML::Node config;
    try {
        config = YAML::LoadFile(filepath);
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return config;
    }
    
    // check version
    int version = config["version"].as<int>(); // yaml-cpp parses unquoted hex as integers
    if(version < CONFIG_VERSION) {
        std::cout << "Parameter profile version " << version << "is outdated below the compatible version " << CONFIG_VERSION << std::endl;
        return config;
    } else {
        std::cout << "Parameter profile version " << version << std::endl;
    }

    // extract values from the config file
    std::array<Param, 5> osc1VolumeProfile = loadEnvProfile(&config, "Osc1Volume");
    std::array<Param, 5> fCutoffProfile = loadEnvProfile(&config, "FilterCutoff");
    std::array<Param, 5> fResonanceProfile = loadEnvProfile(&config, "FilterResonance");

    YAML::Node masterNode = config["MasterPitchOffset"];
    YAML::Node osc1Node = config["Osc1PitchOffset"];
    YAML::Node osc2Node = config["Osc2PitchOffset"];
    YAML::Node osc3Node = config["Osc3PitchOffset"];
    std::array<Param, 3> masterPitchOffsets = {{
        { masterNode["Octave"][0].as<float>(), masterNode["Octave"][1].as<float>(), masterNode["Octave"][2].as<float>() },
        { masterNode["Semitone"][0].as<float>(), masterNode["Semitone"][1].as<float>(), masterNode["Semitone"][2].as<float>() },
        { masterNode["Pitch"][0].as<float>(), masterNode["Pitch"][1].as<float>(), masterNode["Pitch"][2].as<float>() },
    }};
    std::array<Param, 3> osc1PitchOffsets = {{
        { osc1Node["Octave"][0].as<float>(), osc1Node["Octave"][1].as<float>(), osc1Node["Octave"][2].as<float>() },
        { osc1Node["Semitone"][0].as<float>(), osc1Node["Semitone"][1].as<float>(), osc1Node["Semitone"][2].as<float>() },
        { osc1Node["Pitch"][0].as<float>(),osc1Node["Pitch"][1].as<float>(), osc1Node["Pitch"][2].as<float>() },
    }};
    std::array<Param, 3> osc2PitchOffsets = {{
        { osc2Node["Octave"][0].as<float>(), osc2Node["Octave"][1].as<float>(), osc2Node["Octave"][2].as<float>() },
        { osc2Node["Semitone"][0].as<float>(), osc2Node["Semitone"][1].as<float>(), osc2Node["Semitone"][2].as<float>() },
        { osc2Node["Pitch"][0].as<float>(), osc2Node["Pitch"][1].as<float>(), osc2Node["Pitch"][2].as<float>() },
    }};
    std::array<Param, 3> osc3PitchOffsets = {{
        { osc3Node["Octave"][0].as<float>(), osc3Node["Octave"][1].as<float>(), osc3Node["Octave"][2].as<float>() },
        { osc3Node["Semitone"][0].as<float>(), osc3Node["Semitone"][1].as<float>(), osc3Node["Semitone"][2].as<float>() },
        { osc3Node["Pitch"][0].as<float>(), osc3Node["Pitch"][1].as<float>(), osc3Node["Pitch"][2].as<float>() },
    }};

    // set the values in the paramstore
    params_->set(EnvelopeId::Osc1Volume, osc1VolumeProfile[0].val, osc1VolumeProfile[1].val, osc1VolumeProfile[2].val, osc1VolumeProfile[3].val, osc1VolumeProfile[4].val);
    params_->set(EnvelopeId::FilterCutoff, fCutoffProfile[0].val, fCutoffProfile[1].val, fCutoffProfile[2].val, fCutoffProfile[3].val, fCutoffProfile[4].val);
    params_->set(EnvelopeId::FilterResonance, fResonanceProfile[0].val, fResonanceProfile[1].val, fResonanceProfile[2].val, fResonanceProfile[3].val, fResonanceProfile[4].val);
    // TODO: why do I bother passing in 5 values independently when I can just do an array like in loadEnvProfile ?
    params_->set(ParamId::MasterOctaveOffset, masterPitchOffsets[0].val);
    params_->set(ParamId::MasterSemitoneOffset, masterPitchOffsets[1].val);
    params_->set(ParamId::MasterPitchOffset, masterPitchOffsets[2].val);
    params_->set(ParamId::Osc1OctaveOffset, osc1PitchOffsets[0].val);
    params_->set(ParamId::Osc1SemitoneOffset, osc1PitchOffsets[1].val);
    params_->set(ParamId::Osc1PitchOffset, osc1PitchOffsets[2].val);
    params_->set(ParamId::Osc2OctaveOffset, osc2PitchOffsets[0].val);
    params_->set(ParamId::Osc2SemitoneOffset, osc2PitchOffsets[1].val);
    params_->set(ParamId::Osc2PitchOffset, osc2PitchOffsets[2].val);
    params_->set(ParamId::Osc3OctaveOffset, osc3PitchOffsets[0].val);
    params_->set(ParamId::Osc3SemitoneOffset, osc3PitchOffsets[1].val);
    params_->set(ParamId::Osc3PitchOffset, osc3PitchOffsets[2].val);

    params_->set(ParamId::Osc1WaveSelector1, static_cast<float>(config["OscWaveSelector1"].as<int>()));
    params_->set(ParamId::Osc1WaveSelector2, static_cast<float>(config["OscWaveSelector2"].as<int>()));

    return config;

}

std::array<Param, 5> ConfigInterface::loadEnvProfile(YAML::Node* node, std::string profile) {

    YAML::Node envelopeNode = (*node)[profile];

    std::array<Param, 5> paramProfile;

    paramProfile[0] = { envelopeNode["Depth"][0].as<float>(), envelopeNode["Depth"][1].as<float>(), envelopeNode["Depth"][2].as<float>() };
    paramProfile[1] = { envelopeNode["Attack"][0].as<float>(), envelopeNode["Attack"][1].as<float>(), envelopeNode["Attack"][2].as<float>() };
    paramProfile[2] = { envelopeNode["Decay"][0].as<float>(), envelopeNode["Decay"][1].as<float>(), envelopeNode["Decay"][2].as<float>() };
    paramProfile[3] = { envelopeNode["Sustain"][0].as<float>(), envelopeNode["Sustain"][1].as<float>(), envelopeNode["Sustain"][2].as<float>() };
    paramProfile[4] = { envelopeNode["Release"][0].as<float>(), envelopeNode["Release"][1].as<float>(), envelopeNode["Release"][2].as<float>() };

    return paramProfile;
}

std::array<Param, 5> ConfigInterface::loadEnvProfile(std::string filename, std::string profile) {

    std::string filepath = "config/profiles/" + filename + ".yaml";
    filepath = std::filesystem::absolute(filepath).string();
    YAML::Node config = YAML::LoadFile(filepath);

    return loadEnvProfile(&config, profile);
}
