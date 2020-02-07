cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(engine C CXX)

# set c++ 17 and other stuff
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# packages
set(OpenGL_GL_PREFERENCE GLVND) # required
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

# sources
set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    "${_src_root_path}/engine/*.cpp*"
    "${_src_root_path}/engine/*.h*"
)
foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

# engine
add_library(${PROJECT_NAME} STATIC ${_source_list})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER engine)

# platform specific
if (MSVC)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP") # multicore compile
target_link_libraries(${PROJECT_NAME} PRIVATE "Ws2_32") # required by enet
target_link_libraries(${PROJECT_NAME} PRIVATE "Winmm")  # required by enet
else()
target_link_libraries(${ProjectId} PRIVATE "stdc++fs")
endif()


# engine_INCLUDE_DIRS
set(${PROJECT_NAME}_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)



set(GLFW_INSTALL FALSE)
set(GLFW_BUILD_DOCS FALSE)

# add dependencies
# glfw
add_subdirectory(vendor/glfw)
set_target_properties(glfw PROPERTIES FOLDER engine/vendor)
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
# glad
add_subdirectory(vendor/glad)
set_target_properties(glad PROPERTIES FOLDER engine/vendor)
target_include_directories(${PROJECT_NAME} PRIVATE ${glad_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
# enet
add_subdirectory(vendor/enet)
set_target_properties(enet PROPERTIES FOLDER engine/vendor)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/enet/include)
target_link_libraries(${PROJECT_NAME} PRIVATE enet)
# miniz
add_subdirectory(vendor/miniz)
set_target_properties(miniz PROPERTIES FOLDER engine/vendor)
target_include_directories(${PROJECT_NAME} PUBLIC ${miniz_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE miniz)
# freetype
add_subdirectory(vendor/freetype)
set_target_properties(freetype PROPERTIES FOLDER engine/vendor)
target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
# glm
target_include_directories(${PROJECT_NAME} PUBLIC vendor/glm/include)
# stb
target_include_directories(${PROJECT_NAME} PRIVATE vendor/stb/include)
# entt
target_include_directories(${PROJECT_NAME} PUBLIC vendor/entt/include)
# spdlog
target_include_directories(${PROJECT_NAME} PUBLIC vendor/spdlog/include)
# minimp3
target_include_directories(${PROJECT_NAME} PRIVATE vendor/minimp3/include)
# rectpack2D
target_include_directories(${PROJECT_NAME} PRIVATE vendor/rectpack2D/include)
# vulkan
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})
# openal
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENAL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENAL_LIBRARY})
# opengl
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY} ${OPENGL_LIBRARY})
