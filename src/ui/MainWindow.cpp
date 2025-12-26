
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

    // Connect buttons to slots
    connect(ui_->buttonReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    // synth business
    audio_->start();

    // slider business
    connect(ui_->sliderGainA, &SmartSlider::valueChanged, this, [this](float v) {
        audio_->parameters()->set(ParamId::Osc1VolumeEnvA, v); // bind valueChanged signal to the ParameterStore
    });
    connect(ui_->sliderGainD, &SmartSlider::valueChanged, this, [this](float v) {
        audio_->parameters()->set(ParamId::Osc1VolumeEnvD, v); // bind valueChanged signal to the ParameterStore
    });
    connect(ui_->sliderGainS, &SmartSlider::valueChanged, this, [this](float v) {
        audio_->parameters()->set(ParamId::Osc1VolumeEnvS, v); // bind valueChanged signal to the ParameterStore
    });
    connect(ui_->sliderGainR, &SmartSlider::valueChanged, this, [this](float v) {
        audio_->parameters()->set(ParamId::Osc1VolumeEnvR, v); // bind valueChanged signal to the ParameterStore
    });
    // initialize to defaults
    ui_->sliderGainA->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].max);
    ui_->sliderGainA->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].def);
    ui_->sliderGainD->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].max);
    ui_->sliderGainD->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].def);
    ui_->sliderGainS->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].max);
    ui_->sliderGainS->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].def);
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

void MainWindow::onResetClicked() {

    // initialize to defaults
    ui_->sliderGainA->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].max);
    ui_->sliderGainA->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].def);
    ui_->sliderGainD->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].max);
    ui_->sliderGainD->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].def);
    ui_->sliderGainS->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].max);
    ui_->sliderGainS->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].def);
    ui_->sliderGainR->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].max);
    ui_->sliderGainR->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].def);
}
