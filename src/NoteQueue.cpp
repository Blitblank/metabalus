
#include "NoteQueue.h"
#include <iostream>

// add event to noteQueue, called by MidiController or keyboardController
bool NoteQueue::push(const NoteEvent& event) {
    size_t head = head_.load(std::memory_order_relaxed);
    size_t next = (head + 1) % SYNTH_NOTE_QUEUE_SIZE;

    if(next == tail_.load(std::memory_order_relaxed)) return false; // full

    buffer_[head] = event;
    head_.store(next, std::memory_order_relaxed);

    return true;
}

// take event from noteQueue, called by synth
bool NoteQueue::pop(NoteEvent& event) {
    size_t tail = tail_.load(std::memory_order_relaxed);

    if(tail == head_.load(std::memory_order_acquire)) return false; // empty

    event = buffer_[tail];
    tail_.store((tail + 1) % SYNTH_NOTE_QUEUE_SIZE, std::memory_order_release);

    return true;
}
