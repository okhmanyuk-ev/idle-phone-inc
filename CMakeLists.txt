cmake_minimum_required(VERSION 3.10)
project(idle-phone-inc)

set(PRODUCT_NAME "Idle Phone Inc.")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# main

if(BUILD_DEVELOPER)
	add_definitions(-DBUILD_DEVELOPER)
endif()

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DPRODUCT_NAME="${PRODUCT_NAME}")

file(GLOB MAIN_SRC
	src/*.cpp
	src/*.h
)

file(GLOB WINDOWS_SRC
	src/windows/*.cpp
	src/windows/*.h
)

file(GLOB SCREENS_SRC
	src/screens/*.cpp
	src/screens/*.h
)

source_group("all" FILES ${MAIN_SRC})
source_group("windows" FILES ${WINDOWS_SRC})
source_group("screens" FILES ${SCREENS_SRC})

file(GLOB ALL_SRC
	${MAIN_SRC}
	${WINDOWS_SRC}
	${SCREENS_SRC}
)

if(WIN32)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker /SUBSYSTEM:WINDOWS -Xlinker /ENTRY:mainCRTStartup")
	else()
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()

	add_definitions(-DNOMINMAX)

	add_executable(${PROJECT_NAME}
		${ALL_SRC}
	)

	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
elseif(APPLE)
	set(CMAKE_MACOSX_BUNDLE YES)

	if(BUILD_PLATFORM_MAC)
		set(RESOURCES
			assets
		)

		add_executable(${PROJECT_NAME}
			${ALL_SRC}
			${RESOURCES}
		)

		set_target_properties(${PROJECT_NAME} PROPERTIES RESOURCE ${RESOURCES})
	else()
		set(CMAKE_OSX_SYSROOT iphoneos)

		file(GLOB IOS_SRC
			ios/*.m
			ios/*.mm
			ios/*.h
		)

		source_group("all" FILES ${IOS_SRC})

		set(RESOURCES
			ios/LaunchScreen.storyboard
			ios/Images.xcassets
			assets
		)

		set(OBJC_SOURCES
			src/application.cpp
		)
		set_source_files_properties(${OBJC_SOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++ -fembed-bitcode")

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
			XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.dreamskies.${PROJECT_NAME}"
			XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
			XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
			XCODE_ATTRIBUTE_INSTALL_PATH "/Applications"
			XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
			XCODE_ATTRIBUTE_PRODUCT_NAME "${PRODUCT_NAME}"
			XCODE_ATTRIBUTE_FRAMEWORK_SEARCH_PATHS "ios/frameworks"
			MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist
		)
	endif()
elseif(ANDROID)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

	add_library(${PROJECT_NAME} SHARED
		${ALL_SRC}
	)
elseif(EMSCRIPTEN)
	set(ASSETS_FOLDER_NAME assets)
	set(ASSETS_FOLDER_PATH ${CMAKE_SOURCE_DIR}/${ASSETS_FOLDER_NAME})

	add_executable(${PROJECT_NAME} ${ALL_SRC} ${ASSETS_FOLDER_PATH})

	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)
	set(CMAKE_EXECUTABLE_SUFFIX .html)

	target_link_options(${PROJECT_NAME} PUBLIC --preload-file ${ASSETS_FOLDER_PATH}@${ASSETS_FOLDER_NAME})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC src)

# sky

add_subdirectory(sky)
target_link_libraries(${PROJECT_NAME} sky)
copy_required_libs(${PROJECT_NAME})
