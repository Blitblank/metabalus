
#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class EnvelopeGenerator; }
QT_END_NAMESPACE

// this is for the widget, not the synth object named Envelope
class EnvelopeGenerator : public QWidget {
    Q_OBJECT
public:
    explicit EnvelopeGenerator(QWidget* parent = nullptr);
    ~EnvelopeGenerator();

    void setAttack(float v);
    void setDecay(float v);
    void setSustain(float v);
    void setRelease(float v);

    void setRanges(float minA, float maxA, float minD, float maxD, float minS, float maxS, float minR, float maxR);

    float attack() const;
    float decay() const;
    float sustain() const;
    float release() const;

signals:
    void envelopeChanged(double a, double d, double s, double r);

private:

    Ui::EnvelopeGenerator* ui_;

    void emitEnvelope();
};
