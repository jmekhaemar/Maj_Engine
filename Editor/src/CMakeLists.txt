cmake_minimum_required(VERSION 3.8)

# GLOBAL
project(MAJEditor)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "-Xclang -std=c++17 /W3 /GX /MDd /Zi /Ob0 /Od /RTC1")
else()
	set(CMAKE_CXX_FLAGS "/std:c++17 /EHsc /W4")
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(BUILD_PATH "debug")
else()
	set(BUILD_PATH "release")
endif()

string(REPLACE "\\" "/" MAJ_ENGINE_PATH "$ENV{MAJ_ENGINE_PATH}")
set(MAJ_Editor_DIR ${MAJ_ENGINE_PATH}/install)
find_package(MAJ_Editor)

# EXE
add_executable(Editor "${CMAKE_SOURCE_DIR}/main.cpp")

target_link_libraries(Editor PUBLIC MAJEditor)

set_target_properties(Editor PROPERTIES	RUNTIME_OUTPUT_DIRECTORY ${MAJ_ENGINE_PATH}/bin
										LINK_FLAGS "/WHOLEARCHIVE:MAJEditor_${BUILD_PATH}"
										OUTPUT_NAME "MAJEditor")