
#include "MainWindow.h"

#include <QTimer>
#include "ui_MainWindow.h"

#include "ParameterStore.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    audio_(new AudioEngine()),
    keyboard_(audio_->noteQueue()) {

    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    // Initialize UI
    updateCounterLabel();

    // Connect buttons to slots
    connect(ui->buttonIncrement, &QPushButton::clicked, this, &MainWindow::onIncrementClicked);
    connect(ui->buttonReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    // slider business
    // TODO: smart slider widget
    connect(ui->slider,     &QSlider::valueChanged,      this, &MainWindow::onSliderValueChanged);
    connect(ui->inputMin,   &QLineEdit::editingFinished, this, &MainWindow::onMinChanged);
    connect(ui->inputMax,   &QLineEdit::editingFinished, this, &MainWindow::onMaxChanged);
    connect(ui->inputStep,  &QLineEdit::editingFinished, this, &MainWindow::onStepChanged);
    connect(ui->inputValue, &QLineEdit::editingFinished, this, &MainWindow::onValueChanged);

    // synth business
    audio_->start();

    // init defaults
    // TODO:: there's gotta be a better way
    ui->slider->setValue(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].def);
    ui->slider->setMinimum(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].min);
    ui->slider->setMaximum(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].max);
    ui->inputValue->setText(QString::number(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].def));
    ui->inputMin->setText(QString::number(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].min));
    ui->inputMax->setText(QString::number(PARAM_DEFS[static_cast<size_t>(ParamId::Osc1Frequency)].max));
}

MainWindow::~MainWindow() {
    delete ui;
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
    ui->labelCounter->setText(QString::number(counter_));
}

// allows only numbers to be set
void MainWindow::onSliderValueChanged(int value) { 
    QSignalBlocker blocker(ui->inputValue);
    ui->inputValue->setText(QString::number(value));

    // forward value so synthesizer can read
    audio_->parameters()->set(ParamId::Osc1Frequency, static_cast<float>(value));
}

// allows only values within the min, max to be set by the text field
void MainWindow::onValueChanged() {
    bool ok = false;
    int value = ui->inputValue->text().toInt(&ok);
    if(!ok) return;

    value = qBound(ui->slider->minimum(), value, ui->slider->maximum());
    ui->slider->setValue(value);
}

void MainWindow::applySliderRange() {
    bool minOk, maxOk;
    int min = ui->inputMin->text().toInt(&minOk);
    int max = ui->inputMax->text().toInt(&maxOk);
    if(!minOk || !maxOk || min > max) return;
    
    ui->slider->setRange(min, max);

    syncValueToUi(ui->slider->value());
}

void MainWindow::applySliderStep() {
    bool ok;
    int step = ui->inputStep->text().toInt(&ok);
    if(!ok || step <= 0) return;
    
    ui->slider->setSingleStep(step);
    ui->slider->setPageStep(step);
}

void MainWindow::syncValueToUi(int value) {
    QSignalBlocker block1(ui->slider);
    QSignalBlocker block2(ui->inputValue);

    value = qBound(ui->slider->minimum(), value, ui->slider->maximum());
    ui->slider->setValue(value);
    ui->inputValue->setText(QString::number(value));
}
