
echo "Installing dependencies ... "

$project_root = $PWD

if (-not (Test-Path -Path "$PWD\build\lib")) {
    mkdir "$PWD\build\lib"
}

$build_lib_dir = "$PWD\build\lib"

# rtaudio
mkdir "$build_lib_dir\rtaudio" -Force
cd $project_root\lib\rtaudio
cmake -S . -B build -G "Visual Studio 17 2022" -DRTDUIO_API_WASAPI=ON -DRTAUDIO_API_DS=OFF -DRT_AUDIO_API_ASIO=OFF -DRTAUDIO_BUILD_SHARED_LIBS=ON
cmake --build build --config Release
cmake --install build --prefix "$build_lib_dir\rtaudio"


# rtmidi
mkdir "$build_lib_dir\rtmidi" -Force
cd $project_root\lib\rtmidi
cmake -S . -B build -G "Visual Studio 17 2022" -DRT_MIDI_API_WINMM=ON -DRTMIDI_BUILD_SHARED_LIBS=ON
cmake --build build --config Release
cmake --install build --prefix "$build_lib_dir\rtmidi"

# yaml-cpp
<#
cd $project_root\lib\qtbase

#>

cd $project_root
