cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(overpeek-engine)

# engine
add_subdirectory(src) # static library (engine)

# example
add_subdirectory(demo) # executable (demo)