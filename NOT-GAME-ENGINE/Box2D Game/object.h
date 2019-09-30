#pragma once

#include "../overpeek-engine/engine.h"
#include <Box2D/Box2D.h>


const float pi = glm::pi<float>();
const float root2 = sqrt(2.0f);


class object {
protected:
	b2World* world;
	b2Body* body;
	b2BodyDef* bd;
	glm::vec4 color;

public:
	object(float _x, float _y, float _a, bool _isStatic, b2World* _world) {
		world = _world;

		color = oe::hsv2rgb(glm::vec4(oe::Random::random(0.0f, 255.0f), 1.0f, 1.0f, 1.0f));
	
		bd = new b2BodyDef;
		bd->type = !_isStatic ? b2_dynamicBody : b2_staticBody;
		bd->position.x = _x;
		bd->position.y = _y;
		bd->angle = _a;
		body = _world->CreateBody(bd);
	}

	~object() {
		world->DestroyBody(body);
	}


	virtual void render(oe::Renderer* renderer) = 0;

	float getX() { return body->GetPosition().x; }
	float getY() { return body->GetPosition().y; }
	float getVelX() { body->GetLinearVelocity().x; }
	float getVelY() { body->GetLinearVelocity().y; }
	void setPos(float x, float y) { body->SetTransform(b2Vec2(x, y), body->GetAngle()); }

	static float worldToScreen(float pos) { return pos / 10.0f; }
	static float screenToWorld(float pos) { return pos * 10.0f; }

	void setColor() { color = oe::hsv2rgb(glm::vec4(oe::map(getX(), -10.0f, 10.0f, 0.0f, 255.0f), 1.0f, 1.0f, 1.0f)); }
};