cmake_minimum_required(VERSION 3.16)
project(grav_window VERSION 1.0.0)

cmake_policy(SET CMP0072 NEW)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( _glfw3_HEADER_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include"
    "${CMAKE_SOURCE_DIR}/includes"
    "C:/dev/glfw/include" 
    "E:/dev/glfw/include" 
)

set( _glfw3_LIB_SEARCH_DIRS
    "/usr/lib"
    "/usr/local/lib"
    "${CMAKE_SOURCE_DIR}/lib"
    "C:/dev/glfw/lib-mingw-w64"
    "E:/dev/glfw/lib-mingw-w64"
)

set( _glfw3_ENV_ROOT $ENV{GLFW3_ROOT} )
if( NOT GLFW3_ROOT AND _glfw3_ENV_ROOT )
    set(GLFW3_ROOT ${_glfw3_ENV_ROOT} )
endif()

if( GLFW3_ROOT )
    list( INSERT _glfw3_HEADER_SEARCH_DIRS 0 "${GLFW3_ROOT}/include" )
    list( INSERT _glfw3_LIB_SEARCH_DIRS 0 "${GLFW3_ROOT}/lib" )
    list( INSERT _glfw3_LIB_SEARCH_DIRS 0 "${GLFW3_ROOT}/lib-mingw-w64" ) 
endif()

FIND_PATH(GLFW3_INCLUDE_DIR "GLFW/glfw3.h" PATHS ${_glfw3_HEADER_SEARCH_DIRS} )
FIND_LIBRARY(GLFW3_LIBRARY NAMES glfw3 glfw PATHS ${_glfw3_LIB_SEARCH_DIRS} )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)

find_package(OpenGL REQUIRED)

add_executable(
    grav_window
    src/gravsim/2D_gravsim/gravsim.c++
    src/gravsim/2D_gravsim/window/gravsim_window.c++
    src/gravsim/2D_gravsim/camera/gravsim_camera.c++
    src/gravsim/2D_gravsim/bodies/gravsim_bodies.c++
    src/gravsim/2D_gravsim/physics/gravsim_physics.c++
    src/glad.c
)

target_include_directories(
    grav_window
    PRIVATE
    dependencies
    ${GLFW3_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    src/gravsim/2D_gravsim
)

target_link_libraries(
    grav_window
    ${GLFW3_LIBRARY}
    OpenGL::GL
    -lgdi32
    -lopengl32
    -ldwmapi
)