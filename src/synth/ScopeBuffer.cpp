
#include "ScopeBuffer.h"

ScopeBuffer::ScopeBuffer(size_t size) : buffer_(size) {

}

void ScopeBuffer::push(float sample) {
    size_t w = writeIndex_.fetch_add(1, std::memory_order_relaxed);
    buffer_[w % buffer_.size()] = sample;
}

void ScopeBuffer::read(std::vector<float>& out) const {
    size_t w = writeIndex_.load(std::memory_order_relaxed);
    for (size_t i = 0; i < out.size(); i++) {
        size_t idx = (w + trigger_ + i * wavelength_ / out.size()) % buffer_.size();
        out[i] = buffer_[idx];
    }
}
