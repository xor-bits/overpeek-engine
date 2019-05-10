#pragma once

#include "object.h"


class triangle : public object {
private:
	float r;

public:
	triangle(float _x, float _y, float _r, float _a, bool _isStatic, b2World* _world) : object(_x, _y, _a, _isStatic, _world) {
		r = _r;

		b2PolygonShape* shape = new b2PolygonShape();
		b2Vec2 points[3];
		
		glm::vec2 p(0.0f, _r);
		points[0] = b2Vec2(p.x, p.y);
		oe::rotatePoint(glm::vec2(0.0f), pi / 3.0f * 2.0f, p);
		points[1] = b2Vec2(p.x, p.y);
		oe::rotatePoint(glm::vec2(0.0f), pi / 3.0f * 2.0f, p);
		points[2] = b2Vec2(p.x, p.y);
		shape->Set(points, 3);
		//shape->m_radius = _r;

		b2Fixture* fixture = body->CreateFixture(shape, 1.0f);
		fixture->SetRestitution(0.8f);
		fixture->SetFriction(0.0f);
		fixture->SetDensity(1.0f);
	}

	triangle(float _x, float _y, float _r, bool _isStatic, b2World* _world) : triangle(_x, _y, _r, 0.0f, _isStatic, _world) {}

	~triangle() {}


	void render(oe::Renderer* renderer) {
		if (!body) return;

		float angle = body->GetAngle();
		glm::vec2 render_position = glm::vec2(worldToScreen(body->GetPosition().x), worldToScreen(body->GetPosition().y));
		float radius = worldToScreen(r);

		setColor();

		//Vertices
		glm::vec2 p(render_position.x, render_position.y + radius);

		oe::rotatePoint(render_position, angle, p);
		oe::VertexData a = oe::VertexData(glm::vec3(p, 0.0f), glm::vec2(0.0f, 1.0f), 20, color);
		
		oe::rotatePoint(render_position, pi / 3.0f * 2.0f, p);
		oe::VertexData b = oe::VertexData(glm::vec3(p, 0.0f), glm::vec2(0.0f, 1.0f), 20, color);
		
		oe::rotatePoint(render_position, pi / 3.0f * 2.0f, p);
		oe::VertexData c = oe::VertexData(glm::vec3(p, 0.0f), glm::vec2(0.0f, 1.0f), 20, color);

		//Submit
		renderer->triangleRenderer->submitVertex(a);
		renderer->triangleRenderer->submitVertex(b);
		renderer->triangleRenderer->submitVertex(c);
	}
};