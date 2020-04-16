cmake_minimum_required(VERSION 3.8.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -frtti -fexceptions -Wno-error=format-security")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)

set(BASE_DIR "../../../../../")

file(GLOB MAIN_SRC
    ${BASE_DIR}src/*.h
    ${BASE_DIR}src/*.cpp)

add_library(main SHARED
    ${MAIN_SRC}
)

add_subdirectory(${BASE_DIR}sky sky)
copy_required_libs(main)

add_library(fmod SHARED IMPORTED)

set_target_properties(fmod PROPERTIES IMPORTED_LOCATION
    ../../../../../../sky/lib/fmod/android/${ANDROID_ABI}/libfmod.so)

add_custom_command(TARGET main POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    ../../../../../../sky/lib/fmod/android/${ANDROID_ABI}/libfmod.so
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libfmod.so)

target_link_libraries(main
    android
    native_app_glue
    EGL
    sky
    fmod
    GLESv3
    log
    m
)