
#pragma once

#include "../ParameterStore.h"
#include "../NoteQueue.h"
#include "Envelope.h"
#include "ScopeBuffer.h"
#include "Filter.h"
#include "Voice.h"

#include <vector>
#include <atomic>

class Synth {

public:
    Synth(const ParameterStore& params);
    ~Synth() = default;

    // generates a buffer of audio samples nFrames long
    void process(float* out, uint32_t nFrames, uint32_t sampleRate);

    // handles note events
    void handleNoteEvent(const NoteEvent& event);

    // setters
    void setSampleRate(uint32_t sampleRate);
    void setScopeBuffer(ScopeBuffer* scope) { scope_ = scope; }

private:

    // update internal params to the paramStore. not an immediate sync, its buffered for smoothing 
    void updateParams(); // (might remove the smoothing later)

    // small getter that abstracts all the static casts and such
    inline float getParam(ParamId);

    Voice* findFreeVoice();
    Voice* findVoiceByNote(uint8_t note);

    const ParameterStore& paramStore_;
    // smoothed params creates a buffer in case the thread controlling paramStore gets blocked
    std::array<SmoothedParam, PARAM_COUNT> params_;

    std::vector<uint8_t> heldNotes_;

    // voices
    static constexpr int MAX_VOICES = 32;
    std::array<Voice, MAX_VOICES> voices_;
    uint32_t sampleRate_;

    // for the scope
    ScopeBuffer* scope_ = nullptr;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

};
