cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_USE_RELATIVE_PATHS TRUE)

#
# miniz 
# https://github.com/richgel999/miniz
#

project(miniz C)

# miniz
add_library(${PROJECT_NAME} STATIC miniz.h miniz.c)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

# miniz_INCLUDE_DIRS
set(${PROJECT_NAME}_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)