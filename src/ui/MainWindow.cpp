
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SmartSlider/SmartSlider.h"
#include "../ParameterStore.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    audio_(new AudioEngine()),
    keyboard_(audio_->noteQueue()) {

    // Initialize UI
    ui_->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    // Connect buttons to slots
    connect(ui_->buttonReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    // synth business
    audio_->start();

    connect(ui_->envelopeOsc1Volume, &EnvelopeGenerator::envelopeChanged,
        this, [this](float a, float d, float s, float r) {
            audio_->parameters()->set(ParamId::Osc1VolumeEnvA, a);
            audio_->parameters()->set(ParamId::Osc1VolumeEnvD, d);
            audio_->parameters()->set(ParamId::Osc1VolumeEnvS, s);
            audio_->parameters()->set(ParamId::Osc1VolumeEnvR, r);
            // TODO: parameters()->setEnv(ENV_ID, a, d, s ,r)
        });

    // initialize to defaults
    ui_->envelopeOsc1Volume->setRanges(
        PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].max,
        PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].max,
        PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].max,
        PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].max
    );
    ui_->envelopeOsc1Volume->setAttack(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].def);
    ui_->envelopeOsc1Volume->setDecay(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].def);
    ui_->envelopeOsc1Volume->setSustain(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].def);
    ui_->envelopeOsc1Volume->setRelease(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].def);

    // TODO: assign envelope widget a ParamID so that intialization and parameter bindings can be done from the envelope controller
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
    ui_->envelopeOsc1Volume->setRanges(
        PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].max,
        PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].max,
        PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].max,
        PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].max
    );
    ui_->envelopeOsc1Volume->setAttack(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvA)].def);
    ui_->envelopeOsc1Volume->setDecay(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvD)].def);
    ui_->envelopeOsc1Volume->setSustain(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvS)].def);
    ui_->envelopeOsc1Volume->setRelease(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1VolumeEnvR)].def);
}
