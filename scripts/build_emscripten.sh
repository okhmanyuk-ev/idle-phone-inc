cd ..
mkdir build_emscripten

emcmake cmake -S . -B build_emscripten -DBUILD_DEVELOPER=True
cmake --build build_emscripten
