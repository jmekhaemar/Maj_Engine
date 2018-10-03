get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

if(NOT DEFINED MAJ_ENGINE_PATH)
	string(REPLACE "\\" "/" MAJ_ENGINE_PATH "$ENV{MAJ_ENGINE_PATH}")
endif()

set(Qt5Widgets_DIR ${MAJ_ENGINE_PATH}/Lib/Qt/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets)

set(MAJ_Engine_DIR ${MAJ_ENGINE_PATH}/install)
find_package(MAJ_Engine)

include(${SELF_DIR}/${CMAKE_BUILD_TYPE}/MAJ_Editor.cmake)