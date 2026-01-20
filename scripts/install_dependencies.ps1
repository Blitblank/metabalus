
echo "Installing dependencies ... "

$project_root = $PWD
$build_lib_dir = "$PWD\build\lib"


# qt
mkdir "$build_lib_dir\qt"
cd $project_root\lib\qtbase


# rtaudio
mkdir "$build_lib_dir\rtaudio" -Force
cd $project_root\lib\rtaudio
cmake -S . -B build -DRTDUIO_API_WASAPI=ON -DRTAUDIO_API_DS=OFF -DRT_AUDIO_API_ASIO=OFF -DRTAUDIO_BUILD_SHARED_LIBS=ON
cmake --build build --config Release
cmake --install build --prefix "$build_lib_dir\rtaudio"


# rtmidi
mkdir "$build_lib_dir\rtmidi" -Force
cd $project_root\lib\rtmidi
cmake -S . -B build  -DRT_MIDI_API_WINMM=ON -DRTMIDI_BUILD_SHARED_LIBS=ON
cmake --build build --config Release
cmake --install build --prefix "$build_lib_dir\rtmidi"

# yaml-cpp
<#
cd $project_root\lib\qtbase

#>

cd $project_root
