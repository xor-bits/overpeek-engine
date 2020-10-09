#include <engine/include.hpp>

#include <string>
#include <iostream>



oe::graphics::Window window;
oe::graphics::Renderer* renderer;
oe::assets::DefaultShader* shader;
oe::graphics::Font* font;

oe::graphics::u32TextLabel* label;
std::unique_ptr<oe::graphics::Quad> quad;

oe::utils::connect_guard cg_render;
oe::utils::connect_guard cg_update_2;
oe::utils::connect_guard cg_resize;



void render(oe::RenderEvent)
{
	shader->bind();
	renderer->render();
}

void update_2(oe::UpdateEvent<2>) {
	auto& gameloop = window->getGameloop();
	spdlog::debug("- frametime: {:3.3f} ms ({} fps)\n- updatetime: {:3.3f} ms ({} ups)",
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getFrametime()).count(),
		gameloop.getAverageFPS(),
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getUpdatetime<30>()).count(),
		gameloop.getAverageUPS<30>());
	
	oe::graphics::Sprite sprite;
	sprite.m_owner = font->getSpritePack()->getTexture();
	quad->setSprite(sprite);
}

void resize(const oe::ResizeEvent& event) {
	glm::mat4 pr_matrix = glm::ortho(0.0f, (float)event.framebuffer_size.x, (float)event.framebuffer_size.y, 0.0f);
	shader->setProjectionMatrix(pr_matrix);
	shader->setTexture(true);
}

int main(int argc, char** argv) {
	auto& engine = oe::Engine::getSingleton();
	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Colored Text";
	window_info.multisamples = 4;
	window = oe::graphics::Window(window_info);

	// connect events;
	cg_render.connect<oe::RenderEvent, render>(window->getGameloop().getDispatcher());
	cg_update_2.connect<oe::UpdateEvent<2>, update_2>(window->getGameloop().getDispatcher());
	cg_resize.connect<oe::ResizeEvent, resize>(window->getGameloop().getDispatcher());
	
	// instance settings
	engine.culling(oe::culling_modes::back);
	engine.swapInterval(1);
	engine.blending();

	// renderer
	renderer = new oe::graphics::Renderer(1000);

	// shader
	shader = new oe::assets::DefaultShader();

	// sprites
	font = new oe::graphics::Font(64, { oe::default_full_font_path_bold });
	
	// submitting
	label = new oe::graphics::u32TextLabel(*font);
	label->generate(oe::graphics::text_render_input<char32_t>{oe::graphics::text_render_input<char32_t>::string_view_color_vec{
			{ U"\u2116", { 0.06f, 0.13f, 1.0f, 1.0f } },
			{ U"The quick brown fox!", oe::colors::white },
			{ U"\u263A", { 1.0f, 0.13f, 0.13f, 1.0f } }
		}}, window, oe::colors::translucent_black);

	quad = renderer->create();
	quad->setPosition({ 50, 50 });
	quad->setSize(label->getSize());
	quad->setColor(oe::colors::white);
	quad->setSprite(label->getSprite());
	
	renderer->forget(std::move(quad));
	
	oe::graphics::Sprite sprite;
	sprite.m_owner = font->getSpritePack()->getTexture();
	quad = renderer->create();
	quad->setPosition({ 50, 134 });
	quad->setSize({ 200, 200 });
	quad->setColor(oe::colors::white);
	quad->setSprite(sprite);

	// auto close ctest after 2 seconds
	std::thread ctest_close_thread;
	for (size_t i = 0; i < argc; i++)
		if(std::strcmp(argv[i], "--ctest") == 0)
		{
			ctest_close_thread = std::thread([](){
				// test for 2 seconds
				std::this_thread::sleep_for(std::chrono::seconds(2));

				// then stop the gameloop
				window->getGameloop().stop();
			});
			break;
		}
	
	// start
	window->getGameloop().start(); // print the average frametime 30 times in a second

	if(ctest_close_thread.joinable())
		ctest_close_thread.join();

	// closing
	delete label;
	delete renderer;
	delete font;
	delete shader;

	return 0;
}
