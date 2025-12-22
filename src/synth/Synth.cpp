
#include "Synth.h"

#include <QMediaDevices>
#include <QtMath>
#include <iostream>

Synth::Synth(QObject *parent) : QObject(parent) {

    format_.setSampleRate(44100);
    format_.setChannelCount(1);
    format_.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice device = QMediaDevices::defaultAudioOutput();

    if (!device.isFormatSupported(format_)) {
        format_ = device.preferredFormat();
    }

    audioSink_ = new QAudioSink(device, format_, this);
    audioSink_->setBufferSize(4096);
}

Synth::~Synth() {

    stop();
}

void Synth::start() {

    /*
    // std::cout << "Synth::start()" << std::endl;
    if (audioSink_->state() == QAudio::ActiveState)
        return;

    audioDevice_ = audioSink_->start();
    */
}

void Synth::stop() {

    if (audioSink_) {
        audioSink_->stop();
        audioDevice_ = nullptr;
    }
}

void Synth::setFrequency(float frequency) {

    frequency_ = qMax(1.0f, frequency);
}

QByteArray Synth::generateSamples(qint64 bytes) {

    QByteArray buffer(bytes, Qt::Uninitialized);

    const int channels = format_.channelCount();
    const int sampleRate = format_.sampleRate();
    //const float phaseInc = 2.0f * M_PI * frequency_ / sampleRate;
    freq += 1.0f;
    const float phaseInc = 2.0f * M_PI * freq / sampleRate;

    if (format_.sampleFormat() == QAudioFormat::Int16) {
        int16_t* out = reinterpret_cast<int16_t*>(buffer.data());
        int frames = bytes / (sizeof(int16_t) * channels);

        for (int i = 0; i < frames; ++i) {
            int16_t s = static_cast<int16_t>(32767 * std::sin(phase_));
            for (int c = 0; c < channels; ++c)
                *out++ = s;
            phase_ += phaseInc;
        }
    }
    else if (format_.sampleFormat() == QAudioFormat::Float) {
        float* out = reinterpret_cast<float*>(buffer.data());
        int frames = bytes / (sizeof(float) * channels);

        for (int i = 0; i < frames; ++i) {
            float s = std::sin(phase_);
            for (int c = 0; c < channels; ++c)
                *out++ = s;
            phase_ += phaseInc;
        }
    }

    return buffer;
}

void Synth::applyConfig(const AudioConfig& config) {

    // map struct values to the QAudioFormat
    QAudioFormat format;
    format.setSampleRate(config.sampleRate);
    format.setChannelCount(config.channelCount);
    format.setSampleFormat(config.sampleFormat);

    // must create a new device
    QAudioDevice device = QMediaDevices::defaultAudioOutput();

    if (!device.isFormatSupported(format)) {
        std::cout << "Requested format not supported, using preferred format" << std::endl;
        format = device.preferredFormat();
    }

    format_ = format;

    // and must create a new audioSink
    delete audioSink_;
    audioSink_ = new QAudioSink(device, format_, this);

    audioSink_->setBufferSize(config.bufferSize);
}