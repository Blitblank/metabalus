
#include "Envelope.h"

Envelope::Envelope() {

}

void Envelope::noteOn() {
    state_ = State::Attack;
    // there's interntionally no envelope value_ reset here because slurs
}

void Envelope::noteOff() {
    if(state_ != State::Idle) state_ = State::Release;
}

// returns current value based on state and steps forward one sample
float Envelope::process() {
    
    switch (state_) {
      case State::Idle:
        value_ = 0.0f;
        break;
      case State::Attack:
        value_ += 1.0f / (attack_ * sampleRate_);
        if(value_ >= 1.0f) {
            value_ = 1.0f;
            state_ = State::Decay;
        }
        break;
      case State::Decay: // TODO: noteOff during decay means release occurs longer than expected
        value_ -= (1.0f - sustain_) / (decay_ * sampleRate_);
        if(value_ <= sustain_) {
            value_ = sustain_;
            state_ = State::Sustain;
        }
        break;
      case State::Sustain:
        // wait until release
        break;
      case State::Release:
        value_ -= sustain_ / (release_ * sampleRate_);
        if(value_ <= 0.0f) {
            value_ = 0.0f;
            state_ = State::Idle;
        }
        break;
      default: // unreachable
        break;
    }

    return value_;
}
    