
#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SmartSlider; }
QT_END_NAMESPACE

// SmartSlider is the widget including a slider, min/max settings, and a value setting parameter
class SmartSlider : public QWidget {
    Q_OBJECT

public:
    explicit SmartSlider(QWidget* parent = nullptr);
    ~SmartSlider();

    // setters
    void setRange(float min, float max);
    void setValue(float value);

    // getters
    float value();

signals:
    void valueChanged(float value);

private slots:
    void onSliderChanged(int v);
    void onSpinValueChanged(float v);
    void onRangeChanged();

private:

    Ui::SmartSlider* ui_;

    // slider just does ints from 0 to this value
    // all the floating point business happens in here
    int sliderResolution_ = 10000;

    float min_;
    float max_;
    // I got rid of step because it just didn't work right and I really didn't need it

};