#pragma once

#include "../graphics/renderer.h"
#include <Box2D/Box2D.h>

class Cube {
public:
	glm::vec2 m_size;
	b2Body *m_body;
	b2World *m_world;

public:
	Cube(float x, float y, float w, float h, float angle, bool dynamic, b2World *world);
	Cube(float x, float y, float w, float h, float angle, bool dynamic, b2World *world, uint16 category, uint16 mask);
	~Cube();

	void render(graphics::Renderer *renderer, float zoom, glm::vec2 offset);
};