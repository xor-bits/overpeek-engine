cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_USE_RELATIVE_PATHS TRUE)

#
# glad
# https://glad.dav1d.de/
#

# glad
set(target_name "glad")
add_library(${target_name} STATIC src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# glad_INCLUDE_DIRS
set(${target_name}_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE INTERNAL "${target_name}: Include Directories" FORCE
)
