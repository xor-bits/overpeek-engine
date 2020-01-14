cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

#
# engine
#

project(engine C CXX)

# set c++ 17 and other stuff
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# FindGLFW.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules/")

# packages
find_package(Freetype REQUIRED)
find_package(GLFW3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(EntityX REQUIRED)

# ------
# engine
# ------
# sources
set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp*"
    "${_src_root_path}/*.h*"
)
foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
# library
add_library(${PROJECT_NAME} STATIC ${_source_list})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER engine)
# linux
if(UNIX)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-ldl")
endif()

# add dependencies
# add glad
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_include_directories(${PROJECT_NAME} PRIVATE ${glad_INCLUDE_DIRS})
# add freetype
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
# add glfw and opengl
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW3_INCLUDE_DIR})
# add openal
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENAL_INCLUDE_DIR})
# add entityx
target_include_directories(${PROJECT_NAME} PRIVATE ${ENTITYX_INCLUDE_DIRS})
# add others
include_directories(../lib/glad/include)
include_directories(../lib/glm/include)
include_directories(../lib/spdlog/include)
include_directories(../lib/stb/include)
include_directories(../lib/minimp3/include)

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
set(${PROJECT_NAME}_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}
	${inc_dirs}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)

if (UNIX)
set(${PROJECT_NAME}_LIBS
	${FREETYPE_LIBRARIES}
	${GLFW3_LIBRARY}
	${OPENGL_gl_LIBRARY}
	${OPENAL_LIBRARY}
	${ENTITYX_LIBRARIES}
	${CMAKE_DL_LIBS}
    CACHE INTERNAL "${PROJECT_NAME}: Libraries" FORCE
)
else()
set(${PROJECT_NAME}_LIBS
	${FREETYPE_LIBRARIES}
	${GLFW3_LIBRARY}
	${OPENGL_gl_LIBRARY}
	${OPENAL_LIBRARY}
	${ENTITYX_LIBRARIES}
    CACHE INTERNAL "${PROJECT_NAME}: Libraries" FORCE
)
endif()
