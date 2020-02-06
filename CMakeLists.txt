cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(overpeek-engine)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# engine
add_subdirectory(src) # static library (engine)

option(BUILD_TESTS "build with tests" OFF)
if(${BUILD_TESTS})
	# tests
	add_subdirectory(tests/rendering) # executable
	add_subdirectory(tests/text) # executable
	add_subdirectory(tests/gui) # executable
	add_subdirectory(tests/entities) # executable
	add_subdirectory(tests/networking) # executable
	add_subdirectory(tests/vulkan) # executable
endif()
