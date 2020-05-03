#include <engine/include.hpp>

#include <string>



oe::graphics::Window* window;
oe::graphics::SpritePack* pack;
oe::assets::DefaultShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;
oe::graphics::TextLabel* label;

void render(float update_fraction)
{
	shader->bind();
	renderer->render();
}

void update() {
	auto& gameloop = window->getGameloop();
	spdlog::info("frametime: {:3.3f} ms ({} fps)", gameloop.getFrametimeMS(), gameloop.getAverageFPS());
}

void resize(const glm::vec2& window_size) {
	float aspect = window->aspect();
	glm::mat4 pr_matrix = glm::ortho(-aspect, aspect, 1.0f, -1.0f);
	shader->setProjectionMatrix(pr_matrix);
	shader->useTexture(true);
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
	window_info.title = "Test 2 - Colored Text";
	window_info.multisamples = 4;
	window_info.resize_callback = resize;
	window_info.update_callback = update;
	window_info.render_callback = render;
	window = engine.createWindow(window_info);
	
	// instance settings
	engine.culling(oe::culling_modes::back);
	engine.swapInterval(0);
	engine.blending();

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamicrender;
	renderer_info.indexRenderType = oe::types::staticrender;
	renderer_info.max_primitive_count = 1000;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = engine.createRenderer(renderer_info);

	// shader
	shader = new oe::assets::DefaultShader();

	// sprites
	pack = new oe::graphics::SpritePack();
	font = new oe::graphics::Font(pack);
	oe::graphics::Text::setFont(*font);
	pack->construct();

	
	// submitting
	label = new oe::graphics::TextLabel(font);
	label->generate("<#1020ff>The quick brown fox <#ff2020>jumps <#ffffff>over the lazy dog.", window);
	auto quad = renderer->createQuad();
	quad->setPosition({ -1.0f, 0.0f });
	quad->setSize({ 0.1f * label->getAspect(), 0.1f, });
	quad->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	quad->setSprite(label->getSprite());
	quad->update();
	
	// blue: <#0000ff>
	// incomplete: <#542>
	// faulty: <#5f>>>>>>>>>
	// with 0x: <#0x4354>
	// negative: <#-43531>

	
	// start
	resize(window->getSize());
	window->getGameloop().start(30); // print the average frametime 30 times in a second

	// closing
	delete font;
	delete pack;
	delete shader;
	engine.destroyRenderer(renderer);
	engine.destroyWindow(window);

	return 0;
}
