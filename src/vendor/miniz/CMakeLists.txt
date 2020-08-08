cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_USE_RELATIVE_PATHS TRUE)

#
# miniz 
# https://github.com/richgel999/miniz
#

# miniz
set(target_name "miniz")
add_library(${target_name} STATIC miniz.h miniz.c)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# miniz_INCLUDE_DIRS
set(${target_name}_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}
    CACHE INTERNAL "${target_name}: Include Directories" FORCE
)