
#pragma once

#include <QIODevice>
#include "Synth.h"

class AudioStream : public QIODevice
{
    Q_OBJECT

public:
    explicit AudioStream(Synth *synth, QObject *parent = nullptr);

    void start();
    void stop();

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *, qint64) override { return 0; }

private:
    Synth *synth_;
};
