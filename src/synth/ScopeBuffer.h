
#pragma once

#include <vector>
#include <atomic>

class ScopeBuffer {
public:

    explicit ScopeBuffer(size_t size);
    ~ScopeBuffer() = default;

    // add/read from the buffer
    void push(float sample);
    void read(std::vector<float>& out) const;

    // setters/getters
    void setTrigger(int32_t trigger) { trigger_ = trigger; }
    void setWavelength(int32_t wavelength) { wavelength_ = wavelength; }
    int32_t trigger() { return trigger_; }
    int32_t wavelength() { return wavelength_; }

    // NOTE: there are limits to the wavelengths that the scope can show cleanly due to the size of the audio buffer
    // at a buffer size of 256 at 44100hz the min visible steady frequency is ~172hz
private:

    std::vector<float> buffer_;
    std::atomic<size_t> writeIndex_{0};

    int32_t trigger_ = 0; // units in array indices
    int32_t wavelength_ = 400;

};
