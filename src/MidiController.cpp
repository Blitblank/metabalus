
#include "MidiController.h"

#include <iostream>
#include <chrono>

MidiController::MidiController(NoteQueue& queue) : noteQueue_(queue) {
    try {
        midiIn_ = std::make_unique<RtMidiIn>();
        midiIn_->ignoreTypes(false, false, false);
    } catch (RtMidiError& e) {
        std::cerr << "RtMidi init failed: " << e.getMessage() << std::endl;
    }
    // TODO: this still doesnt work on windows
}

MidiController::~MidiController() {
    close();
}

// this dont work too well but whatever
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
        std::cout << "Opened MIDI port: " << midiIn_->getPortName(index) << "\n";
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

// called by RtMidi on receive of a midi message. deltaTime is time since last midi message, not useful atm
void MidiController::midiCallback(double /*deltaTime*/, std::vector<unsigned char>* message, void* userData) {
    auto* self = static_cast<MidiController*>(userData);
    if (!message || message->empty()) return;
    self->handleMessage(*message); // pass to parsing function if valid
}

void MidiController::handleMessage(const std::vector<unsigned char>& msg) {

    if(msg.size() <= 1) return; // msg doesn't contain useful note info

    uint8_t status = msg[0] & 0xF0;
    uint8_t data1 = msg[1];
    uint8_t data2 = msg[2];

    if(status == 0xFE) return; // "Active Sensing" -> 300ms heartbeat. could be useful to sense if this is missing for device failure detection
    if(status == 0xF8) return; // "Timing Clock" -> 24 pulses per quarter note, for steady rhythm. not useful for this instrument

    // sustain pedal message event
    if(status == 0xB0 && data1 == 64) {
        handleSustain(data2 >= 64);
        return;
    }

    unsigned char note = msg.size() > 1 ? msg[1] : 0; // note number
    unsigned char vel = msg.size() > 2 ? msg[2] : 0; // velocity

    // note on (velocity > 0)
    if (status == 0x90 && vel > 0) {
        noteOn(note, vel);
    }
    // note off (or note on with 0 velocity)
    else if (status == 0x80 || (status == 0x90 && vel == 0)) {
        noteOff(note);
    }

}

// construct note on event and add to noteQueue
void MidiController::noteOn(uint8_t note, uint8_t vel) {
    sustainedNotes_.erase(note);

    noteQueue_.push({
        NoteEventType::NoteOn,
        static_cast<uint8_t>(note),
        vel / 127.0f,
        std::chrono::high_resolution_clock::now()
    });
}

// add note off event to noteQueue if no sustain active
void MidiController::noteOff(uint8_t note) {
    if(sustainDown_) {
        sustainedNotes_.insert(note);
        return;
    }
    noteQueue_.push({
        NoteEventType::NoteOff,
        static_cast<uint8_t>(note),
        0.0f,
        std::chrono::high_resolution_clock::now()
    });
}

// if sustain goes from on->off, then noteOff all the active ntoes
void MidiController::handleSustain(bool down) {
    if(down == sustainDown_) return;

    sustainDown_ = down;

    if(!sustainDown_) {
        for(uint8_t note : sustainedNotes_) {
            noteOff(note);
        }
        sustainedNotes_.clear();
    }
}
