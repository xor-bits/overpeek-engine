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
	label->generate("AÄäääääj testteets", window);
	auto quad = renderer->createQuad();
	quad->setPosition({ -1.0f, 0.0f });
	quad->setSize({ 0.1f * label->getAspect(), 0.1f, });
	quad->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	quad->setSprite(label->getSprite());
	quad->update();
#if 0
	oe::graphics::Text::submit(renderer, "0@#%()[]{}<>,.;:?!|/\\", { 0.0f, -0.2f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(renderer, "1234567890 =+-/*", { 0.0f, 0.0f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(renderer, "The quick brown fox jumps over the lazy dog.", { 0.0f, 0.4f }, 0.14f, oe::graphics::alignment::center_center);
#else
	// oe::graphics::Text::submit(renderer, "blue:<#0000ff> test", { 0.0f, -0.2f }, 0.14f, oe::alignments::center_center);
	// oe::graphics::Text::submit(renderer, "incomplete:<#542> test", { 0.0f, 0.0f }, 0.14f, oe::alignments::center_center);
	// oe::graphics::Text::submit(renderer, "faulty:<#5f>>>>>>>>> test", { 0.0f, 0.2f }, 0.14f, oe::alignments::center_center);
	// oe::graphics::Text::submit(renderer, "with 0x:<#0x4354> test", { 0.0f, 0.4f }, 0.14f, oe::alignments::center_center);
	// oe::graphics::Text::submit(renderer, "negative:<#-43531> test", { 0.0f, 0.6f }, 0.14f, oe::alignments::center_center);
#endif

	
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
