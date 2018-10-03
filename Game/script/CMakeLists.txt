file(GLOB_RECURSE SOURCES src/*)

set (EXECUTABLE_SOURCES "${PROJECT_SOURCE_DIR}/script/src/main.cpp")
set (SCRIPT_SOURCES ${SOURCES})
list(REMOVE_ITEM SCRIPT_SOURCES EXECUTABLE_SOURCES)

# GAME
add_executable(Game ${SOURCES})
target_include_directories(Game PRIVATE include)

add_dependencies(Game ShaderCompiler)
target_link_libraries(Game PRIVATE MAJEngine)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(SYSTEM_FLAG "")
else()
	set(SYSTEM_FLAG "/SUBSYSTEM:WINDOWS /entry:mainCRTStartup")
endif()

set_target_properties(Game PROPERTIES	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
										LINK_FLAGS "${SYSTEM_FLAG} /WHOLEARCHIVE:MAJEngine_${BUILD_PATH}")

#	EDITOR
add_library(Script SHARED)

target_sources(Script PRIVATE ${SCRIPT_SOURCES})

add_dependencies(Game ShaderCompiler)
target_include_directories(	Script PRIVATE include)

target_link_libraries(Script PRIVATE MAJEditor)

set_target_properties(Script PROPERTIES	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
										LINK_FLAGS "/WHOLEARCHIVE:MAJEditor_${BUILD_PATH}")