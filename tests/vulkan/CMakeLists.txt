cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

#
# test
#

# name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} C CXX)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# networking
file(GLOB_RECURSE sources "*.cpp" "*.h")
add_executable(${ProjectId} ${sources})
set_target_properties(${ProjectId} PROPERTIES FOLDER engine/demos)

# add dependencies
target_link_libraries(${ProjectId} PRIVATE engine)
target_link_libraries(${ProjectId} PRIVATE ${engine_LIBS})
target_include_directories(${ProjectId} PRIVATE ${engine_INCLUDE_DIRS})