#pragma once

#include "object.h"


class box : public object {
private:
	glm::vec2 size;

public:
	box(float _x, float _y, float _w, float _h,float _a, bool _isStatic, b2World* _world) : object(_x, _y, _a, _isStatic, _world) {
		size = glm::vec2(_w, _h);

		b2PolygonShape* shape = new b2PolygonShape();
		shape->SetAsBox(_w, _h);
		b2Fixture* fixture = body->CreateFixture(shape, 1.0f);
		fixture->SetRestitution(0.8f);
		fixture->SetFriction(0.0f);
		fixture->SetDensity(1.0f);
	}

	box(float _x, float _y, float _w, float _h, bool _isStatic, b2World* _world) : box(_x, _y, _w, _h, 0.0f, _isStatic, _world) {}

	~box() {}


	void render(oe::Renderer* renderer) {
		if (!body) return;

		float angle = body->GetAngle();
		glm::vec2 render_position = glm::vec2(worldToScreen(body->GetPosition().x), worldToScreen(body->GetPosition().y));
		glm::vec2 render_size = glm::vec2(worldToScreen(size.x), worldToScreen(size.y));

		setColor();

		//Vertices
		glm::vec2 p(render_position.x - render_size.x, render_position.y - render_size.y);
		oe::rotatePoint(render_position, angle, p);
		oe::VertexData a = oe::VertexData(glm::vec3(p, 0.0f), glm::vec2(0.0f, 1.0f), 20, color);
		p = glm::vec2(render_position.x - render_size.x, render_position.y + render_size.y);
		oe::rotatePoint(render_position, angle, p);
		oe::VertexData b = oe::VertexData(glm::vec3(p, 0.0f), glm::vec2(0.0f, 1.0f), 20, color);
		p = glm::vec2(render_position.x + render_size.x, render_position.y + render_size.y);
		oe::rotatePoint(render_position, angle, p);
		oe::VertexData c = oe::VertexData(glm::vec3(p, 0.0f), glm::vec2(0.0f, 1.0f), 20, color);
		p = glm::vec2(render_position.x + render_size.x, render_position.y - render_size.y);
		oe::rotatePoint(render_position, angle, p);
		oe::VertexData d = oe::VertexData(glm::vec3(p, 0.0f), glm::vec2(0.0f, 1.0f), 20, color);

		//Submit
		renderer->quadRenderer->submitVertex(a);
		renderer->quadRenderer->submitVertex(b);
		renderer->quadRenderer->submitVertex(c);
		renderer->quadRenderer->submitVertex(d);
	}
};