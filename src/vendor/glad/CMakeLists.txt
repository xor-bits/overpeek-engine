cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_USE_RELATIVE_PATHS TRUE)

#
# glad
# https://glad.dav1d.de/
#

project(glad C)

# glad
add_library(${PROJECT_NAME} STATIC src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# glad_INCLUDE_DIRS
set(${PROJECT_NAME}_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)
