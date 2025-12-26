
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

    EnvelopeParam params = ENV_PARAMS[static_cast<size_t>(id)];

    ui_->sliderAttack->setRange(PARAM_DEFS[static_cast<size_t>(params.a)].min, PARAM_DEFS[static_cast<size_t>(params.a)].max);
    ui_->sliderDecay->setRange(PARAM_DEFS[static_cast<size_t>(params.d)].min, PARAM_DEFS[static_cast<size_t>(params.d)].max);
    ui_->sliderSustain->setRange(PARAM_DEFS[static_cast<size_t>(params.s)].min, PARAM_DEFS[static_cast<size_t>(params.s)].max);
    ui_->sliderRelease->setRange(PARAM_DEFS[static_cast<size_t>(params.r)].min, PARAM_DEFS[static_cast<size_t>(params.r)].max);

    setAttack(PARAM_DEFS[static_cast<size_t>(params.a)].def);
    setDecay(PARAM_DEFS[static_cast<size_t>(params.d)].def);
    setSustain(PARAM_DEFS[static_cast<size_t>(params.s)].def);
    setRelease(PARAM_DEFS[static_cast<size_t>(params.r)].def);

}
