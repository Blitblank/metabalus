
#pragma once

#include <RtMidi.h>
#include <memory>
#include "NoteQueue.h"

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

    std::unique_ptr<RtMidiIn> midiIn_;
    NoteQueue& noteQueue_;
};
