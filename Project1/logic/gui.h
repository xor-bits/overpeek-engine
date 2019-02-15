#pragma once

#include "database.h"

#define M_COLOR_HEALTH_BAR glm::vec4(1.0, 0.2, 0.2, 1.0)
#define M_COLOR_STAMINA_BAR glm::vec4(0.2, 1.0, 0.2, 1.0)
#define M_COLOR_BLACK glm::vec4(0.0, 0.0, 0.0, 1.0)

#define GUI_FRAME_LOGGER_SIZE 200
#define GUI_UPDATE_LOGGER_SIZE 200
#define GUI_FRAME_LOGGER_BAR_WIDTH 0.005

namespace oe { class Window; class Shader; class Renderer; }
class Gui {

private:
	int m_selectedButton;

	long int m_currentFrame;
	float m_frame_logger[GUI_FRAME_LOGGER_SIZE];
	float m_slowest_frame;

	long int m_currentUpdate;
	float m_update_logger[GUI_UPDATE_LOGGER_SIZE];
	float m_slowest_update;

public:
	Gui(float maxHealth, float maxStamina, float healthGainRate, float staminaGainRate);

	void renderBlur(oe::Renderer *renderer_blurred);
	void renderNoBlur(oe::Renderer *renderer);
	void update();
	void keyPress(int key, int action);
};