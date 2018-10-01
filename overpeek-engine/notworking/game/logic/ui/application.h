#pragma once

class Application {
protected:
	float m_x, m_y;
	float m_w, m_h;

public:
	virtual void render();
	virtual void update();

};