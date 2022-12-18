mkdir build_emscripten

emcmake cmake -S . -B build_emscripten
cmake --build build_emscripten
