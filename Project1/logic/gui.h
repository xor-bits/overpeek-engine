#pragma once

#include "database.h"

#define M_COLOR_HEALTH_BAR glm::vec4(1.0, 0.2, 0.2, 1.0)
#define M_COLOR_STAMINA_BAR glm::vec4(0.2, 1.0, 0.2, 1.0)
#define M_COLOR_BLACK glm::vec4(0.0, 0.0, 0.0, 1.0)

#define GUI_FRAME_LOGGER_SIZE 200
#define GUI_UPDATE_LOGGER_SIZE 200
#define GUI_FRAME_LOGGER_BAR_WIDTH 0.005

namespace graphics { class Window; class Shader; class Renderer; }
class Gui {

private:
	float m_playerHealth;
	float m_playerStamina;

	float m_maxHealth;
	float m_maxStamina;

	float m_healthGainRate;
	float m_staminaGainRate;

	int m_staminaRegenCooldown;
	int m_healthRegenCooldown;

	int m_selectedButton;

	long int m_currentFrame;
	float m_frame_logger[GUI_FRAME_LOGGER_SIZE];
	float m_slowest_frame;

	long int m_currentUpdate;
	float m_update_logger[GUI_UPDATE_LOGGER_SIZE];
	float m_slowest_update;

	void clampHPAndSTA();

public:
	Gui(float maxHealth, float maxStamina, float healthGainRate, float staminaGainRate);

	void renderBlur(graphics::Renderer *renderer_blurred);
	void renderNoBlur(graphics::Renderer *renderer);
	void update();
	void keyPress(int key, int action);

	inline void setHealth(float value) { m_playerHealth = value; m_healthRegenCooldown = 0; clampHPAndSTA(); }
	inline float getHealth() { return m_playerHealth; }
	inline void setStamina(float value) { m_playerStamina = value; m_staminaRegenCooldown = 0; clampHPAndSTA(); }
	inline float getStamina() { return m_playerStamina; }

	inline void setMaxHealth(float value) { m_maxHealth = value; }
	inline float getMaxHealth() { return m_maxHealth; }
	inline void setMaxStamina(float value) { m_maxStamina = value; }
	inline float getMaxStamina() { return m_maxStamina; }

	inline void setHealthGainRate(float value) { m_healthGainRate = value; }
	inline float getsHealthGainRate() { return m_healthGainRate; }
	inline void setStaminaGainRate(float value) { m_staminaGainRate = value; }
	inline float getStaminaGainRate() { return m_staminaGainRate; }

	inline void resetHealth() { m_playerHealth = m_maxHealth; }
	inline void resetStamina() { m_playerStamina = m_maxStamina; }

	inline bool dead() { return m_playerHealth <= 0 ? true : false; }
	inline bool exhausted() { return m_playerStamina <= 0 ? true : false; }
};