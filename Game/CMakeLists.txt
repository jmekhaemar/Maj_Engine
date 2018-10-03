cmake_minimum_required(VERSION 3.8)

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

add_custom_target(	ShaderCompiler
					WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
					COMMAND call compile.bat)

add_subdirectory(script)