
#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SmartSlider; }
QT_END_NAMESPACE

class SmartSlider : public QWidget {
    Q_OBJECT

public:
    explicit SmartSlider(QWidget* parent = nullptr);
    ~SmartSlider() = default;

    void setRange(float min, float max);
    void setStep(float step);
    void setValue(float value);

    void value() const;

signals:
    void valueChanged(float value);

private slots:
    void onSliderChanged(int v);
    void onSpinValueChanged(float v);
    void onRangeChanged();

private:

    Ui::SmartSlider* ui_;

    int sliderResolution_ = 10000;

};