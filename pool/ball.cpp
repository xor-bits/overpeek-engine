#include "pch.h"

#include "ball.h"


ball::ball() {
	v_pos = glm::vec2(0.0f, 0.0f);
	v_vel = glm::vec2(0.0f, 0.0f);
	v_acc = glm::vec2(0.0f, 0.0f);

	us_index = 0;
}

ball::~ball() {
	
}

void ball::render(float divider, oe::Renderer* renderer) {
	glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);

	if (us_index == 0                  ) color = OE_COLOR_WHITE;
	if (us_index == 1 || us_index == 9 ) color = OE_COLOR_YELLOW;
	if (us_index == 2 || us_index == 10) color = OE_COLOR_BLUE;
	if (us_index == 3 || us_index == 11) color = OE_COLOR_RED;
	if (us_index == 4 || us_index == 12) color = OE_COLOR_PURPLE;
	if (us_index == 5 || us_index == 13) color = OE_COLOR_ORANGE;
	if (us_index == 6 || us_index == 14) color = OE_COLOR_GREEN;
	if (us_index == 7 || us_index == 15) color = OE_COLOR_DARKRED;
	if (us_index == 8                  ) color = OE_COLOR_BLACK;

	renderer->triangleRenderer->submitSphere(v_pos.x, v_pos.y, BALL_RADIUS, 16, color, 20);
	if (us_index <= 8) {} //SOLIDS
	else { //STRIPES
		renderer->triangleRenderer->submitSphere(v_pos.x, v_pos.y, BALL_RADIUS / 1.5f, 14, OE_COLOR_WHITE , 20);
	}
	renderer->fontRenderer->renderText(glm::vec3(v_pos.x, v_pos.y - BALL_RADIUS / 2.0f, 0.0f), glm::vec2(BALL_RADIUS), std::to_string(us_index).c_str(), oe::center);
}

void ball::update(int ups) {
	v_pos += v_vel;
	v_vel += v_acc;
	v_acc = glm::vec2(0.0f, 0.0f);

	//Friction
	v_vel *= 1.0f - (1.5f / (float)ups);

	if (v_pos.x > +X_SIDE) v_vel.x *= -1;
	if (v_pos.x < -X_SIDE) v_vel.x *= -1;
	if (v_pos.y > +Y_SIDE) v_vel.y *= -1;
	if (v_pos.y < -Y_SIDE) v_vel.y *= -1;

	v_pos.x = oe::clamp(v_pos.x, -X_SIDE, X_SIDE);
	v_pos.y = oe::clamp(v_pos.y, -Y_SIDE, Y_SIDE);
}

void ball::collision(ball* other)
{
	// Check whether there actually was a collision
	if (this == other)
		return;

	glm::vec2 collision = v_pos - other->v_pos;
	float distance = collision.x * collision.x + collision.y * collision.y;
	if (distance == 0.0 || distance > BALL_RADIUS * BALL_RADIUS) return;

	distance = sqrt(distance);

	// Get the components of the velocity vectors which are parallel to the collision.
	// The perpendicular component remains the same for both fish
	collision.x /= distance;
	collision.y /= distance;
	double aci = dot(v_vel, collision);
	double bci = dot(other->v_vel, collision);

	// Solve for the new velocities using the 1-dimensional elastic collision equations.
	// Turns out it's really simple when the masses are the same.
	double acf = bci;
	double bcf = aci;

	// Replace the collision velocity components with the new ones
	v_vel += float(acf - aci) * collision;
	other->v_vel += float(bcf - bci) * collision;




	collision = v_pos - other->v_pos;
	// minimum translation distance to push balls apart after intersecting
	glm::vec2 mtd = collision * float((BALL_RADIUS - distance) / distance);

	// push-pull them apart based off their mass
	v_pos += mtd * (1.0f / 2.0f);
	other->v_pos -= mtd * (1.0f / 2.0f);
}