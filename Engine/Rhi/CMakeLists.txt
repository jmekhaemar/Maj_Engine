file(GLOB_RECURSE COMMON_SOURCES src/common/*)
file(GLOB_RECURSE SOURCES src/${API}/*)

add_library(Rhi STATIC)

target_sources(Rhi PRIVATE	${SOURCES}
							${COMMON_SOURCES})

target_include_directories(	Rhi PUBLIC	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Engine/Rhi/include>
										$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Engine/Rhi/include/${API}>
										$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lib/stb-master/include>
										$<BUILD_INTERFACE:${API_INCLUDE_DIR}>
										$<INSTALL_INTERFACE:include/rhi>
										$<INSTALL_INTERFACE:include/rhi/${API}>
										$<INSTALL_INTERFACE:include/external/stb-master>
										$<INSTALL_INTERFACE:include/external/${API}>)

target_link_libraries(Rhi PUBLIC	Maths
									WindowWrapper)

target_link_libraries(Rhi PRIVATE ${API_LIB})

set_target_properties(Rhi PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Lib)

# INSTALL
install(DIRECTORY include/											DESTINATION ${INSTALL_INCLUDE_DIR}/rhi)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Lib/stb-master/include/		DESTINATION ${INSTALL_INCLUDE_DIR}/external/stb-master)
install(DIRECTORY ${API_INCLUDE_DIR}/								DESTINATION ${INSTALL_INCLUDE_DIR}/external/${API})

install(TARGETS Rhi EXPORT MAJ_Engine						ARCHIVE DESTINATION ${INSTALL_LIB_DIR})