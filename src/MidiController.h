
#pragma once

#include <RtMidi.h>
#include <memory>
#include "NoteQueue.h"
#include <unordered_set>

class MidiController {
public:
    explicit MidiController(NoteQueue& queue);
    ~MidiController();

    bool openDefaultPort();
    bool openPort(unsigned int index);
    void close();

private:
    static void midiCallback(
        double deltaTime,
        std::vector<unsigned char>* message,
        void* userData
    );

    void handleMessage(const std::vector<unsigned char>& msg);
    void handleSustain(bool down);
    void noteOn(uint8_t note, uint8_t vel);
    void noteOff(uint8_t note);

    std::unique_ptr<RtMidiIn> midiIn_;
    NoteQueue& noteQueue_;

    bool sustainDown_ = false;
    std::unordered_set<uint8_t> sustainedNotes_;

};
