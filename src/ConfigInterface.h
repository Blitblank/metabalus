
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "yaml-cpp/yaml.h"

#include "ParameterStore.h"

#define CONFIG_VERSION 0x0002

enum class ConfigFile {
    Audio = 0
    // other files here
};

// might have a config file for specifying paths to other config files instead of this
const std::vector<std::string> filePaths = {
    "audio.yaml"
};

// Reads from yaml config files
// Handles things like profile loading
class ConfigInterface {

public:

    ConfigInterface();
    ConfigInterface(ParameterStore* params);
    ~ConfigInterface() = default;

    int getValue(ConfigFile file, std::string key, int defaultVal);

    YAML::Node loadProfile(std::string filename);
    std::array<ParamDefault, 5> loadEnvProfile(YAML::Node* node, std::string profile);
    std::array<ParamDefault, 5> loadEnvProfile(std::string filename, std::string profile);

private:

    const std::string configRoot = "config";

    // loading parameters
    ParameterStore* params_;

};
