cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project("engine")
if(CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
endif()

# engine
add_subdirectory(src) # static library (engine)

option(OE_BUILD_TESTS "build with tests" ON)
set(OE_BUILD_MODE 0 CACHE STRING "0 - OpenGL, 1 - Shaderc and OpenGL, 2 - Vulkan, Shaderc and OpenGL")
if(OE_BUILD_TESTS)
	# tests
	add_subdirectory(tests)
endif()