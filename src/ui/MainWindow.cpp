
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SmartSlider/SmartSlider.h"
#include "../ParameterStore.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    config_(ConfigInterface(&params_)),
    audio_(new AudioEngine(&config_, &params_)),
    keyboard_(audio_->noteQueue(), &config_),
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

    // rogue sliders, TODO: clean these up in a package
    connect(ui_->sliderMasterOctave, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::MasterOctaveOffset, value);
        ui_->sliderMasterOctave->setResolution();
    });
    connect(ui_->sliderMasterSemitone, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::MasterSemitoneOffset, value);
        ui_->sliderMasterSemitone->setResolution();
    });
    connect(ui_->sliderMasterPitch, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::MasterPitchOffset, value);
    });

    connect(ui_->sliderOsc1Octave, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc1OctaveOffset, value);
        ui_->sliderOsc1Octave->setResolution();
    });
    connect(ui_->sliderOsc1Semitone, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc1SemitoneOffset, value);
        ui_->sliderOsc1Semitone->setResolution();
    });
    connect(ui_->sliderOsc1Pitch, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc1PitchOffset, value);
    });

    connect(ui_->sliderOsc2Octave, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc2OctaveOffset, value);
        ui_->sliderOsc2Octave->setResolution();
    });
    connect(ui_->sliderOsc2Semitone, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc2SemitoneOffset, value);
        ui_->sliderOsc2Semitone->setResolution();
    });
    connect(ui_->sliderOsc2Pitch, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc2PitchOffset, value);
    });

    connect(ui_->sliderOsc3Octave, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc3OctaveOffset, value);
        ui_->sliderOsc3Octave->setResolution();
    });
    connect(ui_->sliderOsc3Semitone, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc3SemitoneOffset, value);
        ui_->sliderOsc3Semitone->setResolution();
    });
    connect(ui_->sliderOsc3Pitch, &SmartSlider::valueChanged,
    this, [this](float value) {
        audio_->parameters()->set(ParamId::Osc3PitchOffset, value);
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

    YAML::Node configRoot = config_.loadProfile("default");

    // update ui from the paramstore
    ui_->envelopeOsc1Volume->init(EnvelopeId::Osc1Volume, config_.loadEnvProfile("default", "Osc1Volume"));
    ui_->envelopeFilterCutoff->init(EnvelopeId::FilterCutoff, config_.loadEnvProfile("default", "FilterCutoff"));
    ui_->envelopeFilterResonance->init(EnvelopeId::FilterResonance, config_.loadEnvProfile("default", "FilterResonance"));

    // TODO: clean these up, maybe put them in a package like the envelope generators (it'll help encapsulate the int-snapping business)
    // what I might do is make a variable-length slider-package object
    ui_->sliderMasterOctave->setResolution(configRoot["MasterOctaveOffset"][2].as<int>() - configRoot["MasterOctaveOffset"][1].as<int>());
    ui_->sliderMasterOctave->setRange(configRoot["MasterOctaveOffset"][1].as<int>(), configRoot["MasterOctaveOffset"][2].as<int>());
    ui_->sliderMasterOctave->setValue(configRoot["MasterOctaveOffset"][0].as<int>());

    ui_->sliderMasterSemitone->setResolution(configRoot["MasterSemitoneOffset"][2].as<int>() - configRoot["MasterSemitoneOffset"][1].as<int>());
    ui_->sliderMasterSemitone->setRange(configRoot["MasterSemitoneOffset"][1].as<int>(), configRoot["MasterSemitoneOffset"][2].as<int>());
    ui_->sliderMasterSemitone->setValue(configRoot["MasterSemitoneOffset"][0].as<int>());

    ui_->sliderMasterPitch->setRange(configRoot["MasterPitchOffset"][1].as<float>(), configRoot["MasterPitchOffset"][2].as<float>());
    ui_->sliderMasterPitch->setValue(configRoot["MasterPitchOffset"][0].as<float>());

    ui_->sliderOsc1Octave->setResolution(configRoot["Osc1OctaveOffset"][2].as<int>() - configRoot["Osc1OctaveOffset"][1].as<int>());
    ui_->sliderOsc1Octave->setRange(configRoot["Osc1OctaveOffset"][1].as<int>(), configRoot["Osc1OctaveOffset"][2].as<int>());
    ui_->sliderOsc1Octave->setValue(configRoot["Osc1OctaveOffset"][0].as<int>());

    ui_->sliderOsc1Semitone->setResolution(configRoot["Osc1SemitoneOffset"][2].as<int>() - configRoot["Osc1SemitoneOffset"][1].as<int>());
    ui_->sliderOsc1Semitone->setRange(configRoot["Osc1SemitoneOffset"][1].as<int>(), configRoot["Osc1SemitoneOffset"][2].as<int>());
    ui_->sliderOsc1Semitone->setValue(configRoot["Osc1SemitoneOffset"][0].as<int>());

    ui_->sliderOsc1Pitch->setRange(configRoot["Osc1PitchOffset"][1].as<float>(), configRoot["Osc1PitchOffset"][2].as<float>());
    ui_->sliderOsc1Pitch->setValue(configRoot["Osc1PitchOffset"][0].as<float>());

    ui_->sliderOsc2Octave->setResolution(configRoot["Osc2OctaveOffset"][2].as<int>() - configRoot["Osc2OctaveOffset"][1].as<int>());
    ui_->sliderOsc2Octave->setRange(configRoot["Osc2OctaveOffset"][1].as<int>(), configRoot["Osc2OctaveOffset"][2].as<int>());
    ui_->sliderOsc2Octave->setValue(configRoot["Osc2OctaveOffset"][0].as<int>());

    ui_->sliderOsc2Semitone->setResolution(configRoot["Osc2SemitoneOffset"][2].as<int>() - configRoot["Osc2SemitoneOffset"][1].as<int>());
    ui_->sliderOsc2Semitone->setRange(configRoot["Osc2SemitoneOffset"][1].as<int>(), configRoot["Osc2SemitoneOffset"][2].as<int>());
    ui_->sliderOsc2Semitone->setValue(configRoot["Osc2SemitoneOffset"][0].as<int>());

    ui_->sliderOsc2Pitch->setRange(configRoot["Osc2PitchOffset"][1].as<float>(), configRoot["Osc2PitchOffset"][2].as<float>());
    ui_->sliderOsc2Pitch->setValue(configRoot["Osc2PitchOffset"][0].as<float>());

    ui_->sliderOsc3Octave->setResolution(configRoot["Osc3OctaveOffset"][2].as<int>() - configRoot["Osc3OctaveOffset"][1].as<int>());
    ui_->sliderOsc3Octave->setRange(configRoot["Osc3OctaveOffset"][1].as<int>(), configRoot["Osc3OctaveOffset"][2].as<int>());
    ui_->sliderOsc3Octave->setValue(configRoot["Osc3OctaveOffset"][0].as<int>());

    ui_->sliderOsc3Semitone->setResolution(configRoot["Osc3SemitoneOffset"][2].as<int>() - configRoot["Osc3SemitoneOffset"][1].as<int>());
    ui_->sliderOsc3Semitone->setRange(configRoot["Osc3SemitoneOffset"][1].as<int>(), configRoot["Osc3SemitoneOffset"][2].as<int>());
    ui_->sliderOsc3Semitone->setValue(configRoot["Osc3SemitoneOffset"][0].as<int>());

    ui_->sliderOsc3Pitch->setRange(configRoot["Osc3PitchOffset"][1].as<float>(), configRoot["Osc3PitchOffset"][2].as<float>());
    ui_->sliderOsc3Pitch->setValue(configRoot["Osc3PitchOffset"][0].as<float>());
    
    ui_->comboOsc1WaveSelector1->setCurrentIndex(configRoot["OscWaveSelector1"].as<int>());
    ui_->comboOsc1WaveSelector2->setCurrentIndex(configRoot["OscWaveSelector2"].as<int>());

}
