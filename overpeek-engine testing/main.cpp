#include "../overpeek-engine/engine.h"

#define PI	3.141f



oe::Shader* shader;
oe::Renderer *renderer;
oe::GameLoop *gameloop;

float xyangle = PI / 2.0f;
float zangle = 0.0f;

glm::vec3 pos = glm::vec3(0.0f, 0.0f, -1.0f);
//float box_vel_x = 1;



void render(float corrector) {
	oe::Window::clear();

	// quad
	renderer->clear();

	renderer->begin();
	renderer->submit(glm::vec2(-0.5f, -0.5f), glm::vec2(1.0f, 1.0f), 0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), oe::topLeft, 0.0f);
	renderer->end();

	renderer->draw(10);
	
	oe::Window::update();
	oe::Window::input();
	if (oe::Window::shouldClose()) gameloop->stop();
}

int n = 0;
void update() {
	if (n++%100==99) oe::Logger::info("FPS: " + std::to_string(gameloop->getFPS()));
	//box_x += box_vel_x / 5.0f;
	//
	//if (box_x >  1.0) box_vel_x *= -1;
	//if (box_x < -1.0) box_vel_x *= -1;
}

int main() {
	oe::Logger::setup();

	oe::Logger::info("info {}", 0, 1);
	oe::Logger::debug("debug");
	oe::Logger::critical("critical");
	oe::Logger::warning("warning");
	oe::Logger::error("error");
	system("pause");
	exit(0);

	oe::Window::init(900, 600, "overpeek-engine tester", WINDOW_MULTISAMPLE_X2 | WINDOW_GL_DEBUG);
	shader = new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl");
	gameloop = new oe::GameLoop(render, update, 60);
	renderer = new oe::Renderer(oe::types::dynamicDraw, oe::types::staticDraw, 1);

	// pr matrix
	glm::mat4 pr = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f);
	shader->SetUniformMat4("matrix", pr);

	oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	oe::Window::setBackFaceCulling(false);
	oe::Window::setClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//oe::Window::showCursor(false);
	oe::Window::setSwapInterval(0);
	
	// start
	gameloop->start();

	// closing
	oe::Window::close();
	delete shader;
	delete gameloop;
	delete renderer;

	return 0;
}