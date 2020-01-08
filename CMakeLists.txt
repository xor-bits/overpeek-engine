cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(overpeek-engine)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# glad
add_subdirectory(lib/glad) # static library (glad)
# engine
add_subdirectory(src) # static library (engine)
# tests
add_subdirectory(tests/test1) # executable (test1)
add_subdirectory(tests/test2) # executable (test2)