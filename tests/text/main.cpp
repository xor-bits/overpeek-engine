#include <engine/engine.hpp>

#include <string>



oe::graphics::Instance* instance;
oe::graphics::Window* window;
oe::graphics::SpritePack* pack;
oe::assets::DefaultShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;

void render(float update_fraction) {
	// clear framebuffer
	window->clear();

	// begin submitting
	renderer->begin();
	renderer->clear();

	// submitting
#if 0
	oe::graphics::Text::submit(*renderer, "0@#%()[]{}<>,.;:?!|/\\", { 0.0f, -0.2f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "1234567890 =+-/*", { 0.0f, 0.0f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "The quick brown fox jumps over the lazy dog.", { 0.0f, 0.4f }, 0.14f, oe::graphics::alignment::center_center);
#else
	oe::graphics::Text::submit(*renderer, "blue:<#0000ff> test", { 0.0f, -0.2f }, 0.14f, oe::alignments::center_center);
	oe::graphics::Text::submit(*renderer, "incomplete:<#542> test", { 0.0f, 0.0f }, 0.14f, oe::alignments::center_center);
	oe::graphics::Text::submit(*renderer, "faulty:<#5f>>>>>>>>> test", { 0.0f, 0.2f }, 0.14f, oe::alignments::center_center);
	oe::graphics::Text::submit(*renderer, "with 0x:<#0x4354> test", { 0.0f, 0.4f }, 0.14f, oe::alignments::center_center);
	oe::graphics::Text::submit(*renderer, "negative:<#-43531> test", { 0.0f, 0.6f }, 0.14f, oe::alignments::center_center);
#endif

	// bind texture with font and bind shader
	shader->bind();
	pack->bind();

	// stop submitting and render
	renderer->end();
	renderer->render();

	// swap buffers and poll events
	window->update();

	// check if needs to close
	if (window->shouldClose()) oe::utils::GameLoop::getSingleton().stop();
}

void update() {
	spdlog::info("FPS: {}", oe::utils::GameLoop::getSingleton().getFPS());
}

void resize(const glm::vec2& window_size) {
	float aspect = window->aspect();
	glm::mat4 pr_matrix = glm::ortho(-aspect, aspect, 1.0f, -1.0f);
	shader->setProjectionMatrix(pr_matrix);
	shader->useTexture(true);
}

int main(int argc, char** argv) {
	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	oe::Engine::getSingleton().init(engine_info);

	// instance
	oe::InstanceInfo instance_info = {};
	instance_info.debug_messages = true;
	// instance_info.favored_gpu_vulkan = oe::gpu::dedicated;
	instance = oe::Engine::getSingleton().createInstance(instance_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Test 2 - Colored Text";
	window_info.multisamples = 4;
	window_info.resize_callback = resize;
	window = instance->createWindow(window_info);

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamicrender;
	renderer_info.indexRenderType = oe::types::staticrender;
	renderer_info.max_quad_count = 1000;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = window->createRenderer(renderer_info);

	// shader
	shader = new oe::assets::DefaultShader(window);

	// sprites
	pack = new oe::graphics::SpritePack(window);
	font = new oe::graphics::Font(pack);
	oe::graphics::Text::setFont(*font);
	pack->construct();
	
	// start
	resize(window->getSize());
	oe::utils::GameLoop::getSingleton().start(render, update, 1);

	// closing
	delete font;
	delete pack;
	delete shader;
	window->destroyRenderer(renderer);
	instance->destroyWindow(window);

	return 0;
}
