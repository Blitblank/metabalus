
#include "KeyboardController.h"

#include <iostream>

KeyboardController::KeyboardController(NoteQueue& queue) : queue_(queue) {

    // TODO: also configurable via a yml
    keymap_ = {
        { Qt::Key_Z, 60-12 }, // C
        { Qt::Key_S, 61-12 }, // C#
        { Qt::Key_X, 62-12 }, // D
        { Qt::Key_D, 63-12 }, // D#
        { Qt::Key_C, 64-12 }, // E
        { Qt::Key_V, 65-12 }, // F
        { Qt::Key_G, 66-12 }, // F#
        { Qt::Key_B, 67-12 }, // G
        { Qt::Key_H, 68-12 }, // G#
        { Qt::Key_N, 69-12 }, // A
        { Qt::Key_J, 70-12 }, // A#
        { Qt::Key_M, 71-12 }, // B
        { Qt::Key_Q, 72-12 }  // C (octave up)
    };
}

void KeyboardController::handleKeyPress(QKeyEvent* e) {
    if (e->isAutoRepeat()) return;

    auto it = keymap_.find(e->key());
    if (it == keymap_.end()) return;

    queue_.push({
        NoteEventType::NoteOn,
        it->second,
        0.8f
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
        0.8f
    });
}