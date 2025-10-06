cmake_minimum_required(VERSION 3.22)

# Uses C++ and CUDA
project(PhysicsEngine 
    VERSION 0.1.0
    DESCRIPTION "Molecular Physics Engine"
    LANGUAGES CXX CUDA
)

# Standards (keep what you wanted)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Build static libs instead of dynamic (your original choice)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# CUDA arch (set once for the project)
# 86 = Ampere (e.g., RTX 30xx). Adjust if needed.
set(CMAKE_CUDA_ARCHITECTURES 86)

# Dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(CUDAToolkit REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Source files (add as you go)
set(SRC
    src/main.cpp
    src/renderer/shader.cpp
    src/renderer/shader.h
    src/logging/log.cpp
    src/logging/log.h
    src/events/event.h
    src/events/application_event.h
    src/events/input.cpp
    src/events/input.h
    src/events/key_event.h
    src/events/mouse_event.h
    src/core.h
  "src/io/file_reader.h" "src/renderer/window.h" "src/renderer/window.cpp" "src/renderer/camera/camera.h" "src/renderer/camera/perspective_camera.h" "src/renderer/camera/perspective_camera.cpp" "src/renderer/camera/perspective_camera_controller.cpp" "src/renderer/camera/perspective_camera_controller.h" "src/application.h")

# Create executable
add_executable(${PROJECT_NAME} ${SRC})

# Make this the default startup project in VS
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# CUDA device linking: keep OFF until you actually need cross-TU device symbols
set_target_properties(${PROJECT_NAME} PROPERTIES
    CUDA_SEPARABLE_COMPILATION OFF
)

# Make sure GLFW doesn't include <GL/gl.h> because we're using glad
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

# Warnings — apply correctly per language (avoid leaking raw /W4 into nvcc)
target_compile_options(${PROJECT_NAME} PRIVATE
    # MSVC C++ warnings
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:CXX>>:/W4 /permissive->
    # CUDA passes host flags via -Xcompiler
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/W4>
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/permissive->
)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        glfw
        glad::glad
        glm::glm
        CUDA::cudart          # CUDA runtime
        EnTT::EnTT
        spdlog::spdlog
)

# IDE source files structure
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${SRC})
