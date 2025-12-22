
#include "AudioStream.h"

#include <iostream>

AudioStream::AudioStream(Synth *synth, QObject *parent) : QIODevice(parent), synth_(synth) {

}

void AudioStream::start() {

    // std::cout << "AudioStream::start()" << std::endl;
    open(QIODevice::ReadOnly);
}

void AudioStream::stop() {

    close();
}

qint64 AudioStream::readData(char *data, qint64 maxlen) {

    // std::cout << "sample out " << std::endl;
    QByteArray samples = synth_->generateSamples(maxlen);
    memcpy(data, samples.constData(), samples.size());
    return samples.size();
}
