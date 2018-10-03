cmake_minimum_required(VERSION 3.8)

get_filename_component(INSTALL_ROOT_DIR install ABSOLUTE)
set(INSTALL_CONFIG_DIR ${INSTALL_ROOT_DIR}/${CMAKE_BUILD_TYPE})
set(INSTALL_INCLUDE_DIR ${INSTALL_CONFIG_DIR}/include)
set(INSTALL_BIN_DIR ${INSTALL_CONFIG_DIR}/bin/engine)
set(INSTALL_LIB_DIR ${INSTALL_CONFIG_DIR}/lib)

set(CMAKE_INSTALL_PREFIX ${INSTALL_CONFIG_DIR})
project(MAJ)

include(CTest)
enable_testing()
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

if ("${API}" MATCHES "vk")
	set(API_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Lib/Vulkan/include)
	set(API_LIB ${PROJECT_SOURCE_DIR}/Lib/Vulkan/lib/vulkan-1.lib)
endif()

# INSTALL
add_subdirectory(Utils)
add_subdirectory(Maths)
add_subdirectory(Engine)
add_subdirectory(UnitTest)

install(FILES MAJ_EngineConfig.cmake DESTINATION ${INSTALL_ROOT_DIR})
install(EXPORT MAJ_Engine DESTINATION ${INSTALL_CONFIG_DIR})