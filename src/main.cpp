
#include <QApplication>

#include "ui/MainWindow.h"

#include <iostream>

int main(int argc, char *argv[]) {

    // std::cout << "Main()" << std::endl;

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    int status = app.exec();

    return status;
}
