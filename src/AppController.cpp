
#include "AppController.h"

AppController::AppController(QObject *parent)
    : QObject(parent)
{
    m_status = "Ready";
}

void AppController::setSliderValue(int val) {

    if(sliderValue_ == val) return;
    sliderValue_ = val;
    emit sliderValueChanged();
}

void AppController::setSliderMin(int val) {

    if(sliderMin_ == val) return;
    sliderMin_ = val;
    emit sliderMinChanged();
}

void AppController::setSliderMax(int val) {

    if(sliderMax_ == val) return;
    sliderMax_ = val;
    emit sliderMaxChanged();
}
