cmake_minimum_required(VERSION 3.21)
project(metabolus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(metabolus
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
    resources/resources.qrc
)

target_link_libraries(metabolus
    PRIVATE Qt6::Widgets
)