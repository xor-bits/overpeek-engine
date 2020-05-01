cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project("engine")

# engine
add_subdirectory(src) # static library (engine)

option(BUILD_TESTS "build with tests" OFF)
option(BUILD_VULKAN "build with vulkan test" OFF)
if(${BUILD_TESTS})
	# tests
	add_subdirectory(tests)
endif()
