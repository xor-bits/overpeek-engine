cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

#
# glad
# https://glad.dav1d.de/
#

project(glad C CXX)

# set c++ 17 and other stuff
set(CMAKE_USE_RELATIVE_PATHS TRUE)

# glad
add_library(${PROJECT_NAME} STATIC src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER lib)

# glad_INCLUDE_DIRS
set(${PROJECT_NAME}_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)