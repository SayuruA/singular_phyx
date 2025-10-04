cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 17)

# --- Add GLFW ---
set(GLFW_ROOT "C:/cppCoding/glfw-3.4") 
# glfw-3.4

# Tell compiler where to find headers
# include_directories(${GLFW_ROOT}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Link with precompiled GLFW library
link_directories(${GLFW_ROOT}/build/src)

# --- Your executable ---
add_executable(test src/test.cpp src/glad.c src/Shader.cpp)

# Link GLFW and system dependencies
target_link_libraries(test glfw3)

# On Windows with MinGW, also link OpenGL and gdi32/user32 libs:
if (WIN32)
    target_link_libraries(test opengl32 gdi32 user32 -static-libgcc -static-libstdc++)
endif()
