
#include "EnvelopeGenerator.h"
#include "ui_EnvelopeGenerator.h"

#include <iostream>

EnvelopeGenerator::EnvelopeGenerator(QWidget* parent) : QWidget(parent), ui_(new Ui::EnvelopeGenerator) {

    ui_->setupUi(this);

    auto connectSlider = [this](SmartSlider* s) {
        connect(s, &SmartSlider::valueChanged,
                this, &EnvelopeGenerator::emitEnvelope);
    };

    connectSlider(ui_->sliderDepth);
    connectSlider(ui_->sliderAttack);
    connectSlider(ui_->sliderDecay);
    connectSlider(ui_->sliderSustain);
    connectSlider(ui_->sliderRelease);
}

EnvelopeGenerator::~EnvelopeGenerator() {
    delete ui_;
}

// getters are here to separate ui from header
float EnvelopeGenerator::depth() const {
    return ui_->sliderDepth->value();
}

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

void EnvelopeGenerator::setDepth(float v) {
    ui_->sliderDepth->setValue(v);
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
        depth(),
        attack(),
        decay(),
        sustain(),
        release()
    );
}

void EnvelopeGenerator::init(EnvelopeId id, std::array<ParamDefault, 5> profile) {

    EnvelopeParam params = ENV_PARAMS[static_cast<size_t>(id)];

    ui_->sliderDepth->setRange(profile[0].min, profile[0].max);
    ui_->sliderAttack->setRange(profile[1].min, profile[1].max);
    ui_->sliderDecay->setRange(profile[2].min, profile[2].max);
    ui_->sliderSustain->setRange(profile[3].min, profile[3].max);
    ui_->sliderRelease->setRange(profile[4].min, profile[4].max);

    setDepth(profile[0].def);
    setAttack(profile[1].def);
    setDecay(profile[2].def);
    setSustain(profile[3].def);
    setRelease(profile[4].def);

}
