
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SmartSlider.h"
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

    // synth business
    audio_->start();

    // slider business
    connect(ui_->sliderGainR, &SmartSlider::valueChanged, this, [this](float v) {
        audio_->parameters()->set(ParamId::Osc1VolumeEnvR, v); // bind valueChanged signal to the ParameterStore
    });
    // initialize to defaults
    ui_->sliderGainR->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].max);
    ui_->sliderGainR->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].def);
    // TODO: package a smartSlider into an envelope controller widget
    // then intialization and parameter bindings can be done from the envelope controller
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
