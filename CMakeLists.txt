cmake_minimum_required(VERSION 3.22)

project(metabolus
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/mingw_64")

find_package(Qt6 6.10 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
)

qt_standard_project_setup()

qt_add_executable(metabolus
    src/main.cpp
    src/AppController.cpp
    src/AppController.h
)

qt_add_qml_module(metabolus
    URI MyApp
    VERSION 1.0

    QML_FILES
        qml/Main.qml
        qml/screens/MainScreen.qml
        qml/screens/MainScreenUI.ui.qml
)

target_link_libraries(metabolus
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
)
