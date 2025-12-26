
#include "SmartSlider.h"
#include "ui_SmartSlider.h"

#include <iostream>

SmartSlider::SmartSlider(QWidget* parent) : QWidget(parent), ui_(new Ui::SmartSlider) {

    ui_->setupUi(this);

    ui_->slider->setMinimum(0);
    ui_->slider->setMaximum(sliderResolution_);

    // connect widgets to slots
    connect(ui_->slider, &QSlider::valueChanged, this, &SmartSlider::onSliderChanged);
    connect(ui_->spinValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SmartSlider::onSpinValueChanged);
    connect(ui_->spinMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SmartSlider::onRangeChanged);
    connect(ui_->spinMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SmartSlider::onRangeChanged);
}

SmartSlider::~SmartSlider() {
    delete ui_;
}

// sets range of the slider, updates the spinBoxes, and clamps output, called by other classes
// probably overengineered
void SmartSlider::setRange(float min, float max) {
    if (min >= max) return;

    min_ = min; max_ = max;

    ui_->slider->setRange(0, sliderResolution_);
    ui_->spinMin->setValue(min_);
    ui_->spinMax->setValue(max_);
    //ui_->spinValue->setRange(min_, max_);

    float value = ui_->spinValue->value();
    value = std::clamp(value, min_, max_);

    float t = (value - min_) / (max_ - min_);
    int32_t sliderValue = static_cast<int32_t>(t) * sliderResolution_;

    ui_->slider->setValue(sliderValue);
    ui_->spinValue->setValue(value);
}

// sets value of the slider and the spinBox, called by other classes
void SmartSlider::setValue(float value) {
    value = std::clamp(value, min_, max_);

    float t = (value - min_) / (max_ - min_);
    int32_t sliderValue = static_cast<int32_t>(t) * sliderResolution_;

    ui_->slider->setValue(sliderValue);
    ui_->spinValue->setValue(value);

    emit valueChanged(value);
}

void SmartSlider::onSliderChanged(int32_t v) {
    float min = ui_->spinMin->value();
    float max = ui_->spinMax->value();
    float value = min + (max - min) * static_cast<float>(v) / sliderResolution_;

    ui_->spinValue->setValue(value);

    emit valueChanged(value);
}

void SmartSlider::onSpinValueChanged(float v) {
    float min = ui_->spinMin->value();
    float max = ui_->spinMax->value();

    float norm = (v - min) / (max - min);
    int32_t sliderValue = qRound(norm * sliderResolution_);

    ui_->slider->setValue(sliderValue);

    // intentionally skips clamping by min/max
    emit valueChanged(v);
}

void SmartSlider::onRangeChanged() {
    float min = ui_->spinMin->value();
    float max = ui_->spinMax->value();

    onSpinValueChanged(ui_->spinValue->value());
}
