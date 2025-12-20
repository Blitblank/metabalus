#pragma once

#include <QMainWindow>

class QLabel;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
    explicit MainWindow(QWidget *parent = nullptr);

    private slots:
    void incrementCounter();

    private:
    int counter_ = 0;
    QLabel *label_;
    QPushButton *button_;
};