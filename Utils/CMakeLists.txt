file(GLOB_RECURSE SOURCES src/*)

add_library(Utils STATIC)

target_sources(Utils PRIVATE ${SOURCES})
target_include_directories(Utils PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Utils/include>
										$<INSTALL_INTERFACE:include/utils>)

set_target_properties(Utils PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Lib)

# INSTALL
install(DIRECTORY include/						DESTINATION ${INSTALL_INCLUDE_DIR}/utils)

install(TARGETS Utils EXPORT MAJ_Engine	ARCHIVE DESTINATION ${INSTALL_LIB_DIR})