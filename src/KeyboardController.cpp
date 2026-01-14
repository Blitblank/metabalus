
#include "KeyboardController.h"

#include <iostream>

KeyboardController::KeyboardController(NoteQueue& queue) : queue_(queue) {

    // TODO: also configurable via a yml
    keymap_ = {
        { Qt::Key_Shift, 47 }, // B 2
        { Qt::Key_Z, 48 }, // C 3
        { Qt::Key_S, 49 }, // C#
        { Qt::Key_X, 50 }, // D
        { Qt::Key_D, 51 }, // D#
        { Qt::Key_C, 52 }, // E
        { Qt::Key_V, 53 }, // F
        { Qt::Key_G, 54 }, // F#
        { Qt::Key_B, 55 }, // G
        { Qt::Key_H, 56 }, // G#
        { Qt::Key_N, 57 }, // A
        { Qt::Key_J, 58 }, // A#
        { Qt::Key_M, 59 }, // B 3
        { Qt::Key_Q, 60 }, // C 4
        { Qt::Key_2, 61 }, // C#
        { Qt::Key_W, 62 }, // D
        { Qt::Key_3, 63 }, // D#
        { Qt::Key_E, 64 }, // E
        { Qt::Key_R, 65 }, // F
        { Qt::Key_5, 66 }, // F#
        { Qt::Key_T, 67 }, // G
        { Qt::Key_6, 68 }, // G#
        { Qt::Key_Y, 69 }, // A
        { Qt::Key_7, 70 }, // A#
        { Qt::Key_U, 71 }, // B 4
        { Qt::Key_I, 72 }, // C 5
    };
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

// TODO: something like a sustain pedal will suspend note-off events. probably control that in the midi controller
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