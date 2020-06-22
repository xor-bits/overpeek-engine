cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# set c++ 17 and other stuff
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# packages
set(OpenGL_GL_PREFERENCE GLVND) # required
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
if(BUILD_VULKAN)
find_package(Vulkan REQUIRED)
endif()

# sources
set(source_list "")
set(source_list ${source_list} 
	"engine/engine.hpp"
	"engine/engine.cpp"
	"engine/enum.hpp"
	"engine/internal_libs.hpp"
	"engine/include.hpp"
	"engine/interfacegen.hpp"
	"engine/interfacegen.cpp"
)
set(source_list ${source_list} 
	"engine/assets/default_shader/default_shader.cpp"
	"engine/assets/default_shader/default_shader.hpp"
	"engine/assets/default_shader/shader.vert.hpp"
	"engine/assets/default_shader/shader.frag.hpp"
	"engine/assets/texture_set/texture_set.cpp"
	"engine/assets/texture_set/texture_set.hpp"
)
set(source_list ${source_list} 
	"engine/audio/audio.cpp"
	"engine/audio/audio.hpp"
)
set(source_list ${source_list} 
	"engine/graphics/font.cpp"
	"engine/graphics/font.hpp"
	"engine/graphics/sprite.cpp"
	"engine/graphics/sprite.hpp"
	"engine/graphics/spritePacker.cpp"
	"engine/graphics/spritePacker.hpp"
	"engine/graphics/textLabel.cpp"
	"engine/graphics/textLabel.hpp"
	"engine/graphics/vertexData.hpp"
)
set(source_list ${source_list} 
	"engine/graphics/interface/framebuffer.cpp"
	"engine/graphics/interface/framebuffer.hpp"
	"engine/graphics/interface/index_buffer_gen.hpp"
	"engine/graphics/interface/instance.cpp"
	"engine/graphics/interface/instance.hpp"
	"engine/graphics/interface/renderer.cpp"
	"engine/graphics/interface/renderer.hpp"
	"engine/graphics/interface/primitive_renderer.hpp"
	"engine/graphics/interface/shader.cpp"
	"engine/graphics/interface/shader.hpp"
	"engine/graphics/interface/shader_common.hpp"
	"engine/graphics/interface/texture.cpp"
	"engine/graphics/interface/texture.hpp"
	"engine/graphics/interface/window.cpp"
	"engine/graphics/interface/window.hpp"
)
set(source_list ${source_list} 
	"engine/graphics/opengl/gl_framebuffer.cpp"
	"engine/graphics/opengl/gl_framebuffer.hpp"
	"engine/graphics/opengl/gl_instance.cpp"
	"engine/graphics/opengl/gl_instance.hpp"
	"engine/graphics/opengl/gl_primitive_renderer.cpp"
	"engine/graphics/opengl/gl_primitive_renderer.hpp"
	"engine/graphics/opengl/gl_shader.cpp"
	"engine/graphics/opengl/gl_shader.hpp"
	"engine/graphics/opengl/gl_texture.cpp"
	"engine/graphics/opengl/gl_texture.hpp"
	"engine/graphics/opengl/gl_window.cpp"
	"engine/graphics/opengl/gl_window.hpp"
	"engine/graphics/opengl/buffers/buffer.cpp"
	"engine/graphics/opengl/buffers/buffer.hpp"
	"engine/graphics/opengl/buffers/indexBuffer.cpp"
	"engine/graphics/opengl/buffers/indexBuffer.hpp"
	"engine/graphics/opengl/buffers/vertexBuffer.cpp"
	"engine/graphics/opengl/buffers/vertexBuffer.hpp"
	"engine/graphics/opengl/buffers/storageBuffer.cpp"
	"engine/graphics/opengl/buffers/storageBuffer.hpp"
	"engine/graphics/opengl/buffers/vertexArray.cpp"
	"engine/graphics/opengl/buffers/vertexArray.hpp"
)
if(${BUILD_VULKAN})
	set(source_list ${source_list} 
		"engine/graphics/vulkan/vk_debugger.cpp"
		"engine/graphics/vulkan/vk_debugger.hpp"
		"engine/graphics/vulkan/vk_instance.cpp"
		"engine/graphics/vulkan/vk_instance.hpp"
		"engine/graphics/vulkan/vk_logical_device.cpp"
		"engine/graphics/vulkan/vk_logical_device.hpp"
		"engine/graphics/vulkan/vk_physical_device.cpp"
		"engine/graphics/vulkan/vk_physical_device.hpp"
		"engine/graphics/vulkan/vk_render_target.hpp"
		"engine/graphics/vulkan/vk_renderer.cpp"
		"engine/graphics/vulkan/vk_renderer.hpp"
		"engine/graphics/vulkan/vk_shader.cpp"
		"engine/graphics/vulkan/vk_shader.hpp"
		"engine/graphics/vulkan/vk_support.cpp"
		"engine/graphics/vulkan/vk_support.hpp"
		"engine/graphics/vulkan/vk_swapchain.hpp"
		"engine/graphics/vulkan/vk_sync_objects.hpp"
		"engine/graphics/vulkan/vk_window.cpp"
		"engine/graphics/vulkan/vk_window.hpp"
		"engine/graphics/vulkan/buffers/vk_buffer.cpp"
		"engine/graphics/vulkan/buffers/vk_buffer.hpp"
		"engine/graphics/vulkan/buffers/vk_command_buffer.cpp"
		"engine/graphics/vulkan/buffers/vk_command_buffer.hpp"
		"engine/graphics/vulkan/buffers/vk_command_pool.cpp"
		"engine/graphics/vulkan/buffers/vk_command_pool.hpp"
	)
endif()
set(source_list ${source_list} 
	"engine/gui/gui_manager.cpp"
	"engine/gui/gui_manager.hpp"
	"engine/gui/widgets/button.cpp"
	"engine/gui/widgets/button.hpp"
	"engine/gui/widgets/button_decorated.cpp"
	"engine/gui/widgets/button_decorated.hpp"
	"engine/gui/widgets/checkbox.cpp"
	"engine/gui/widgets/checkbox.hpp"
	"engine/gui/widgets/color_picker.cpp"
	"engine/gui/widgets/color_picker.hpp"
	"engine/gui/widgets/form.cpp"
	"engine/gui/widgets/form.hpp"
	"engine/gui/widgets/slider.cpp"
	"engine/gui/widgets/slider.hpp"
	"engine/gui/widgets/sprite_panel.cpp"
	"engine/gui/widgets/sprite_panel.hpp"
	"engine/gui/widgets/text_input.cpp"
	"engine/gui/widgets/text_input.hpp"
	"engine/gui/widgets/text_panel.cpp"
	"engine/gui/widgets/text_panel.hpp"
	"engine/gui/widgets/widget.cpp"
	"engine/gui/widgets/widget.hpp"
	"engine/gui/widgets/list.cpp"
	"engine/gui/widgets/list.hpp"
)
set(source_list ${source_list} 
	"engine/networking/client.cpp"
	"engine/networking/client.hpp"
	"engine/networking/server.cpp"
	"engine/networking/server.hpp"
)
set(source_list ${source_list} 
	"engine/utility/clock.cpp"
	"engine/utility/clock.hpp"
	"engine/utility/extra.cpp"
	"engine/utility/extra.hpp"
	"engine/utility/fileio.cpp"
	"engine/utility/fileio.hpp"
	"engine/utility/gameloop.cpp"
	"engine/utility/gameloop.hpp"
	"engine/utility/random.cpp"
	"engine/utility/random.hpp"
	"engine/utility/zip.cpp"
	"engine/utility/zip.hpp"
)
# source grouping
foreach(source IN ITEMS ${source_list})
    get_filename_component(source_path "${CMAKE_CURRENT_SOURCE_DIR}/${source}" PATH)
    file(RELATIVE_PATH relative_source_path "${CMAKE_CURRENT_SOURCE_DIR}" "${source_path}")
	if(MSVC)
		string(REPLACE "/" "\\" group ${relative_source_path})
    else()
		set(group ${relative_source_path})
	endif()
	source_group("${group}" FILES "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
endforeach()

# engine
set(target_name "engine")
add_library(${target_name} STATIC ${source_list})
set_target_properties(${target_name} PROPERTIES FOLDER engine)

# platform specific
if (MSVC)
set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "/MP") # multicore compile
target_link_libraries(${target_name} PRIVATE "Ws2_32") # required by enet
target_link_libraries(${target_name} PRIVATE "Winmm")  # required by enet
else()
target_link_libraries(${target_name} PRIVATE "stdc++fs")
endif()


# engine_INCLUDE_DIRS
set(${target_name}_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}
    CACHE INTERNAL "${target_name}: Include Directories" FORCE
)



set(GLFW_INSTALL FALSE)
set(GLFW_BUILD_DOCS FALSE)

# add dependencies
# box2d
add_subdirectory("vendor/box2d")
target_include_directories(${target_name} PUBLIC "vendor/box2d/include")
target_link_libraries(${target_name} PRIVATE box2d)
# glfw
add_subdirectory("vendor/glfw")
set_target_properties(glfw PROPERTIES FOLDER "engine/vendor")
target_include_directories(${target_name} PRIVATE ${glfw_INCLUDE_DIRS})
target_link_libraries(${target_name} PRIVATE glfw)
# glad
add_subdirectory("vendor/glad")
set_target_properties(glad PROPERTIES FOLDER "engine/vendor")
target_include_directories(${target_name} PRIVATE ${glad_INCLUDE_DIRS})
target_link_libraries(${target_name} PRIVATE glad)
# enet
add_subdirectory("vendor/enet")
set_target_properties(enet PROPERTIES FOLDER "engine/vendor")
target_include_directories(${target_name} PRIVATE "vendor/enet/include")
target_link_libraries(${target_name} PRIVATE enet)
# miniz
add_subdirectory("vendor/miniz")
set_target_properties(miniz PROPERTIES FOLDER "engine/vendor")
target_include_directories(${target_name} PUBLIC ${miniz_INCLUDE_DIRS})
target_link_libraries(${target_name} PRIVATE miniz)
# freetype
add_subdirectory("vendor/freetype")
set_target_properties(freetype PROPERTIES FOLDER "engine/vendor")
target_include_directories(${target_name} PRIVATE ${freetype_INCLUDE_DIRS})
target_link_libraries(${target_name} PRIVATE freetype)
# glm
target_include_directories(${target_name} PUBLIC "vendor/glm/include")
# stb
target_include_directories(${target_name} PRIVATE "vendor/stb/include")
# entt
target_include_directories(${target_name} PUBLIC "vendor/entt/include")
# spdlog
target_include_directories(${target_name} PUBLIC "vendor/spdlog/include")
# minimp3
target_include_directories(${target_name} PRIVATE "vendor/minimp3/include")
# rectpack2D
target_include_directories(${target_name} PRIVATE "vendor/rectpack2D/include")
# vulkan
if(BUILD_VULKAN)
target_include_directories(${target_name} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${target_name} PRIVATE ${Vulkan_LIBRARIES})
endif()
# openal
target_include_directories(${target_name} PRIVATE ${OPENAL_INCLUDE_DIR})
target_link_libraries(${target_name} PRIVATE ${OPENAL_LIBRARY})
# opengl
target_link_libraries(${target_name} PRIVATE ${OPENGL_LIBRARY})
# shaderc

add_subdirectory("vendor/shaderc/src")
target_link_libraries(${target_name} PRIVATE shaderc)
target_include_directories(${target_name} PRIVATE "vendor/shaderc/src/libshaderc/include")
set_target_properties(shaderc PROPERTIES FOLDER "engine/vendor")