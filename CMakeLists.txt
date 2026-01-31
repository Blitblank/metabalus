
cmake_minimum_required(VERSION 3.21)
project(metabolus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if (WIN32) # windows 11 x86_64

    add_library(RtAudio::rtaudio SHARED IMPORTED)
    set_target_properties(RtAudio::rtaudio PROPERTIES
        IMPORTED_LOCATION "${RtAudio_ROOT}/bin/rtaudio.dll"
        IMPORTED_IMPLIB "${RtAudio_ROOT}/lib/rtaudio.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${RtAudio_ROOT}/include/rtaudio"
    )
    
    add_library(RtMidi::rtmidi SHARED IMPORTED)
    set_target_properties(RtMidi::rtmidi PROPERTIES
        IMPORTED_LOCATION "${RtMidi_ROOT}/bin/rtmidi.dll"
        IMPORTED_IMPLIB "${RtMidi_ROOT}/lib/rtmidi.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${RtMidi_ROOT}/include/rtmidi"
    )

    add_library(yaml-cpp SHARED IMPORTED)
    set_target_properties(yaml-cpp PROPERTIES
        IMPORTED_LOCATION "${yaml-cpp_ROOT}/bin/yaml-cpp.dll"
        IMPORTED_IMPLIB "${yaml-cpp_ROOT}/lib/yaml-cpp.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${yaml-cpp_ROOT}/include"
    )
    
else() # debian 12 x86_64
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RTAUDIO REQUIRED rtaudio)
    pkg_check_modules(RTMIDI REQUIRED rtmidi)
    pkg_check_modules(YAMLCPP REQUIRED yaml-cpp)
endif()

qt_standard_project_setup()

# TODO: prob fix this to make it less ugly
# might nest CMakeList.txt files once I clean up the directory structure
qt_add_executable(metabolus
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.ui
    src/ParameterStore.cpp
    src/ParameterStore.h
    src/KeyboardController.cpp
    src/KeyboardController.h
    src/MidiController.cpp
    src/MidiController.h
    src/NoteQueue.cpp
    src/NoteQueue.h
    src/ConfigInterface.cpp
    src/ConfigInterface.h
    src/synth/AudioEngine.cpp
    src/synth/AudioEngine.h
    src/synth/Envelope.cpp
    src/synth/Envelope.h
    src/synth/Synth.cpp
    src/synth/Synth.h
    src/synth/ScopeBuffer.cpp
    src/synth/ScopeBuffer.h
    src/synth/Filter.cpp
    src/synth/Filter.h
    src/synth/Oscillator.cpp
    src/synth/Oscillator.h
    src/synth/Voice.cpp
    src/synth/Voice.h
    src/synth/WavetableController.cpp
    src/synth/WavetableController.h
    resources/resources.qrc
    src/ui/widgets/SmartSlider/SmartSlider.cpp
    src/ui/widgets/SmartSlider/SmartSlider.h
    src/ui/widgets/SmartSlider/SmartSlider.ui
    src/ui/widgets/EnvelopeGenerator/EnvelopeGenerator.cpp
    src/ui/widgets/EnvelopeGenerator/EnvelopeGenerator.h
    src/ui/widgets/EnvelopeGenerator/EnvelopeGenerator.ui
    src/ui/widgets/Scope/Scope.cpp
    src/ui/widgets/Scope/Scope.h
    src/ui/widgets/Scope/Scope.ui
)

set_target_properties(metabolus PROPERTIES AUTOUIC ON)

target_include_directories(metabolus PRIVATE
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/ui/widgets
)

if (WIN32)

    target_compile_options(metabolus PUBLIC "/Zc:__cplusplus")

    target_link_libraries(metabolus
        PRIVATE
            RtAudio::rtaudio
            RtMidi::rtmidi
            yaml-cpp
            Qt6::Widgets
    )

else() 
    target_include_directories(metabolus PRIVATE ${RTAUDIO_INCLUDE_DIRS} ${RTMIDI_INCLUDE_DIRS} ${YAMLCPP_INCLUDE_DIRS})
    target_link_libraries(metabolus PRIVATE Qt6::Widgets ${RTAUDIO_LIBRARIES} ${RTMIDI_LIBRARIES} ${YAMLCPP_LIBARIES})
    target_compile_options(metabolus PRIVATE ${RTAUDIO_CFLAGS_OTHER})
endif()
