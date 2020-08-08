cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# test
file(GLOB_RECURSE sources "*.cpp" "*.h")
add_executable(${ProjectId} ${sources})
set_target_properties(${ProjectId} PROPERTIES FOLDER "engine/tests")

# add dependencies
target_link_libraries(${ProjectId} PRIVATE engine)
target_link_libraries(${ProjectId} PRIVATE ${engine_LIBS})
target_include_directories(${ProjectId} PRIVATE ${engine_INCLUDE_DIRS})

if(EMSCRIPTEN)
	set_target_properties(${ProjectId} PROPERTIES LINK_FLAGS "-s FULL_ES3=1 -s USE_GLFW=3 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s LLD_REPORT_UNDEFINED -o out.html")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()