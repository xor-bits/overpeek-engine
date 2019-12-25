cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

#
# engine
#

project(engine C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules/")
message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

# packages
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLFW3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)


# engine
file(GLOB_RECURSE CPP_FILES *.cpp)
file(GLOB_RECURSE H_FILES *.h)
add_library(${PROJECT_NAME} STATIC ${CPP_FILES} ${H_FILES})
# Required on Unix OS family to be able to be linked into shared libraries.
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_compile_definitions(GLEW_STATIC)

# add dependencies
# add glew
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
# add freetype
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
# add glfw and opengl
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW3_INCLUDE_DIR})
# add others
include_directories(../lib/glm/glm)
include_directories(../lib/spdlog/include)
include_directories(../lib/stb)
include_directories(../lib/miniaudio)

# print all variables
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
set(${PROJECT_NAME}_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}
	${inc_dirs}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)

set(${PROJECT_NAME}_LIBS
	${GLEW_STATIC_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${GLFW3_LIBRARY}
	${OPENGL_gl_LIBRARY}
    CACHE INTERNAL "${PROJECT_NAME}: Libraries" FORCE
)

message("${PROJECT_NAME}_INCLUDE_DIRS: ${${PROJECT_NAME}_INCLUDE_DIRS}")
message("${PROJECT_NAME}_LIBS: ${${PROJECT_NAME}_LIBS}")
