#pragma once

#include "object.h"


class circle : public object {
private:
	float r;

public:
	circle(float _x, float _y, float _r, float _a, bool _isStatic, b2World* _world) : object(_x, _y, _a, _isStatic, _world) {
		r = _r;

		b2CircleShape* shape = new b2CircleShape();
		shape->m_radius = _r;
		b2Fixture* fixture = body->CreateFixture(shape, 1.0f);
		fixture->SetRestitution(0.8f);
		fixture->SetFriction(0.0f);
		fixture->SetDensity(1.0f);
	}

	circle(float _x, float _y, float _r, bool _isStatic, b2World* _world) : circle(_x, _y, _r, 0.0f, _isStatic, _world) {}

	~circle() {}


	void render(oe::Renderer* renderer) {
		if (!body) return;

		float angle = body->GetAngle();
		glm::vec2 render_position = glm::vec2(worldToScreen(body->GetPosition().x), worldToScreen(body->GetPosition().y));
		float radius = worldToScreen(r);

		setColor();

		//Submit
		renderer->triangleRenderer->submitSphere(render_position.x, render_position.y, radius * 2.0f, 10, color, 20);
	}
};