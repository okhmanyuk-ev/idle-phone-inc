cd ..
mkdir build_emscripten

emcmake cmake -S . -B build_emscripten -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEVELOPER=True
cmake --build build_emscripten
