cd ..
mkdir build

emcmake cmake -G Ninja -S . -B build -DBUILD_DEVELOPER=True
cmake --build build
