
cmake_minimum_required(VERSION 3.21)
project(metabolus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(RTAUDIO REQUIRED rtaudio)

qt_standard_project_setup()

add_executable(metabolus
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
    src/synth/AudioEngine.cpp
    src/synth/AudioEngine.h
    src/synth/Synth.cpp
    src/synth/Synth.h
    resources/resources.qrc
)

target_include_directories(metabolus PRIVATE ${RTAUDIO_INCLUDE_DIRS})
target_link_libraries(metabolus PRIVATE Qt6::Widgets ${RTAUDIO_LIBRARIES})
target_compile_options(metabolus PRIVATE ${RTAUDIO_CFLAGS_OTHER})
