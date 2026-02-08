
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
    ui_->comboOsc1WaveSelector1->clear();
    ui_->comboOsc1WaveSelector2->clear();
    for(std::filesystem::directory_entry entry : std::filesystem::directory_iterator("config/wavetables")) {
        if(std::filesystem::is_regular_file(entry.status())) {
            std::string fileName = entry.path().string().substr(18);
            fileName.erase(fileName.length() - 3);
            ui_->comboOsc1WaveSelector1->addItem(QString::fromStdString(fileName));
            ui_->comboOsc1WaveSelector2->addItem(QString::fromStdString(fileName));
        }
    }
    

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
    //midi_.openDefaultPort(); // TODO: error check
    midi_.openPort(1);
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
    YAML::Node masterNode = configRoot["MasterPitchOffset"];
    YAML::Node osc1Node = configRoot["Osc1PitchOffset"];
    YAML::Node osc2Node = configRoot["Osc2PitchOffset"];
    YAML::Node osc3Node = configRoot["Osc3PitchOffset"];
    ui_->sliderMasterOctave->setResolution(masterNode["Octave"][2].as<int>() - masterNode["Octave"][1].as<int>());
    ui_->sliderMasterOctave->setRange(masterNode["Octave"][1].as<int>(), masterNode["Octave"][2].as<int>());
    ui_->sliderMasterOctave->setValue(masterNode["Octave"][0].as<int>());

    ui_->sliderMasterSemitone->setResolution(masterNode["Semitone"][2].as<int>() - masterNode["Semitone"][1].as<int>());
    ui_->sliderMasterSemitone->setRange(masterNode["Semitone"][1].as<int>(), masterNode["Semitone"][2].as<int>());
    ui_->sliderMasterSemitone->setValue(masterNode["Semitone"][0].as<int>());

    ui_->sliderMasterPitch->setRange(masterNode["Pitch"][1].as<float>(), masterNode["Pitch"][2].as<float>());
    ui_->sliderMasterPitch->setValue(masterNode["Pitch"][0].as<float>());

    ui_->sliderOsc1Octave->setResolution(osc1Node["Octave"][2].as<int>() - osc1Node["Octave"][1].as<int>());
    ui_->sliderOsc1Octave->setRange(osc1Node["Octave"][1].as<int>(), osc1Node["Octave"][2].as<int>());
    ui_->sliderOsc1Octave->setValue(osc1Node["Octave"][0].as<int>());

    ui_->sliderOsc1Semitone->setResolution(osc1Node["Semitone"][2].as<int>() - osc1Node["Semitone"][1].as<int>());
    ui_->sliderOsc1Semitone->setRange(osc1Node["Semitone"][1].as<int>(), osc1Node["Semitone"][2].as<int>());
    ui_->sliderOsc1Semitone->setValue(osc1Node["Semitone"][0].as<int>());

    ui_->sliderOsc1Pitch->setRange(osc1Node["Pitch"][1].as<float>(), osc1Node["Pitch"][2].as<float>());
    ui_->sliderOsc1Pitch->setValue(osc1Node["Pitch"][0].as<float>());

    ui_->sliderOsc2Octave->setResolution(osc2Node["Octave"][2].as<int>() - osc2Node["Octave"][1].as<int>());
    ui_->sliderOsc2Octave->setRange(osc2Node["Octave"][1].as<int>(), osc2Node["Octave"][2].as<int>());
    ui_->sliderOsc2Octave->setValue(osc2Node["Octave"][0].as<int>());

    ui_->sliderOsc2Semitone->setResolution(osc2Node["Semitone"][2].as<int>() - osc2Node["Semitone"][1].as<int>());
    ui_->sliderOsc2Semitone->setRange(osc2Node["Semitone"][1].as<int>(), osc2Node["Semitone"][2].as<int>());
    ui_->sliderOsc2Semitone->setValue(osc2Node["Semitone"][0].as<int>());

    ui_->sliderOsc2Pitch->setRange(osc2Node["Pitch"][1].as<float>(), osc2Node["Pitch"][2].as<float>());
    ui_->sliderOsc2Pitch->setValue(osc2Node["Pitch"][0].as<float>());

    ui_->sliderOsc3Octave->setResolution(osc3Node["Octave"][2].as<int>() - osc3Node["Octave"][1].as<int>());
    ui_->sliderOsc3Octave->setRange(osc3Node["Octave"][1].as<int>(), osc3Node["Octave"][2].as<int>());
    ui_->sliderOsc3Octave->setValue(osc3Node["Octave"][0].as<int>());

    ui_->sliderOsc3Semitone->setResolution(osc3Node["Semitone"][2].as<int>() - osc3Node["Semitone"][1].as<int>());
    ui_->sliderOsc3Semitone->setRange(osc3Node["Semitone"][1].as<int>(), osc3Node["Semitone"][2].as<int>());
    ui_->sliderOsc3Semitone->setValue(osc3Node["Semitone"][0].as<int>());

    ui_->sliderOsc3Pitch->setRange(osc3Node["Pitch"][1].as<float>(), osc3Node["Pitch"][2].as<float>());
    ui_->sliderOsc3Pitch->setValue(osc3Node["Pitch"][0].as<float>());

    ui_->comboOsc1WaveSelector1->setCurrentIndex(configRoot["OscWaveSelector1"].as<int>());
    ui_->comboOsc1WaveSelector2->setCurrentIndex(configRoot["OscWaveSelector2"].as<int>());

}
