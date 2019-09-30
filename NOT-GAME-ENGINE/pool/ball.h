#pragma once

#define BALL_RADIUS 0.1f

#define X_SIDE	9.0f / 6.0f
#define Y_SIDE	1.0f


namespace oe { class Renderer; }
class ball
{
private:
	glm::vec2 v_pos;
	glm::vec2 v_vel;
	glm::vec2 v_acc;

	unsigned short us_index;

public:
	ball();
	~ball();

	void update(int ups);
	void render(float divider, oe::Renderer *renderer);

	void collision(ball* other);

	inline void setPosX(float x) { v_pos.x = x; }
	inline void setPosY(float y) { v_pos.y = y; }
	inline void setVelX(float x) { v_vel.x = x; }
	inline void setVelY(float y) { v_vel.y = y; }
	inline void setAccX(float x) { v_acc.x = x; }
	inline void setAccY(float y) { v_acc.y = y; }

	inline void setIndex(unsigned short i) { us_index = i; }

	inline float getPosX() { return v_pos.x; }
	inline float getPosY() { return v_pos.y; }
	inline float getVelX() { return v_vel.x; }
	inline float getVelY() { return v_vel.y; }
	inline float getAccX() { return v_acc.x; }
	inline float getAccY() { return v_acc.y; }

	inline unsigned short getIndex() { return us_index; }
};

