file(GLOB_RECURSE SOURCES src/*)

add_library(WindowWrapper STATIC)

target_sources(WindowWrapper PRIVATE ${SOURCES})
target_include_directories(WindowWrapper PUBLIC	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Engine/WW/include>
												$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lib/glfw/include>
												$<INSTALL_INTERFACE:include/ww>
												$<INSTALL_INTERFACE:include/external/glfw>)

target_link_libraries(WindowWrapper PUBLIC	Maths
											Rhi)

target_link_libraries(WindowWrapper PRIVATE ${PROJECT_SOURCE_DIR}/Lib/glfw/lib/glfw3.lib)
						
set_target_properties(WindowWrapper PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Lib)

# INSTALL
install(DIRECTORY include/										DESTINATION ${INSTALL_INCLUDE_DIR}/ww)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Lib/glfw/include/	DESTINATION ${INSTALL_INCLUDE_DIR}/external/glfw)

install(TARGETS WindowWrapper EXPORT MAJ_Engine			ARCHIVE DESTINATION ${INSTALL_LIB_DIR})