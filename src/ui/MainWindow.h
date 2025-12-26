
#pragma once

#include <QMainWindow>
#include <QKeyEvent>

#include "../synth/AudioEngine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void onIncrementClicked();
    void onResetClicked();

private:
    Ui::MainWindow *ui_;
    int counter_ = 0;

    int value = 0;

    void updateCounterLabel();

    AudioEngine* audio_ = nullptr;
    KeyboardController keyboard_;

};
