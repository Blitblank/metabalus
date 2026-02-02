
#include "KeyboardController.h"

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <filesystem>

KeyboardController::KeyboardController(NoteQueue& queue, ConfigInterface* config) : queue_(queue), config_(config) {

    // load keymap from config file
    std::string filepath = "config/keymap.yaml";
    filepath = std::filesystem::absolute(filepath).string();
    YAML::Node file;
    try {
        file = YAML::LoadFile(filepath);
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    YAML::Node keymapNode = file["keymap"]; // node for string to string mappings
    YAML::Node notesNode = file["notes"]; // string to midi int mappings
    YAML::Node keysNode = file["keys"]; // string to qt key id mappings

    // for each element in the keymap
    for (const auto& entry : keymapNode) {

        std::string keyString = entry.first.as<std::string>();
        std::string noteString = entry.second.as<std::string>();

        // match the strings to ints 
        uint8_t noteValue = notesNode[noteString].as<uint8_t>();
        uint32_t keyValue = keysNode[keyString].as<uint32_t>();

        // insert into map
        keymap_.emplace(keyValue, noteValue);
    }

}

void KeyboardController::handleKeyPress(QKeyEvent* e) {
    if (e->isAutoRepeat()) return;

    auto it = keymap_.find(e->key());
    if (it == keymap_.end()) return;

    queue_.push({
        NoteEventType::NoteOn,
        it->second,
        0.8f,
        std::chrono::high_resolution_clock::now()
    });
}

void KeyboardController::handleKeyRelease(QKeyEvent* e) {
    if (e->isAutoRepeat()) return;

    auto it = keymap_.find(e->key());
    if (it == keymap_.end()) return;

    queue_.push({
        NoteEventType::NoteOff,
        it->second,
        0.8f,
        std::chrono::high_resolution_clock::now()
    });
}