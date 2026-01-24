
#include "ConfigInterface.h"

#include "yaml-cpp/yaml.h"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

ConfigInterface::ConfigInterface() {
    
    std::string audioConfig = configRoot + "/" + filename;

    std::cout << "CWD: " << fs::current_path() << std::endl;
    std::cout << "Audio config: " << fs::absolute(audioConfig).string() << std::endl;

    // opening the file myself because YAML::LoadFile didn't seem to work on windows
    std::ifstream file(fs::absolute(audioConfig).string());
    if(file.good()) {
        std::cout << "File exists" << std::endl;
    } else {
        std::cout << "File does not exist" << std::endl;
    }

    //YAML::Node config;
    /*
    try {
        YAML::Node config = YAML::Load("{sampleRate: 44100}");
        std::cout << "Loaded config file" << std::endl;

        std::cout << "Type enum: " << static_cast<int>(config.Type()) << std::endl;

        if(config["sampleRate"]) {
            //int sampleRate = config["sampleRate"].as<int>();
            //std::cout << sampleRate << std::endl;
        } else {
            std::cout << "Key does not exist" << std::endl;
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    */

    YAML::Node n = YAML::Load("{sampleRate: 44100}");
    std::cout << n["sampleRate"].as<int>() << "\n";    

}
