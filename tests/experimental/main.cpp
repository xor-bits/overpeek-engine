#include <engine/include.hpp>
#include <engine/graphics/opengl/gl_command_buffer.hpp>
#include <engine/graphics/opengl/gl_include.hpp>



int main()
{
	oe::Engine::getSingleton().init({});
	oe::graphics::Window window{{}};

	oe::graphics::GLCommandBuffer cb{};

	while (!window->shouldClose())
	{
		cb.clearColor(oe::colors::clear_color.r, oe::colors::clear_color.g, oe::colors::clear_color.b, oe::colors::clear_color.a);
		cb.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		while (cb.run()) { spdlog::debug("[][]"); }

		window->update();
		window->pollEvents();
	}
		
	return 0;
}