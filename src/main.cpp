
#include <QApplication>

#include "ui/MainWindow.h"
#include "ConfigInterface.h"

#include <iostream>

int main(int argc, char *argv[]) {

    // std::cout << "Main()" << std::endl;
    
    QApplication app(argc, argv);

    MainWindow window; // entry point goes to MainWindow::MainWindow()
    window.show();
    
    int status = app.exec(); // app execution; blocks until window close

    return status;
}
