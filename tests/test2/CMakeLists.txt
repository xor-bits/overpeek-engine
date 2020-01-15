cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

#
# test2
#

project(test2 C CXX)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# test2
add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER engine)

# add dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE engine)
target_link_libraries(${PROJECT_NAME} PRIVATE ${engine_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${engine_INCLUDE_DIRS})