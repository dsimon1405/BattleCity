cmake_minimum_required(VERSION 3.25.2 FATAL_ERROR)

set(PROJECT_NAME BattleCity)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	source/main.cpp
	source/Renderer/ShaderProgram.cpp
	source/Renderer/ShaderProgram.h
	source/Resources/ResourceManager.cpp
	source/Resources/ResourceManager.h
	source/Resources/stb_image.h
	source/Renderer/Texture2D.cpp
	source/Renderer/Texture2D.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)