
#pragma once

#include <QKeyEvent>
#include <unordered_map>

#include "NoteQueue.h"

// The keyboardcontroller handles user inputs from a keyboard and maps them to note events
class KeyboardController {

public:
    explicit KeyboardController(NoteQueue& queue);
    ~KeyboardController() = default;

    void handleKeyPress(QKeyEvent* e);
    void handleKeyRelease(QKeyEvent* e);

private:

    NoteQueue& queue_;

    // keymap is key -> midi note id
    std::unordered_map<int32_t, uint8_t> keymap_;

};
