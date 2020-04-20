cmake_minimum_required(VERSION 3.8.0)
project(phoneinc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# main

file(GLOB MAIN_SRC
	src/*.cpp
	src/*.h
)

file(GLOB WINDOWS_SRC
	src/windows/*.cpp
	src/windows/*.h
)

source_group("all" FILES ${MAIN_SRC})
source_group("windows" FILES ${WINDOWS_SRC})

file(GLOB ALL_SRC
	${MAIN_SRC}
	${WINDOWS_SRC}
)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    add_definitions(-DNOMINMAX)
	
    add_executable(${PROJECT_NAME} 
        ${ALL_SRC}
    )

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
elseif(APPLE)
    set(CMAKE_MACOSX_BUNDLE YES)
    set(CMAKE_OSX_SYSROOT iphoneos)

    file(GLOB IOS_SRC 
        ios/*.m
        ios/*.mm
        ios/*.h
    )

    set(RESOURCES
        ios/Main.storyboard
        ios/LaunchScreen.storyboard
        assets
    )

    add_executable(${PROJECT_NAME} 
        ${ALL_SRC}
		${IOS_SRC}
        ${RESOURCES}
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "${RESOURCES}"
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "12.0"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "69RWHMDZA7"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.skymunge.${PROJECT_NAME}"
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist
    )

    target_link_libraries(${PROJECT_NAME}
        "-framework UIKit"
        "-framework CoreGraphics"
        "-framework GLKit"
        "-framework OpenGLES"
        "-framework Foundation"
        "-framework AVFoundation"
        "-framework AudioToolbox"
    )
elseif(ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -frtti -fexceptions -Wno-error=format-security")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

    add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
    include_directories(${ANDROID_NDK}/sources/android/native_app_glue)

	add_library(${PROJECT_NAME} SHARED
		${ALL_SRC}
	)

    add_library(fmod SHARED IMPORTED)

    set_target_properties(fmod PROPERTIES IMPORTED_LOCATION
        ../../../../../../sky/lib/fmod/android/${ANDROID_ABI}/libfmod.so)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ../../../../../../sky/lib/fmod/android/${ANDROID_ABI}/libfmod.so
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libfmod.so)

    target_link_libraries(${PROJECT_NAME}
        android
        native_app_glue
        EGL
        sky
        fmod
        GLESv3
        log
        m
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC src)

# sky

add_subdirectory(sky)
target_link_libraries(${PROJECT_NAME} sky)
copy_required_libs(${PROJECT_NAME})