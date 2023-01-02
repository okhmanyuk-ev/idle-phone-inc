cd ..
mkdir build
cd build
mkdir ios
cd ios
cmake ../.. -G Xcode -DBUILD_DEVELOPER=False -DBUILD_PLATFORM_MAC=False
