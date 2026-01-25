
#pragma once

#include <string>
#include <vector>
#include <iostream>

enum class ConfigFile {
    Audio = 0
    // other files here
};

// might have a config file for specifying paths to other config files instead of this
const std::vector<std::string> filePaths = {
    "audio.yaml"
};

class ConfigInterface {

public:

    ConfigInterface();
    ~ConfigInterface() = default;

    int getValue(ConfigFile file, std::string key, int defaultVal);

private:

    const std::string configRoot = "config";

};
