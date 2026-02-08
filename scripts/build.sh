
PROJECT_ROOT=${PWD}
BUILD_DIR=${PROJECT_ROOT}/build
LIB_ROOT=${BUILD_DIR}/lib

# libraries
RTAUDIO_ROOT=${LIB_ROOT}/rtaudio
RTMIDI_ROOT=${LIB_ROOT}/rtmidi
YAMLCPP_ROOT=${LIB_ROOT}/yaml-cpp

# detect dependencies
libraries=("rtaudio" "rtmidi" "yaml-cpp")
dependencies_found=0

for lib in "${libraries[@]}"; do
    if [[ -e "./build/lib/$lib" ]]; then
        echo "found $lib"
        ((dependencies_found++))
    else
        echo "did not find $lib"
    fi
done

# run the install script if dependencies not found
if [[ "$dependencies_found" -ne "${#libraries[@]}" ]]; then
    ./scripts/install_dependencies.sh
else
    echo "All dependencies detected, skipping dependency install step..."
fi

cmake -S . -B ${BUILD_DIR} -G Ninja \
    -DRtAudio_DIR="${RTAUDIO_ROOT}/share/rtaudio" \
    -DRtMidi_DIR="${RTMIDI_ROOT}/share/rtmidi" \
    -Dyaml-cpp_DIR="${YAMLCPP_ROOT}/lib/cmake/yaml-cpp" 

cmake --build build
