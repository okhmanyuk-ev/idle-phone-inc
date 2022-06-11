cmake_minimum_required(VERSION 3.10)
project(phoneinc)

set(PRODUCT_NAME "Idle Phone Inc.")

set(CMAKE_CXX_STANDARD 20)
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
elseif(ANDROID)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

	add_library(${PROJECT_NAME} SHARED
		${ALL_SRC}
	)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC src)

# sky

add_subdirectory(sky)
target_link_libraries(${PROJECT_NAME} sky)
copy_required_libs(${PROJECT_NAME})
