cd ..
mkdir build

emcmake cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEVELOPER=True
cmake --build build
