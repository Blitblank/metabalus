
#include "MidiController.h"

#include <iostream>
#include <chrono>

MidiController::MidiController(NoteQueue& queue) : noteQueue_(queue) {
    try {
        midiIn_ = std::make_unique<RtMidiIn>();
        midiIn_->ignoreTypes(true, true, true);
    } catch (RtMidiError& e) {
        std::cerr << "RtMidi init failed: " << e.getMessage() << std::endl;
    }
    // TODO: this still doesnt work on windows
}

MidiController::~MidiController() {
    close();
}

bool MidiController::openDefaultPort() {
    if (!midiIn_) return false;
    if (midiIn_->getPortCount() == 0) {
        std::cerr << "No MIDI input ports available\n";
        return false;
    }
    return openPort(0);
}

bool MidiController::openPort(unsigned int index) {
    if (!midiIn_) return false;

    try {
        midiIn_->openPort(index);
        midiIn_->setCallback(&MidiController::midiCallback, this);
        std::cout << "Opened MIDI port: "
                  << midiIn_->getPortName(index) << "\n";
        return true;
    } catch (RtMidiError& e) {
        std::cerr << e.getMessage() << std::endl;
        return false;
    }
}

void MidiController::close() {
    if (midiIn_ && midiIn_->isPortOpen()) {
        midiIn_->closePort();
    }
}

void MidiController::midiCallback(double /*deltaTime*/, std::vector<unsigned char>* message, void* userData) {
    auto* self = static_cast<MidiController*>(userData);
    if (!message || message->empty()) return;
    self->handleMessage(*message);
}

void MidiController::handleMessage(const std::vector<unsigned char>& msg) {
    unsigned char status = msg[0] & 0xF0;

    if (status == 0xFE) return;
    if (status == 0xF8) return;

    unsigned char note   = msg.size() > 1 ? msg[1] : 0;
    unsigned char vel    = msg.size() > 2 ? msg[2] : 0;

    // Note On (velocity > 0)
    if (status == 0x90 && vel > 0) {
        noteQueue_.push({
            NoteEventType::NoteOn,
            static_cast<uint8_t>(note),
            vel / 127.0f,
            std::chrono::high_resolution_clock::now()
        });
    }
    // Note Off (or Note On with velocity 0)
    else if (status == 0x80 || (status == 0x90 && vel == 0)) {
        noteQueue_.push({
            NoteEventType::NoteOff,
            static_cast<uint8_t>(note),
            0.0f,
            std::chrono::high_resolution_clock::now()
        });
    }

    std::cout << "MidiHandleMessage start (0)" << std::endl;
}