
#include "mainwindow.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    // probably this will be in a qml file
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    label_ = new QLabel("cases: 0", this);
    label_->setAlignment(Qt::AlignCenter);

    button_ = new QPushButton("many such cases !", this);

    layout->addWidget(label_);
    layout->addWidget(button_);

    setCentralWidget(central);
    setWindowTitle("moblus !!!");
    resize(400, 200);

    connect(button_, &QPushButton::clicked, this, &MainWindow::incrementCounter);
}

void MainWindow::incrementCounter() {
    counter_++;
    label_->setText(QString("cases: %1").arg(counter_));
}