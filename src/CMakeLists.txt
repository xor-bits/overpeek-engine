cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# set c++ 17 and other stuff
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

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
	"engine/interfacegen_renderer.hpp"
)
set(source_list ${source_list} 
	"engine/asset/default_shader/default_shader.cpp"
	"engine/asset/default_shader/default_shader.hpp"
	"engine/asset/font_shader/font_shader.cpp"
	"engine/asset/font_shader/font_shader.hpp"
	"engine/asset/texture_set/texture_set.cpp"
	"engine/asset/texture_set/texture_set.hpp"
	"engine/asset/asset_loader.cpp"
	"engine/asset/asset_loader.hpp"
	"engine/asset/fonts.hpp"
)
set(source_list ${source_list} 
	"engine/audio/audio.cpp"
	"engine/audio/audio.hpp"
)
set(source_list ${source_list} 
	"engine/ecs/components/all.hpp"
	"engine/ecs/components/quad.hpp"
	"engine/ecs/entity.cpp"
	"engine/ecs/entity.hpp"
	"engine/ecs/world.hpp"
	"engine/ecs/world.cpp"
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
	"engine/graphics/primitives.cpp"
	"engine/graphics/primitives.hpp"
	"engine/graphics/renderer.cpp"
	"engine/graphics/renderer.hpp"
)
set(source_list ${source_list} 
	"engine/graphics/interface/command_buffer.hpp"
	"engine/graphics/interface/framebuffer.cpp"
	"engine/graphics/interface/framebuffer.hpp"
	"engine/graphics/interface/index_buffer_gen.hpp"
	"engine/graphics/interface/instance.hpp"
	"engine/graphics/interface/poly_renderer.hpp"
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
	"engine/graphics/opengl/gl_command_buffer.cpp"
	"engine/graphics/opengl/gl_command_buffer.hpp"
	"engine/graphics/opengl/gl_framebuffer.cpp"
	"engine/graphics/opengl/gl_framebuffer.hpp"
	"engine/graphics/opengl/gl_include.hpp"
	"engine/graphics/opengl/gl_instance.cpp"
	"engine/graphics/opengl/gl_instance.hpp"
	"engine/graphics/opengl/gl_poly_renderer.hpp"
	"engine/graphics/opengl/gl_primitive_renderer.cpp"
	"engine/graphics/opengl/gl_primitive_renderer.hpp"
	"engine/graphics/opengl/gl_shader.cpp"
	"engine/graphics/opengl/gl_shader.hpp"
	"engine/graphics/opengl/gl_texture.cpp"
	"engine/graphics/opengl/gl_texture.hpp"
	"engine/graphics/opengl/gl_window.cpp"
	"engine/graphics/opengl/gl_window.hpp"
	"engine/graphics/opengl/gl.cpp"
	"engine/graphics/opengl/gl.hpp"
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
	"engine/gui/jgui.cpp"
	"engine/gui/jgui.hpp"
	"engine/gui/widgets/button.cpp"
	"engine/gui/widgets/button.hpp"
	"engine/gui/widgets/button_decorated.cpp"
	"engine/gui/widgets/button_decorated.hpp"
	"engine/gui/widgets/checkbox.cpp"
	"engine/gui/widgets/checkbox.hpp"
	"engine/gui/widgets/color_input.cpp"
	"engine/gui/widgets/color_input.hpp"
	"engine/gui/widgets/color_picker.cpp"
	"engine/gui/widgets/color_picker.hpp"
	"engine/gui/widgets/slider_input.cpp"
	"engine/gui/widgets/slider_input.hpp"
	"engine/gui/widgets/vec_slider.hpp"
	"engine/gui/widgets/sprite_panel.cpp"
	"engine/gui/widgets/sprite_panel.hpp"
	"engine/gui/widgets/text_input.cpp"
	"engine/gui/widgets/text_input.hpp"
	"engine/gui/widgets/number_input.hpp"
	"engine/gui/widgets/text_panel.cpp"
	"engine/gui/widgets/text_panel.hpp"
	"engine/gui/widgets/widget.cpp"
	"engine/gui/widgets/widget.hpp"
	"engine/gui/widgets/list.cpp"
	"engine/gui/widgets/list.hpp"
	"engine/gui/widgets/event.hpp"
	"engine/gui/widgets/graph.cpp"
	"engine/gui/widgets/graph.hpp"
	)
set(source_list ${source_list} 
	"engine/networking/shared.hpp"
	"engine/networking/client.cpp"
	"engine/networking/client.hpp"
	"engine/networking/server.cpp"
	"engine/networking/server.hpp"
)
set(source_list ${source_list} 
	"engine/utility/color_string.hpp"
	"engine/utility/extra.cpp"
	"engine/utility/extra.hpp"
	"engine/utility/fileio.cpp"
	"engine/utility/fileio.hpp"
	"engine/utility/font_file.hpp"
	"engine/utility/formatted_error.hpp"
	"engine/utility/gameloop.cpp"
	"engine/utility/gameloop.hpp"
	"engine/utility/random.cpp"
	"engine/utility/random.hpp"
	"engine/utility/connect_guard.hpp"
	"engine/utility/connect_guard_additions.hpp"
	"engine/utility/text_edit.hpp"
	"engine/utility/text_edit_common.hpp"
	"engine/utility/text_edit_char.cpp"
	"engine/utility/text_edit_wchar_t.cpp"
	"engine/utility/text_edit_char16_t.cpp"
	"engine/utility/text_edit_char32_t.cpp"
	"engine/utility/ts_queue.hpp"
)

set(target_name "engine")
add_library(${target_name} STATIC ${source_list} "engine/ecs/world.hpp" "engine/ecs/world.cpp"    "engine/interfacegen_renderer.hpp")
set_target_properties(${target_name} PROPERTIES FOLDER engine)
if(OE_BUILD_MODE EQUAL 0)
	target_compile_definitions(${target_name} PUBLIC "OE_BUILD_MODE_GL")
elseif(OE_BUILD_MODE EQUAL 1)
	target_compile_definitions(${target_name} PUBLIC "OE_BUILD_MODE_SHADERC")
elseif(OE_BUILD_MODE EQUAL 2)
	target_compile_definitions(${target_name} PUBLIC "OE_BUILD_MODE_VULKAN")
endif()
# link cmrc asset
target_link_libraries(${target_name} PRIVATE asset)

# platform specific
if (MSVC)
	target_link_libraries(${target_name} PRIVATE "Ws2_32") # required by enet
	target_link_libraries(${target_name} PRIVATE "Winmm")  # required by enet
	target_compile_options(${target_name} PRIVATE "/W4" "/MP")
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(EMSCRIPTEN)
	# 
elseif(APPLE)
	target_compile_options(${target_name} PRIVATE "-Werror" "-Wall" "-Wextra" "-Wpedantic")
elseif(UNIX)
	target_link_libraries(${target_name} PRIVATE "stdc++fs")
	target_compile_options(${target_name} PRIVATE "-Werror" "-Wall" "-Wextra" "-Wpedantic")
endif()



# conan
target_link_libraries(${target_name} PUBLIC ${CONAN_LIBS})
target_link_directories(${target_name} PUBLIC ${CONAN_LIB_DIRS})
target_include_directories(${target_name} PUBLIC ${CONAN_INCLUDE_DIRS})
# additional non-conan dependencies
# opengl
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
if(OE_BUILD_MODE EQUAL 2)
find_package(Vulkan REQUIRED)
endif()
# rectpack2D
target_include_directories(${target_name} PUBLIC "vendor/rectpack2D/include")
# earcut.hpp
target_include_directories(${target_name} PUBLIC "vendor/earcut/include")
# miniaudio
target_include_directories(${target_name} PUBLIC "vendor/miniaudio")
# nativefiledialog
add_subdirectory("vendor/nativefiledialog")
target_link_libraries(${target_name} PUBLIC "nativefiledialog")
target_include_directories(${target_name} PUBLIC "vendor/nativefiledialog/src/include")



# engine_INCLUDE_DIRS
set(${target_name}_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}
    CACHE INTERNAL "${target_name}: Include Directories" FORCE
)