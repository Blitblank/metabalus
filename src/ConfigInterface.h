
#pragma once

#include <string>

class ConfigInterface {

public:

    ConfigInterface();
    ~ConfigInterface() = default;

private:

    const std::string configRoot = "config";
    const std::string filename = "audio.yaml";

};
