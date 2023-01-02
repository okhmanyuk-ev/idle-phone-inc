cd ..
mkdir build
cd build
mkdir mac
cd mac
cmake ../.. -G Xcode -DBUILD_DEVELOPER=False -DBUILD_PLATFORM_MAC=True
