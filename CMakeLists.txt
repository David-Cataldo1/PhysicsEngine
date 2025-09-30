cmake_minimum_required(VERSION 3.22)

# Uses c++ and cuda
project(PhysicsEngine 
	VERSION 0.1.0
	DESCRIPTION "Molecular Physics Engine"
	LANGUAGES CXX CUDA
)

# Requires c++ 23 and cuda 17
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Warning stuff
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Builds static libs instead of dynamic
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Dependencies

find_package(OpenGL REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(glad CONFIG REQUIRED)

find_package(glm CONFIG REQUIRED)


# Source files (add as you go)

set(SRC
	src/main.cpp
 "src/renderer/shader.cpp" "src/renderer/shader.h")

# Create executable

add_executable(${PROJECT_NAME} ${SRC})

# Sets the correct project as default

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Cuda linking

set_target_properties(${PROJECT_NAME} PROPERTIES
	CUDA_SEPARABLE_COMPILATION ON
)


# Make sure GLFW doesn't include <GL_gl.h> b/c we're using glad

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)


# Link dependencies

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		OpenGL::GL
		glfw
		glad::glad
		glm::glm
)


# Supported CUDA architectures

set(CMAKE_CUDA_ARCHITECTURES 86)


# IDE source files structure

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${SRC})
