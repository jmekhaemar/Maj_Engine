add_subdirectory(Rhi)
add_subdirectory(WW)

file(GLOB_RECURSE SOURCES src/*)

#	NORMAL
add_library(MAJEngine STATIC)

target_sources(MAJEngine PRIVATE ${SOURCES})
target_include_directories(MAJEngine PUBLIC	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Engine/include>
											$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lib/freetype/include>
											$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lib/FBX/include>
											$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lib/physX/include>
											$<INSTALL_INTERFACE:include>
											$<INSTALL_INTERFACE:include/external/freetype>
											$<INSTALL_INTERFACE:include/external/FBX>
											$<INSTALL_INTERFACE:include/external/physX>)

target_link_libraries(MAJEngine PUBLIC	Rhi
										WindowWrapper)
										
target_link_libraries(MAJEngine PRIVATE	${PROJECT_SOURCE_DIR}/Lib/freetype/lib/${BUILD_PATH}/freetype.lib
										${PROJECT_SOURCE_DIR}/Lib/FBX/lib/${BUILD_PATH}/libfbxsdk-md.lib
										${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PhysX3Extensions.lib
										${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PhysX3_x64.lib
										${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PhysX3Common_x64.lib
										${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PxFoundation_x64.lib
										${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PxPvdSDK_x64.lib)

set_target_properties(MAJEngine PROPERTIES	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Lib
											OUTPUT_NAME "MAJEngine_${BUILD_PATH}")

#	EDITOR
add_library(MAJEditorEngine STATIC)

target_sources(MAJEditorEngine PRIVATE ${SOURCES})
target_include_directories(MAJEditorEngine PUBLIC	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Engine/include>
													$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lib/freetype/include>
													$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lib/FBX/include>
													$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lib/physX/include>
													$<INSTALL_INTERFACE:include>
													$<INSTALL_INTERFACE:include/external/freetype>
													$<INSTALL_INTERFACE:include/external/FBX>
													$<INSTALL_INTERFACE:include/external/physX>)

target_link_libraries(MAJEditorEngine PUBLIC	Rhi
												WindowWrapper)

target_link_libraries(MAJEditorEngine PRIVATE	${PROJECT_SOURCE_DIR}/Lib/freetype/lib/${BUILD_PATH}/freetype.lib
												${PROJECT_SOURCE_DIR}/Lib/FBX/lib/${BUILD_PATH}/libfbxsdk-md.lib
												${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PhysX3Extensions.lib
												${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PhysX3_x64.lib
												${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PhysX3Common_x64.lib
												${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PxFoundation_x64.lib
												${PROJECT_SOURCE_DIR}/Lib/physX/lib/${BUILD_PATH}/PxPvdSDK_x64.lib)

target_compile_definitions(MAJEditorEngine PUBLIC "MAJ_EDITOR")
set_target_properties(MAJEditorEngine PROPERTIES	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Lib
													OUTPUT_NAME "MAJEditorEngine_${BUILD_PATH}")

# INSTALL
install(DIRECTORY include/											DESTINATION ${INSTALL_INCLUDE_DIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Lib/freetype/include/		DESTINATION ${INSTALL_INCLUDE_DIR}/external/freetype)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Lib/FBX/include/			DESTINATION ${INSTALL_INCLUDE_DIR}/external/fbx)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Lib/physX/include/			DESTINATION ${INSTALL_INCLUDE_DIR}/external/physX)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Lib/physX/dll/				DESTINATION ${INSTALL_BIN_DIR})

install(TARGETS MAJEngine		EXPORT MAJ_Engine			ARCHIVE DESTINATION ${INSTALL_LIB_DIR})
install(TARGETS MAJEditorEngine EXPORT MAJ_Engine			ARCHIVE DESTINATION ${INSTALL_LIB_DIR})
