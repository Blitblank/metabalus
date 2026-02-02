
cmake -S . -B build -G Ninja \
    -DCMAKE_PREFIX_PATH=build/lib
cmake --build build
