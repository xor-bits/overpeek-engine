cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# demo  
include_directories(${engine_include})

add_executable(demo main.cpp)
target_link_libraries(demo)