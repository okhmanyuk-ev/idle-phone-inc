cd ..
mkdir build
cd build
mkdir mac
cd mac
cmake ../.. -G Xcode -DBUILD_DEVELOPER=True -DBUILD_PLATFORM_MAC=True
