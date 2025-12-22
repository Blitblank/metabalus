
#pragma once

#include <QObject>
#include <QAudioFormat>
#include <QAudioSink>
#include <QIODevice>

#include <atomic>

struct AudioConfig {
    int sampleRate = 44100;
    int channelCount = 1;
    QAudioFormat::SampleFormat sampleFormat = QAudioFormat::Int16;
    int bufferSize = 4096; // bytes
};

class Synth : public QObject {
    Q_OBJECT

public:
    explicit Synth(QObject *parent = nullptr);
    ~Synth();

    // audioSink is the media consumer for the audio data
    QAudioSink* audioSink() { return audioSink_; }

    // audio config setter/getter 
    void applyConfig(const AudioConfig& config);
    const QAudioFormat& format() const { return format_; }

    // synth commands
    void start();
    void stop();
    void setFrequency(float frequency);
    // bread and butter right here
    QByteArray generateSamples(qint64 bytes);

private:
    QAudioFormat format_;
    QAudioSink *audioSink_ = nullptr;
    QIODevice *audioDevice_ = nullptr;

    std::atomic<float> frequency_{440.0f};
    float phase_ = 0.0f;

    float freq = 400.0f;
};