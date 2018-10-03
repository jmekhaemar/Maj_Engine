cmake_minimum_required(VERSION 3.8)

# GLOBAL
string(REPLACE "\\" "/" MAJ_ENGINE_PATH "$ENV{MAJ_ENGINE_PATH}")

set(INSTALL_ROOT_DIR ${MAJ_ENGINE_PATH}/install)
set(INSTALL_CONFIG_DIR ${INSTALL_ROOT_DIR}/${CMAKE_BUILD_TYPE})
set(INSTALL_INCLUDE_DIR ${INSTALL_CONFIG_DIR}/include)
set(INSTALL_BIN_DIR ${INSTALL_CONFIG_DIR}/bin/editor)
set(INSTALL_LIB_DIR ${INSTALL_CONFIG_DIR}/lib)

set(CMAKE_INSTALL_PREFIX ${INSTALL_CONFIG_DIR})
project(MAJEditor_Lib)

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

set(Qt5Widgets_DIR ${MAJ_ENGINE_PATH}/Lib/Qt/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets)

set(MAJ_Engine_DIR ${MAJ_ENGINE_PATH}/install)
find_package(MAJ_Engine)

# SOURCES
file(GLOB_RECURSE SOURCES src/*)

set(EXECUTABLE_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

set(LIBRARY_SOURCES ${SOURCES})
list(REMOVE_ITEM LIBRARY_SOURCES ${EXECUTABLE_SOURCES})

# LIBRARY
add_library(MAJEditor STATIC)

target_sources(MAJEditor PRIVATE ${LIBRARY_SOURCES})
target_include_directories(MAJEditor PUBLIC	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
											$<INSTALL_INTERFACE:include/editor>)

target_link_libraries(MAJEditor PUBLIC	MAJEditorEngine
										Qt5::Widgets)

set_target_properties(MAJEditor PROPERTIES	ARCHIVE_OUTPUT_DIRECTORY ${MAJ_ENGINE_PATH}/Lib
											LINK_FLAGS "/WHOLEARCHIVE:MAJEditorEngine_${BUILD_PATH}"
											OUTPUT_NAME "MAJEditor_${BUILD_PATH}")

# INSTALL
install(DIRECTORY include/ DESTINATION "${INSTALL_INCLUDE_DIR}/editor")
install(DIRECTORY ${MAJ_ENGINE_PATH}/Lib/Qt/dll/ DESTINATION "${INSTALL_BIN_DIR}")

install(TARGETS MAJEditor EXPORT MAJ_Editor ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")

install(FILES MAJ_EditorConfig.cmake DESTINATION "${INSTALL_ROOT_DIR}")
install(EXPORT MAJ_Editor DESTINATION "${INSTALL_CONFIG_DIR}")