
PROJECT_ROOT=${PWD}
BUILD_DIR=${PROJECT_ROOT}/build
LIB_ROOT=${BUILD_DIR}/lib

# libraries
RTAUDIO_ROOT=${LIB_ROOT}/rtaudio
RTMIDI_ROOT=${LIB_ROOT}/rtmidi
YAMLCPP_ROOT=${LIB_ROOT}/yaml-cpp

cmake -S . -B ${BUILD_DIR} -G Ninja \
    -DRtAudio_DIR="${RTAUDIO_ROOT}/share/rtaudio" \
    -DRtMidi_DIR="${RTMIDI_ROOT}/share/rtmidi" \
    -Dyaml-cpp_DIR="${YAMLCPP_ROOT}/lib/cmake/yaml-cpp" 

cmake --build build
