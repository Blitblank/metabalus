
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SmartSlider/SmartSlider.h"
#include "../ParameterStore.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    audio_(new AudioEngine()),
    keyboard_(audio_->noteQueue()) {

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
        this, [this](float a, float d, float s, float r) {
            audio_->parameters()->set(EnvelopeId::Osc1Volume, a, d, s, r);
        });
    connect(ui_->envelopeFilterCutoff, &EnvelopeGenerator::envelopeChanged,
        this, [this](float a, float d, float s, float r) {
            audio_->parameters()->set(EnvelopeId::FilterCutoff, a, d, s, r);
        });
    connect(ui_->envelopeFilterResonance, &EnvelopeGenerator::envelopeChanged,
        this, [this](float a, float d, float s, float r) {
            audio_->parameters()->set(EnvelopeId::FilterResonance, a, d, s, r);
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

    connect(ui_->sliderOsc1Volume, &SmartSlider::valueChanged, this, [this](float v) {
            audio_->parameters()->set(ParamId::Osc1Volume, v);
        });
    connect(ui_->sliderFilterCutoff, &SmartSlider::valueChanged, this, [this](float v) {
            audio_->parameters()->set(ParamId::FilterCutoff, v);
        });
    connect(ui_->sliderFilterResonance, &SmartSlider::valueChanged, this, [this](float v) {
            audio_->parameters()->set(ParamId::FilterResonance, v);
        });

    // synth business
    audio_->start();

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

    // envelopeGenerators
    ui_->envelopeOsc1Volume->init(EnvelopeId::Osc1Volume);
    ui_->envelopeFilterCutoff->init(EnvelopeId::FilterCutoff);
    ui_->envelopeFilterResonance->init(EnvelopeId::FilterResonance);

    // comboBoxes
    ui_->comboOsc1WaveSelector1->setCurrentIndex(static_cast<int>(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1WaveSelector1)].def));
    ui_->comboOsc1WaveSelector2->setCurrentIndex(static_cast<int>(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1WaveSelector2)].def));

    // misc sliders
    ui_->sliderOsc1Volume->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Volume)].min, PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Volume)].max);
    ui_->sliderFilterCutoff->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::FilterCutoff)].min, PARAM_DEFS[static_cast<size_t>(ParamId::FilterCutoff)].max);
    ui_->sliderFilterResonance->setRange(PARAM_DEFS[static_cast<size_t>(ParamId::FilterResonance)].min, PARAM_DEFS[static_cast<size_t>(ParamId::FilterResonance)].max);
    ui_->sliderOsc1Volume->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Volume)].def);
    ui_->sliderFilterCutoff->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::FilterCutoff)].def);
    ui_->sliderFilterResonance->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::FilterResonance)].def);
}
