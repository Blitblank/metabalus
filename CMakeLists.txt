
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
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
    src/ParameterStore.h
    src/synth/AudioEngine.cpp
    src/synth/AudioEngine.h
    src/synth/Synth.cpp
    src/synth/Synth.h
    resources/resources.qrc
)

if (WIN32)
    target_link_libraries(metabolus PRIVATE Qt6::Widgets RtAudio::RtAudio)

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
