
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

    // connect envelopeGenerator
    ui_->envelopeOsc1Volume->init(EnvelopeId::Osc1Volume);
    connect(ui_->envelopeOsc1Volume, &EnvelopeGenerator::envelopeChanged,
        this, [this](float a, float d, float s, float r) {
            audio_->parameters()->set(EnvelopeId::Osc1Volume, a, d, s, r);
        });
    // this should be easy enough to put into a for each envelopeGenerator loop 
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
    ui_->envelopeOsc1Volume->init(EnvelopeId::Osc1Volume);
}
