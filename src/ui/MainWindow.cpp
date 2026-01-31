
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SmartSlider/SmartSlider.h"
#include "../ParameterStore.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    config_(ConfigInterface(&params_)),
    audio_(new AudioEngine(&config_, &params_)),
    keyboard_(audio_->noteQueue()),
    midi_(audio_->noteQueue()) {

    // initialize ui
    ui_->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    // connect scope
    ui_->scope->setScopeBuffer(&audio_->scopeBuffer());

    // Connect buttons to slots
    connect(ui_->buttonReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    onResetClicked(); // manually reset

    // connect envelopeGenerators
    connect(ui_->envelopeOsc1Volume, &EnvelopeGenerator::envelopeChanged,
        this, [this](float depth, float a, float d, float s, float r) {
            audio_->parameters()->set(EnvelopeId::Osc1Volume, depth, a, d, s, r);
        });
    connect(ui_->envelopeFilterCutoff, &EnvelopeGenerator::envelopeChanged,
        this, [this](float depth, float a, float d, float s, float r) {
            audio_->parameters()->set(EnvelopeId::FilterCutoff, depth, a, d, s, r);
        });
    connect(ui_->envelopeFilterResonance, &EnvelopeGenerator::envelopeChanged,
        this, [this](float depth, float a, float d, float s, float r) {
            audio_->parameters()->set(EnvelopeId::FilterResonance, depth, a, d, s, r);
        });
    // this should be easy enough to put into a for each envelopeGenerator loop, each ui element just needs an EnvelopeId specifiers

    // other ui elements
    connect(ui_->comboOsc1WaveSelector1, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int index) {
            audio_->parameters()->set(ParamId::Osc1WaveSelector1, index);
        });
    connect(ui_->comboOsc1WaveSelector2, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int index) {
            audio_->parameters()->set(ParamId::Osc1WaveSelector2, index);
        });

    // synth business
    audio_->start();

    // midi
#ifndef _WIN32
    midi_.openPort(1); // TODO: error check
#endif

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

    config_.loadProfile("default");

    // update ui from the paramstore
    ui_->envelopeOsc1Volume->init(EnvelopeId::Osc1Volume, config_.loadEnvProfile("default", "Osc1Volume"));
    ui_->envelopeFilterCutoff->init(EnvelopeId::FilterCutoff, config_.loadEnvProfile("default", "FilterCutoff"));
    ui_->envelopeFilterResonance->init(EnvelopeId::FilterResonance, config_.loadEnvProfile("default", "FilterResonance"));

    ui_->comboOsc1WaveSelector1->setCurrentIndex(static_cast<int>(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1WaveSelector1)].def));
    ui_->comboOsc1WaveSelector2->setCurrentIndex(static_cast<int>(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1WaveSelector2)].def));

}
