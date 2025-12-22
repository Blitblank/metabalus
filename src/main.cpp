
#include <QApplication>
#include "MainWindow.h"

#include <iostream>

int main(int argc, char *argv[]) {

    // std::cout << "Main()" << std::endl;

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
