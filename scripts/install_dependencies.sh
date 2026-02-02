
echo "Installing dependencies ... "

# TODO: add a clean (like delete build dirs) script

PROJECT_ROOT=$PWD

mkdir ${PWD}/build/lib -p

BUILD_LIB_DIR=${PWD}/build/lib

# rtaudio
mkdir ${BUILD_LIB_DIR}/rtaudio -p
cd ${PROJECT_ROOT}/lib/rtaudio
cmake -S . -B build -G Ninja \
    -DRTAUDIO_BUILD_SHARED_LIBS=ON \
    -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build --prefix ${BUILD_LIB_DIR}/rtaudio

# rtmidi
mkdir ${BUILD_LIB_DIR}/rtmidi -p
cd ${PROJECT_ROOT}/lib/rtmidi
cmake -S . -B build -G Ninja \
    -DRTMIDI_BUILD_SHARED_LIBS=ON \
    -DRTMIDI_API_ALSA=ON \
    -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build --prefix ${BUILD_LIB_DIR}/rtmidi

# yaml-cpp
mkdir ${BUILD_LIB_DIR}/yaml-cpp -p
cd ${PROJECT_ROOT}/lib/yaml-cpp 
cmake -S . -B build -G Ninja \
    -DYAML_BUILD_SHARED_LIBS=ON \
    -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build --prefix ${BUILD_LIB_DIR}/yaml-cpp

cd ${PROJECT_ROOT}
