
#include "ParameterStore.h"

ParameterStore::ParameterStore() {
    resetToDefaults();
}

void ParameterStore::set(ParamId id, float value) {
    values_[static_cast<size_t>(id)].store(value, std::memory_order_relaxed);
}

void ParameterStore::set(EnvelopeId id, float depth, float a, float d, float s, float r) {

    EnvelopeParam params = ENV_PARAMS[static_cast<size_t>(id)];
    set(params.depth, depth);
    set(params.a, a);
    set(params.d, d);
    set(params.s, s);
    set(params.r, r);
}

float ParameterStore::get(ParamId id) const {
    return values_[static_cast<size_t>(id)].load(std::memory_order_relaxed);
}

void ParameterStore::resetToDefaults() {
    for(size_t i = 0; i < PARAM_COUNT; i++) {
        values_[i].store(PARAM_DEFS[i].def, std::memory_order_relaxed);
    }
}
