
#include "ConfigInterface.h"

#include "yaml-cpp/yaml.h"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

ConfigInterface::ConfigInterface() {
    
    //std::cout << "Config constructor" << std::endl;

}

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
