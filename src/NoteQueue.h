
#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <chrono>

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
    static constexpr size_t SIZE = 128;
    
    std::array<NoteEvent, SIZE> buffer_;
    std::atomic<size_t> head_{ 0 };
    std::atomic<size_t> tail_{ 0 };

};
