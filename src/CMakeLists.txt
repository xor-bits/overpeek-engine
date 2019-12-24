cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# packages
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(glfw3 3.3 REQUIRED)


# engine
file(GLOB_RECURSE CPP_FILES *.cpp)
file(GLOB_RECURSE H_FILES *.h)
add_library(${PROJECT_NAME} STATIC ${CPP_FILES} ${H_FILES})


# add dependencies
# add glew
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
# add freetype
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
# add glfw and opengl
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
# add others
target_include_directories(${PROJECT_NAME} PRIVATE ../lib/glm/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ../lib/spdlog/include)
target_include_directories(${PROJECT_NAME} PRIVATE ../lib/stb-master)
target_include_directories(${PROJECT_NAME} PRIVATE ../lib/miniaudio/include)


set(${PROJECT_NAME}_include ${H_FILES} ${INCLUDE_DIRECTORIES}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)