cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

project(engine C CXX)

# dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)
add_subdirectory(lib/freetype)
add_subdirectory(lib/glew)

# engine
include_directories(lib/glm)
include_directories(lib/spdlog)
include_directories(lib/stb-master)
include_directories(lib/miniaudio/include)

file(GLOB_RECURSE CPP_FILES *.cpp)
file(GLOB_RECURSE H_FILES *.h)

add_library(${PROJECT_NAME} STATIC ${CPP_FILES} ${H_FILES})

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} libglew_static)
target_link_libraries(${PROJECT_NAME} freetype)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC lib/glfw/include)
target_include_directories(${PROJECT_NAME} PUBLIC lib/glew/include)
target_include_directories(${PROJECT_NAME} PUBLIC lib/freetype/include)

set(${PROJECT_NAME}_include ${H_FILES} ${INCLUDE_DIRECTORIES})
#    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)