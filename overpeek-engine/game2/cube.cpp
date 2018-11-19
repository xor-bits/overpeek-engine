#include "cube.h"

Cube::Cube(float x, float y, float w, float h, float angle, bool dynamic, b2World *world) {
	m_size = glm::vec2(w, h);
	m_world = world;
	if (dynamic) {
		b2BodyDef bodyDef;
		b2PolygonShape dynamicBox;
		b2FixtureDef fixtureDef;

		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.angle = angle;
		m_body = m_world->CreateBody(&bodyDef);
		dynamicBox.SetAsBox(w / 4.0, h / 4.0);
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		m_body->CreateFixture(&fixtureDef);
	}
	else {
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(x, y);
		groundBodyDef.angle = angle;
		m_body = m_world->CreateBody(&groundBodyDef);
		b2PolygonShape groundBox;
		groundBox.SetAsBox(w / 4.0, h / 4.0);
		m_body->CreateFixture(&groundBox, 0.0f);
	}
}

Cube::Cube(float x, float y, float w, float h, float angle, bool dynamic, b2World *world, uint16 category, uint16 mask) {
	m_size = glm::vec2(w, h);
	m_world = world;
	if (dynamic) {
		b2BodyDef bodyDef;
		b2PolygonShape dynamicBox;
		b2FixtureDef fixtureDef;

		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.angle = angle;
		m_body = m_world->CreateBody(&bodyDef);
		dynamicBox.SetAsBox(w / 4.0, h / 4.0);

		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.filter.categoryBits = category;
		fixtureDef.filter.maskBits = mask;

		m_body->CreateFixture(&fixtureDef);
	}
	else {
		b2BodyDef groundBodyDef;
		b2PolygonShape groundBox;
		b2FixtureDef fixtureDef;

		groundBodyDef.type = b2_staticBody;
		groundBodyDef.position.Set(x, y);
		groundBodyDef.angle = angle;
		m_body = m_world->CreateBody(&groundBodyDef);
		groundBox.SetAsBox(w / 4.0, h / 4.0);

		fixtureDef.shape = &groundBox;
		fixtureDef.density = 0.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.filter.categoryBits = category;
		fixtureDef.filter.maskBits = mask;

		m_body->CreateFixture(&fixtureDef);
	}
}

Cube::~Cube() {
	m_world->DestroyBody(m_body);
}

void Cube::render(graphics::Renderer *renderer, float zoom, glm::vec2 offset) {
	renderer->renderBoxCentered((m_body->GetPosition().x + offset.x) / zoom, (m_body->GetPosition().y + offset.y) / zoom, m_size.x / zoom, m_size.y / zoom, m_body->GetAngle(), 1);
}