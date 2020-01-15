cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

#
# imgui
# https://github.com/ocornut/imgui
#

project(imgui C CXX)

# set c++ 17 and other stuff
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_USE_RELATIVE_PATHS TRUE)

# ------
# imgui
# ------
# sources
set(_src_root_path "")
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp*"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*"
)
# library
add_library(${PROJECT_NAME} STATIC ${_source_list})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER lib)
include_directories(src/)

# imgui_INCLUDE_DIRS
set(${PROJECT_NAME}_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)