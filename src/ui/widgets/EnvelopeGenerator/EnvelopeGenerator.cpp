
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

void EnvelopeGenerator::setRanges(float minA, float maxA, float minD, float maxD, float minS, float maxS, float minR, float maxR) {
    ui_->sliderAttack->setRange(minA, maxA);
    ui_->sliderDecay->setRange(minD, maxD);
    ui_->sliderSustain->setRange(minS, maxS);
    ui_->sliderRelease->setRange(minR, maxR);
}

void EnvelopeGenerator::emitEnvelope() {
    emit envelopeChanged(
        attack(),
        decay(),
        sustain(),
        release()
    );
}