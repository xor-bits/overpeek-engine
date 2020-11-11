#include <engine/include.hpp>

// NOTE: this test attempts to follow the best practises

// I prefer writing the long namespace
/* namespace oe { using namespace graphics; } */

#define CUSTOM_SHADER_EXAMPLE 0



class Application
{
public:
	Application();

	void run();

public:
	constexpr static size_t ups = 60;

private:
	static oe::WindowInfo gen_window_info();
#if CUSTOM_SHADER_EXAMPLE
	static oe::ShaderInfo gen_shader_info();
#endif

	// some events
	void on_render(const oe::RenderEvent&);
	void on_update(const oe::UpdateEvent<ups>&);
	void on_resize(const oe::ResizeEvent&);

private:
	// object to hold the window
	oe::graphics::Window window;

	// custom shader
#if CUSTOM_SHADER_EXAMPLE
	oe::graphics::Shader custom_shader;
#endif
	// asset shader is easier to use :P
	oe::asset::DefaultShader shader;

	// 2d, batched, easy-to-use, sorted, multitexture, multishader(*1),... quad renderer
	// (*1) = not yet
	oe::graphics::Renderer renderer;

	// generates a spritepack holding all utf-8 sprites it has been requested to hold
	// always loads atleast the codepoints: 0 (NULL), [ 32 , 126 ]
	oe::graphics::Font font;

	// Holds a texture (framebuffer) that can be written in to.
	// Colored unicode characters with outlines are possible!
	oe::graphics::u32TextLabel label;

	// event connect guards to not to worry about disconnecting event functions
	// on the order of destruction: connect guards should be destructed before \
	// the connection (ex. dispatcher) it points to
	oe::utils::connect_guard cg_on_render;
	oe::utils::connect_guard cg_on_update;
	oe::utils::connect_guard cg_on_resize;
};



oe::WindowInfo Application::gen_window_info()
{
	oe::WindowInfo w_info;
	w_info.size = { 900, 600 };
	w_info.main_updatesystem_ups = ups;
	/* w_info. */

	return std::move(w_info);
}

#if CUSTOM_SHADER_EXAMPLE
oe::ShaderInfo Application::gen_shader_info()
{
	oe::ShaderInfo s_info;
	s_info.name = "my_custom_shader";
	s_info.shader_stages = {
		{ oe::shader_stages::vertex_shader,   /* your vertex shader code */, /* optional include path (only works with shaderc on) */ },
		{ oe::shader_stages::fragment_shader, /* /\ */,                      /* /\ */ },
	};
	/* w_info. */

	return std::move(s_info);
}
#endif

Application::Application()
	: window(gen_window_info())
#if CUSTOM_SHADER_EXAMPLE
	, custom_shader(gen_shader_info())
#endif
	, shader(oe::polygon_mode::fill)
	, renderer()
	, font(64, true)
	, label(font, 32)
{
	// window, shader, etc. default constructors wont open up windows or anything
	// they just construct shared_ptr with nullptr
	// except for the assets and renderer
	
	// connect event functions like so \/
	// or connect lambdas like so \/
	// cg_on_render.connect<oe::UpdateEvent<ups>>(window, [](const oe::UpdateEvent<ups>& e){ /* code */ });
	cg_on_render.connect<oe::UpdateEvent<ups>, &Application::on_update>(window, this);
	cg_on_update.connect<oe::RenderEvent, &Application::on_render>(window, this);
	cg_on_resize.connect<oe::ResizeEvent, &Application::on_resize>(window, this);

	// submit something for the renderer
	label.generate({{ U"Hello\n", oe::colors::red }, { U"World", oe::colors::blue }}, window);
	std::unique_ptr<oe::graphics::Quad> quad = renderer.create();
	quad->setPosition({ 0.0f, 0.0f });
	quad->setSize({ 0.5f * label.getAspect(), 0.5f });
	quad->setSprite(label.getSprite());
	quad->setRotationAlignment(oe::alignments::center_center); // position is in the middle of the sprite
	// hold the quad or give it back to the renderer
	// renderer will hold it until the next clear
	renderer.forget(std::move(quad));
}

void Application::run()
{
	// start the main gameloop
	window->getGameloop().start();
}

void Application::on_render(const oe::RenderEvent& /* e */)
{
	/* runs as how ever many times it (unless swap interwal is set) can per second */

	/* window->clear(oe::colors::cyan); */ // optional, will be cleared anyway

	shader.bind();
	renderer.render();
}

void Application::on_update(const oe::UpdateEvent<ups>& /* e */)
{
	/* runs *ups* per second */
}

void Application::on_resize(const oe::ResizeEvent& e)
{
	/* once when the gameloop starts and once every time the user resizes the window */

#if CUSTOM_SHADER_EXAMPLE
	custom_shader.setUniform("uniform_pr_matrix_name", glm::ortho<float>(-e.aspect, e.aspect, 1.0f, -1.0f));
#endif
	shader.setProjectionMatrix(glm::ortho<float>(-e.aspect, e.aspect, 1.0f, -1.0f));
}



int main()
{
	oe::EngineInfo e_info;
	/* e_info. */

	auto& engine = oe::Engine::getSingleton();
	engine.init(e_info);

	Application app;
	app.run();
}