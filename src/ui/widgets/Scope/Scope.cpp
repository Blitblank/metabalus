
#include "Scope.h"
#include "ui_Scope.h"

#include "../../../synth/ScopeBuffer.h"

#include <QPainter>
#include <iostream>

Scope::Scope(QWidget* parent) : QWidget(parent), ui_(new Ui::Scope), samples_(512) {
    
    timer_.setInterval(16); // ~60 hz
    connect(&timer_, &QTimer::timeout, this, QOverload<>::of(&Scope::update));
    timer_.start();
}

Scope::~Scope() {
    delete ui_;
}

void Scope::setScopeBuffer(ScopeBuffer* buffer) {
    buffer_ = buffer;
}

void Scope::paintEvent(QPaintEvent*) {
    if (!buffer_) return;

    int32_t wavelength = buffer_->wavelength();
    int32_t trigger = buffer_->trigger();

    buffer_->read(samples_);
    
    // auto scale amplitude. disabled because it hides the envelope effects
    float maxAmp = 1.0f;
    //for (float s : samples_) {
    //    maxAmp = std::max(maxAmp, std::abs(s));
    //}
    // TODO: if you get really bored you can start adding scope display options
        
    float scaleY = (height() * 0.45f) / maxAmp;
    float midY = height() / 2.0f;

    QPainter p(this);
    QColor gray(20, 20, 20);
    p.fillRect(rect(), gray);

    // got caught playing around
    QPen pen;
    pen.setWidthF(2.0f);
    QColor green(50, 255, 70);
    pen.setColor(green);
    p.setPen(pen);

    for (int32_t i = 1; i < samples_.size(); i++) {
        p.drawLine(
            (i) * width() / samples_.size(),
            midY - samples_[i - 1] * scaleY,
            i * width() / samples_.size(),
            midY - samples_[i] * scaleY
        );
    }
}