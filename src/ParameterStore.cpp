
#include "ParameterStore.h"

#include <iostream>
#include "yaml-cpp/yaml.h" // TODO: using yaml.h outside of ConfigInterface feels spaghetti to me
#include <filesystem>

ParameterStore::ParameterStore() {

}

// set parameter value
void ParameterStore::set(ParamId id, float value) {
    values_[static_cast<size_t>(id)].store(value, std::memory_order_relaxed);
}

// set a whole envelope of parameters
void ParameterStore::set(EnvelopeId id, float depth, float a, float d, float s, float r) {

    EnvelopeParam params = ENV_PARAMS[static_cast<size_t>(id)];
    set(params.depth, depth);
    set(params.a, a);
    set(params.d, d);
    set(params.s, s);
    set(params.r, r);
}

// get a single parameter
float ParameterStore::get(ParamId id) const {
    return values_[static_cast<size_t>(id)].load(std::memory_order_relaxed);
}
