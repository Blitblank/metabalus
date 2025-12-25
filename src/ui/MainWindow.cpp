
#include "MainWindow.h"

#include <QTimer>
#include "ui_MainWindow.h"

#include "../ParameterStore.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    audio_(new AudioEngine()),
    keyboard_(audio_->noteQueue()) {

    ui_->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    // Initialize UI
    updateCounterLabel();

    // Connect buttons to slots
    connect(ui_->buttonIncrement, &QPushButton::clicked, this, &MainWindow::onIncrementClicked);
    connect(ui_->buttonReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    // slider business
    // TODO: smart slider widget
    connect(ui_->slider,     &QSlider::valueChanged,      this, &MainWindow::onSliderValueChanged);
    connect(ui_->inputMin,   &QLineEdit::editingFinished, this, &MainWindow::onMinChanged);
    connect(ui_->inputMax,   &QLineEdit::editingFinished, this, &MainWindow::onMaxChanged);
    connect(ui_->inputStep,  &QLineEdit::editingFinished, this, &MainWindow::onStepChanged);
    connect(ui_->inputValue, &QLineEdit::editingFinished, this, &MainWindow::onValueChanged);

    // synth business
    audio_->start();

    // init defaults
    // TODO:: there's gotta be a better way
    ui_->slider->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].def);
    ui_->slider->setMinimum(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].min);
    ui_->slider->setMaximum(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].max);
    ui_->inputValue->setText(QString::number(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].def));
    ui_->inputMin->setText(QString::number(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].min));
    ui_->inputMax->setText(QString::number(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].max));
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    keyboard_.handleKeyPress(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    keyboard_.handleKeyRelease(event);
}

void MainWindow::onIncrementClicked() {
    counter_++;
    updateCounterLabel();
}

void MainWindow::onResetClicked() {
    counter_ = 0;
    updateCounterLabel();
}

void MainWindow::updateCounterLabel() {
    ui_->labelCounter->setText(QString::number(counter_));
}

// allows only numbers to be set
void MainWindow::onSliderValueChanged(int value) { 
    QSignalBlocker blocker(ui_->inputValue);
    ui_->inputValue->setText(QString::number(value));

    // forward value so synthesizer can read
    audio_->parameters()->set(ParamId::Osc1Frequency, static_cast<float>(value));
}

// allows only values within the min, max to be set by the text field
void MainWindow::onValueChanged() {
    bool ok = false;
    int value = ui_->inputValue->text().toInt(&ok);
    if(!ok) return;

    value = qBound(ui_->slider->minimum(), value, ui_->slider->maximum());
    ui_->slider->setValue(value);
}

void MainWindow::applySliderRange() {
    bool minOk, maxOk;
    int min = ui_->inputMin->text().toInt(&minOk);
    int max = ui_->inputMax->text().toInt(&maxOk);
    if(!minOk || !maxOk || min > max) return;
    
    ui_->slider->setRange(min, max);

    syncValueToUi(ui_->slider->value());
}

void MainWindow::applySliderStep() {
    bool ok;
    int step = ui_->inputStep->text().toInt(&ok);
    if(!ok || step <= 0) return;
    
    ui_->slider->setSingleStep(step);
    ui_->slider->setPageStep(step);
}

void MainWindow::syncValueToUi(int value) {
    QSignalBlocker block1(ui_->slider);
    QSignalBlocker block2(ui_->inputValue);

    value = qBound(ui_->slider->minimum(), value, ui_->slider->maximum());
    ui_->slider->setValue(value);
    ui_->inputValue->setText(QString::number(value));
}
