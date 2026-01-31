
#pragma once

#include <QMainWindow>
#include <QKeyEvent>

#include "../ConfigInterface.h"
#include "../synth/AudioEngine.h"
#include "../MidiController.h"

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
    void onResetClicked();

private:

    Ui::MainWindow *ui_;

    ParameterStore params_;
    ConfigInterface config_;
    AudioEngine* audio_ = nullptr;
    KeyboardController keyboard_;
    MidiController midi_;

};
