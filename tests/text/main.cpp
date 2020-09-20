#include <engine/include.hpp>

#include <string>
#include <iostream>



oe::graphics::Window window;
oe::graphics::Renderer* renderer;
oe::assets::DefaultShader* shader;
oe::graphics::Font* font;

oe::graphics::u32TextLabel* label;
std::unique_ptr<oe::graphics::Quad> quad;



void render(oe::RenderEvent)
{
	shader->bind();
	renderer->render();
}

void update_2(oe::UpdateEvent<2>) {
	auto& gameloop = window->getGameloop();
	spdlog::debug("frametime: {:3.3f} ms ({} fps)", gameloop.getFrametimeMS(), gameloop.getAverageFPS());
	
	oe::graphics::Sprite sprite;
	sprite.m_owner = font->getSpritePack()->getTexture();
	quad->setSprite(sprite);
	quad->update();
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
	engine_info.debug_messages = true;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Colored Text";
	window_info.multisamples = 4;
	window = oe::graphics::Window(window_info);

	// connect events
	window->connect_listener<oe::ResizeEvent, &resize>();
	window->connect_listener<oe::RenderEvent, &render>();
	window->connect_listener<oe::UpdateEvent<2>, &update_2>();
	
	// instance settings
	engine.culling(oe::culling_modes::back);
	engine.swapInterval(0);
	engine.blending();

	// renderer
	renderer = new oe::graphics::Renderer(1000);

	// shader
	shader = new oe::assets::DefaultShader();

	// sprites
	font = new oe::graphics::Font(64, oe::default_full_font_path_bold);
	
	// submitting
	label = new oe::graphics::u32TextLabel(*font);
	label->generate({{
			{ U"\u2116", { 0.06f, 0.13f, 1.0f, 1.0f } },
			{ U"The quick brown fox!", oe::colors::white },
			{ U"\u263A", { 1.0f, 0.13f, 0.13f, 1.0f } }
		}}, window, oe::colors::translucent_black);

	quad = renderer->create();
	quad->setPosition({ 50, 50 });
	quad->setSize(label->getSize()); spdlog::debug("{}", label->getSize());
	quad->setColor(oe::colors::white);
	quad->setSprite(label->getSprite());
	quad->update();
	
	renderer->forget(std::move(quad));
	
	oe::graphics::Sprite sprite;
	sprite.m_owner = font->getSpritePack()->getTexture();
	quad = renderer->create();
	quad->setPosition({ 50, 134 });
	quad->setSize({ 200, 200 });
	quad->setColor(oe::colors::white);
	quad->setSprite(sprite);
	quad->update();
	
	// blue: <#0000ff>
	// incomplete: <#542>
	// faulty: <#5f>>>>>>>>>
	// with 0x: <#0x4354>
	// negative: <#-43531>
	
	// start
	window->getGameloop().start(); // print the average frametime 30 times in a second

	// closing
	delete font;
	delete shader;

	return 0;
}
