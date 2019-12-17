cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

SET(sampleName unnamed-game)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
SET(CMAKE_CONFIGURATION_TYPE Release)


include_directories(${PCL_INCLUDE_DIRS})
include_directories("source/dependencies/GLEW/include")
include_directories("source/dependencies/GLFW/include")
include_directories("source/dependencies/FreeType/include")
include_directories("source/dependencies/glm")
include_directories("source/dependencies/spdlog")
include_directories("source/dependencies/stb-master")
include_directories("source/dependencies/miniaudio/include")

# main include dir
include_directories("source")


link_directories(${PCL_LIBRARY_DIRS})
link_directories("source/dependencies/GLFW/lib")
link_directories("source/dependencies/GLEW/lib")
link_directories("source/dependencies/FreeType/lib")


add_definitions(${PCL_DEFINITIONS})

add_library(unnamed-game source/engine.h source/internal_libs.h source/audio/audio.cpp source/audio/audio.h source/graphics/assets/basicPostprocessShader.cpp source/graphics/assets/basicPostprocessShader.h source/graphics/assets/multiTextureShader.cpp source/graphics/assets/multiTextureShader.h source/graphics/assets/singleTextureShader.cpp source/graphics/assets/singleTextureShader.h source/graphics/buffers/buffer.cpp source/graphics/buffers/buffer.h source/graphics/buffers/indexBuffer.cpp source/graphics/buffers/indexBuffer.h source/graphics/buffers/vertexBuffer.cpp source/graphics/buffers/vertexBuffer.h source/graphics/buffers/vertexArray.cpp source/graphics/buffers/vertexArray.h source/graphics/frameBuffer.cpp source/graphics/frameBuffer.h source/graphics/gl.cpp source/graphics/gl.h source/graphics/renderer.cpp source/graphics/renderer.h source/graphics/shader.cpp source/graphics/shader.h source/graphics/texture.cpp source/graphics/texture.h source/graphics/textureManager.cpp source/graphics/textureManager.h source/graphics/vertexData.h source/graphics/window.cpp source/graphics/window.h source/utility/binaryIO.h source/utility/clock.cpp source/utility/clock.h source/utility/debug.cpp source/utility/debug.h source/utility/filereader.cpp source/utility/filereader.h source/utility/gameloop.cpp source/utility/gameloop.h source/utility/math.cpp source/utility/math.h source/utility/random.cpp source/utility/random.h)

target_link_libraries(unnamed-game ${PCL_LIBRARIES} glfw3.lib glew32s.lib freetype.lib)
TARGET_LINK_LIBRARIES(${sampleName}
${MRPT_LIBS} # This is filled by FIND_PACKAGE(MRPT ...)
"" # Optional extra libs...
)