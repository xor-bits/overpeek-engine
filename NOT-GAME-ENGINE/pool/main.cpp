#include "pch.h"

#include "ball.h"

#define WINDOW_WIDTH	900
#define WINDOW_HEIGHT	600
#define UPS				1000
#define BALL_COUNT		15
#define SHOT_POWER		5


oe::Window *window;
oe::GameLoop *loop;
oe::Shader *shader;
oe::Renderer *renderer;

ball* balls = new ball[BALL_COUNT];
ball* cue = new ball;

glm::vec2 mouse_drag_start;

void render(float divider) {
	if (window->getKey(OE_KEY_ESCAPE)) loop->stop();

	window->clear();

	//renderer->pointRenderer->submitVertex(oe::VertexData(glm::vec3(0.2f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 20, OE_COLOR_RED));
	//renderer->triangleRenderer->submitSphere(0.0f, 0.0f, 0.5f, 3, OE_COLOR_CYAN, 20);
	for (int i = 0; i < BALL_COUNT; i++) balls[i].render(divider, renderer);
	cue->render(divider, renderer);

	if (window->getButton(OE_MOUSE_BUTTON_LEFT)) {
		double mx, my;
		window->getMousePos(mx, my);
		renderer->lineRenderer->submitVertex(oe::VertexData(glm::vec3(cue->getPosX(), cue->getPosY(), 0.0f), glm::vec2(0.0f, 0.0f), 20, OE_COLOR_ORANGE));
		renderer->lineRenderer->submitVertex(oe::VertexData(glm::vec3(cue->getPosX() - (mx - mouse_drag_start.x), cue->getPosY() - (my - mouse_drag_start.y), 0.0f), glm::vec2(0.0f, 0.0f), 20, OE_COLOR_ORANGE));
	}

	renderer->draw(shader, shader, oe::TextureManager::getTexture(0), true);

	window->update();
	window->input();
}

void update() {
	//Collisions
//#pragma omp parallel for
	for (int i = 0; i < BALL_COUNT; i++) {
		for (int j = i + 1; j < BALL_COUNT; j++)
		{
			if (i == j) continue;

			balls[i].collision(&balls[j]);
		}
		cue->collision(&balls[i]);
	}

	//Update
	cue->update(UPS);
	for (int i = 0; i < BALL_COUNT; i++) {
		balls[i].update(UPS); 
	}
}

void mouseButton(int button, int action) {
	double mx, my;
	window->getMousePos(mx, my);

	if (action == OE_PRESS) {
		mouse_drag_start = glm::vec2(mx, my);
	}
	if (action == OE_RELEASE) {
		cue->setAccX(pow(-(mx - mouse_drag_start.x) * SHOT_POWER, 3));
		cue->setAccY(pow(-(my - mouse_drag_start.y) * SHOT_POWER, 3));
		
		mouse_drag_start = glm::vec2(0.0f, 0.0f);
	}
}

int main() {
	window = new oe::Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TITLE", WINDOW_MULTISAMPLE_X2);
	loop = new oe::GameLoop(render, update, UPS, window);
	renderer = new oe::Renderer("res/font/arial.ttf", window);
	shader = new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl");
	oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	oe::Random::seed();

	glm::mat4 ortho = glm::ortho(-window->getAspect(), window->getAspect(), 1.0f, -1.0f);
	shader->enable(); shader->SetUniformMat4("pr_matrix", ortho);
	window->setPointRadius(3.0f);
	window->setBackFaceCulling(false);
	window->setButtonCallback(mouseButton);

	for (int i = 0; i < BALL_COUNT; i++)
	{
		balls[i].setIndex(i + 1);
		balls[i].setPosX(oe::Random::random(-0.9f, 0.9f));
		balls[i].setPosY(oe::Random::random(-0.9f, 0.9f) / 2.0f);

		balls[i].setAccX(oe::Random::random(-0.01f, 0.01f));
		balls[i].setAccY(oe::Random::random(-0.01f, 0.01f));
	}
	cue->setIndex(0);
	cue->setPosX(0.0f);
	cue->setPosY(0.0f);

	loop->start();
}