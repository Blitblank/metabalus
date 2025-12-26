
#include "EnvelopeGenerator.h"
#include "ui_EnvelopeGenerator.h"

#include <iostream>

EnvelopeGenerator::EnvelopeGenerator(QWidget* parent) : QWidget(parent), ui_(new Ui::EnvelopeGenerator) {

    ui_->setupUi(this);

    auto connectSlider = [this](SmartSlider* s) {
        connect(s, &SmartSlider::valueChanged,
                this, &EnvelopeGenerator::emitEnvelope);
    };

    connectSlider(ui_->sliderAttack);
    connectSlider(ui_->sliderDecay);
    connectSlider(ui_->sliderSustain);
    connectSlider(ui_->sliderRelease);
}

EnvelopeGenerator::~EnvelopeGenerator() {
    delete ui_;
}

// getters are here to separate ui from header
float EnvelopeGenerator::attack() const {
    return ui_->sliderAttack->value();
}

float EnvelopeGenerator::decay() const {
    return ui_->sliderDecay->value();
}

float EnvelopeGenerator::sustain() const {
    return ui_->sliderSustain->value();
}

float EnvelopeGenerator::release() const {
    return ui_->sliderRelease->value();
}

void EnvelopeGenerator::setAttack(float v) {
    ui_->sliderAttack->setValue(v);
}

void EnvelopeGenerator::setDecay(float v) {
    ui_->sliderDecay->setValue(v);
}

void EnvelopeGenerator::setSustain(float v) {
    ui_->sliderSustain->setValue(v);
}

void EnvelopeGenerator::setRelease(float v) {
    ui_->sliderRelease->setValue(v);
}

void EnvelopeGenerator::emitEnvelope() {
    emit envelopeChanged(
        attack(),
        decay(),
        sustain(),
        release()
    );
}

void EnvelopeGenerator::init(EnvelopeId id) {

    // could probably make this simpler with a map
    ParamId aId, dId, sId, rId;
    switch (id) {
    case EnvelopeId::Osc1Volume:
        aId = ParamId::Osc1VolumeEnvA; dId = ParamId::Osc1VolumeEnvD; sId = ParamId::Osc1VolumeEnvS; rId = ParamId::Osc1VolumeEnvR;
        break;
    case EnvelopeId::FilterCutoff:
        aId = ParamId::FilterCutoffEnvA; dId = ParamId::FilterCutoffEnvD; sId = ParamId::FilterCutoffEnvS; rId = ParamId::FilterCutoffEnvR;
        break;
    case EnvelopeId::FilterResonance:
        aId = ParamId::FilterResonanceEnvA; dId = ParamId::FilterResonanceEnvD; sId = ParamId::FilterResonanceEnvS; rId = ParamId::FilterResonanceEnvR;
        break;
    default: // not found
        break;
    }

    ui_->sliderAttack->setRange(PARAM_DEFS[static_cast<size_t>(aId)].min, PARAM_DEFS[static_cast<size_t>(aId)].max);
    ui_->sliderDecay->setRange(PARAM_DEFS[static_cast<size_t>(dId)].min, PARAM_DEFS[static_cast<size_t>(dId)].max);
    ui_->sliderSustain->setRange(PARAM_DEFS[static_cast<size_t>(sId)].min, PARAM_DEFS[static_cast<size_t>(sId)].max);
    ui_->sliderRelease->setRange(PARAM_DEFS[static_cast<size_t>(rId)].min, PARAM_DEFS[static_cast<size_t>(rId)].max);

    setAttack(PARAM_DEFS[static_cast<size_t>(aId)].def);
    setDecay(PARAM_DEFS[static_cast<size_t>(sId)].def);
    setSustain(PARAM_DEFS[static_cast<size_t>(sId)].def);
    setRelease(PARAM_DEFS[static_cast<size_t>(rId)].def);

}
