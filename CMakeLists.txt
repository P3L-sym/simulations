cmake_minimum_required(VERSION 3.16)
project(grav_window_test VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

set(GLFW3_INCLUDE_DIR "C:/msys64/ucrt64/include")
set(GLFW3_LIBRARY     "C:/msys64/ucrt64/lib/libglfw3.a")

add_executable(
    grav_window_test
    src/learning/opengl/opengl.c++
    src/glad.c
)

target_include_directories(
    grav_window_test 
    PRIVATE
    dependencies
    ${GLFW3_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    grav_window_test
    ${GLFW3_LIBRARY}
    OpenGL::GL
    -lgdi32
    -lopengl32
    -ldwmapi
)