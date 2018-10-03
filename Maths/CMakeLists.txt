add_library(Maths INTERFACE)

target_include_directories(Maths INTERFACE	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Maths/include>
											$<INSTALL_INTERFACE:include/maths>)

target_link_libraries(Maths INTERFACE Utils)

# INSTALL
install(DIRECTORY include/						DESTINATION ${INSTALL_INCLUDE_DIR}/maths)

install(TARGETS Maths EXPORT MAJ_Engine	ARCHIVE DESTINATION ${INSTALL_LIB_DIR})