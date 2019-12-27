cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

#
# engine
#

project(engine C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules/")

# packages
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLFW3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)


# engine
set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp*"
    "${_src_root_path}/*.h*"
)
add_library(${PROJECT_NAME} STATIC ${_source_list})
# Required on Unix OS family to be able to be linked into shared libraries.
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_compile_definitions(GLEW_STATIC)
# organize
foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

# add dependencies
# add glew
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
# add freetype
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
# add glfw and opengl
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW3_INCLUDE_DIR})
# add openal
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENAL_INCLUDE_DIR})
# add others
include_directories(../lib/glm/glm)
include_directories(../lib/spdlog/include)
include_directories(../lib/stb)
include_directories(../lib/minimp3/include)

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
	${OPENAL_LIBRARY}
    CACHE INTERNAL "${PROJECT_NAME}: Libraries" FORCE
)