
cmake_minimum_required(VERSION 3.21)
project(metabolus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if (WIN32) # windows 11 x86_64

    # Header-only target (real target, no ::)
    add_library(rtaudio_headers INTERFACE)
    target_include_directories(rtaudio_headers INTERFACE
        "C:/rtaudio/include"
        "C:/rtaudio/include/rtAudio"
    )

    # Imported binary (real target, no ::)
    add_library(rtaudio_binary SHARED IMPORTED)
    set_target_properties(rtaudio_binary PROPERTIES
        IMPORTED_LOCATION "C:/rtaudio/bin/rtaudio.dll"
        IMPORTED_IMPLIB   "C:/rtaudio/lib/rtaudio.lib"
    )

    # Unified interface target
    add_library(rtaudio INTERFACE)
    target_link_libraries(rtaudio INTERFACE
        rtaudio_headers
        rtaudio_binary
    )

    # Public alias (this is where :: belongs)
    add_library(RtAudio::RtAudio ALIAS rtaudio)

else() # debian 12 x86_64
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RTAUDIO REQUIRED rtaudio)
endif()

qt_standard_project_setup()

qt_add_executable(metabolus
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.ui
    src/ParameterStore.cpp
    src/ParameterStore.h
    src/KeyboardController.cpp
    src/KeyboardController.h
    src/NoteQueue.cpp
    src/NoteQueue.h
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
    target_link_libraries(metabolus
        PRIVATE
            Qt6::Widgets
            RtAudio::RtAudio
    )

    add_custom_command(TARGET metabolus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/rtaudio/bin/rtaudio.dll"
        $<TARGET_FILE_DIR:metabolus>
    )

else() 
    target_include_directories(metabolus PRIVATE ${RTAUDIO_INCLUDE_DIRS})
    target_link_libraries(metabolus PRIVATE Qt6::Widgets ${RTAUDIO_LIBRARIES})
    target_compile_options(metabolus PRIVATE ${RTAUDIO_CFLAGS_OTHER})
endif()
