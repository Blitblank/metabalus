
#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <chrono>

#define SYNTH_NOTE_QUEUE_SIZE 128

enum class NoteEventType {
    NoteOn,
    NoteOff
};

struct NoteEvent {
    NoteEventType type; // noteOn or noteOff
    uint8_t note; // 0-128, a keyboard goes 0-87
    float velocity; // 0-1, from a midi instrument its 0-127 though
    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
};

// the queue is to keep track of note events from the UI/input thread to the audio engine thread
class NoteQueue {

public:
    NoteQueue() = default;
    ~NoteQueue() = default;

    bool push(const NoteEvent& event);
    bool pop(NoteEvent& event);

private:
    
    std::array<NoteEvent, SYNTH_NOTE_QUEUE_SIZE> buffer_;
    std::atomic<size_t> head_{ 0 };
    std::atomic<size_t> tail_{ 0 };

};
