function(createTest NAME)
	set(SAMPLE_NAME test-${NAME})
	add_executable(${SAMPLE_NAME} src/${NAME}.cpp)

	add_dependencies(MAJEngine ${SAMPLE_NAME})
	add_dependencies(MAJEditorEngine ${SAMPLE_NAME})

	include_directories(include)
	target_link_libraries(${SAMPLE_NAME} Maths)

	target_include_directories(${SAMPLE_NAME} PRIVATE 
		$<BUILD_INTERFACE: ${PROJECT_SOURCE_DIR}/Lib/glm-0.9.8.5/glm>
		$<INSTALL_INTERFACE:glm>)

	set_target_properties(${SAMPLE_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/${SAMPLE_NAME})

	add_test(
		NAME ${SAMPLE_NAME}
		COMMAND $<TARGET_FILE:${SAMPLE_NAME}> 
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/${SAMPLE_NAME})

	add_custom_command(
		 TARGET ${SAMPLE_NAME}
		 COMMENT "Run tests"
		 POST_BUILD 
		 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/${SAMPLE_NAME}
		 COMMAND ${SAMPLE_NAME}
	)

endfunction()

createTest(Vec4)
createTest(Vec3)
createTest(Vec2)
createTest(Mat3)
createTest(Mat4)
createTest(Quaternion)
createTest(Transform)