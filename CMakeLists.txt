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


set(str "")
macro(print_dirs target)
	get_property(dirs TARGET ${target} PROPERTY INCLUDE_DIRECTORIES)
	foreach(dir ${dirs})
	set(str "${str} --include-path=\"${dir}\"")
	endforeach()
endmacro()

print_dirs("engine")
print_dirs("entities")
print_dirs("box2d")
print_dirs("glfw")

message("${str}")