#pragma once

#include <QObject>

// AppController is the bridge from the QML logic to the C++ app
class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sliderValue READ sliderValue WRITE setSliderValue NOTIFY sliderValueChanged)
    Q_PROPERTY(int sliderMin READ sliderMin WRITE setSliderMin NOTIFY sliderMinChanged)
    Q_PROPERTY(int sliderMax READ sliderMax WRITE setSliderMax NOTIFY sliderMaxChanged)

public:
    explicit AppController(QObject *parent = nullptr);

    int sliderValue(int val) { return sliderValue_; }
    int sliderMin(int val) { return sliderMin_; }
    int sliderMax(int val) { return sliderMax_; }

    void setSliderValue();
    void setSliderMin();
    void setSliderMax();

private:
    
    int sliderValue_;
    int sliderMin_;
    int sliderMax_;

};
