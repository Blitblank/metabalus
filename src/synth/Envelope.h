
#pragma once

#include <cstdint>
#include <algorithm>

enum class State {
    Idle,
    Attack,
    Sustain,
    Decay,
    Release
};

class Envelope {

public:

    Envelope();
    ~Envelope() = default;

    // setters
    void setSampleRate(float sampleRate) { sampleRate_ = sampleRate; }
    void set(float a, float d, float s, float r) { setAttack(a); setDecay(d); setSustain(s); setRelease(r); }
    void setAttack(float seconds) { attack_ = std::max(seconds, 0.0001f); }
    void setDecay(float seconds) { decay_ = std::max(seconds, 0.0001f); }
    void setSustain(float level) { sustain_ = level; }
    void setRelease(float seconds) { release_ = std::max(seconds, 0.0001f); }
    // values close to zero introduce that popping sound on noteOn/noteOffs

    // note events
    void noteOn();
    void noteOff();

    // calculates and returns envelope level. must only be called once per sample
    float process(); 

    // determine if a note is playing or not
    bool isActive() const { return state_ != State::Idle; } 

private:
    
    State state_ = State::Idle;

    float sampleRate_ = 44100.0f;

    float attack_ = 0.05f; // seconds
    float decay_ = 0.2f; // seconds
    float sustain_ = 0.7f; // level
    float release_ = 0.2f; // seconds

    float value_ = 0.0f;

};

