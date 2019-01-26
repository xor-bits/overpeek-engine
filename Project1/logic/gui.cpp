#include "../pch.h"

#include "gui.h"
#include "game.h"

Gui::Gui(float maxHealth, float maxStamina, float healthGainRate, float staminaGainRate) {
	m_maxHealth = maxHealth;
	m_maxStamina = maxStamina;
	m_healthGainRate = healthGainRate;
	m_staminaGainRate = staminaGainRate;

	m_currentFrame = 0;
	m_currentUpdate = 0;
	m_staminaRegenCooldown = 0; 
	m_selectedButton = 0;

	resetHealth();
	resetStamina();

	for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
		m_frame_logger[i] = 0;
	}
}

void Gui::render(graphics::Renderer *renderer_blurred, graphics::Renderer *renderer) {
	renderer_blurred->renderBox(Game::getWindow()->getAspect() - 0.6, -0.9, 0.5, 0.05, 0, 24, M_COLOR_BLACK);
	renderer_blurred->renderBox(Game::getWindow()->getAspect() - 0.595, -0.895, 0.49 * (m_playerHealth / m_maxHealth), 0.04, 0, 24, M_COLOR_HEALTH_BAR);

	renderer_blurred->renderBox(Game::getWindow()->getAspect() - 0.6, -0.84, 0.5, 0.05, 0, 24, M_COLOR_BLACK);
	renderer_blurred->renderBox(Game::getWindow()->getAspect() - 0.595, -0.835, 0.49 * (m_playerStamina / m_maxStamina), 0.04, 0, 24, M_COLOR_STAMINA_BAR);
	
	if (Game::advancedDebugMode) {
		m_currentFrame++;
		if (m_currentFrame >= GUI_FRAME_LOGGER_SIZE) m_currentFrame = 0;
		m_frame_logger[m_currentFrame] = Game::getLoop()->getFMS();
		if (m_frame_logger[m_currentFrame] > m_slowest_frame) m_slowest_frame = m_frame_logger[m_currentFrame];

		renderer->renderBox(-Game::getWindow()->getAspect(), 0.005, GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.01, 0.0, 24, glm::vec4(0.0, 1.0, 0.0, 1.0));
		renderer->renderText(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE / 2.0 - 1)),
			-0.02, 0.05, 0.05, 0.0, "frame micro-s: " + std::to_string(m_slowest_frame), glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_TOP);


		renderer->renderBox(Game::getWindow()->getAspect() - GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.005, GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.01, 0.0, 24, glm::vec4(0.0, 1.0, 0.0, 1.0));
		renderer->renderText(Game::getWindow()->getAspect() - (GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE / 2.0 - 1)),
			-0.02, 0.05, 0.05, 0.0, "update micro-s: " + std::to_string(m_slowest_update), glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_TOP);
		
		for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
			renderer->renderBox(Game::getWindow()->getAspect() - (GUI_FRAME_LOGGER_BAR_WIDTH * (i + 1)), 1.0 - m_frame_logger[i] / m_slowest_frame, GUI_FRAME_LOGGER_BAR_WIDTH, m_frame_logger[i] / m_slowest_frame, 0, 24, M_COLOR_BLACK);
		}
		for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
			renderer->renderBox(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * i), 1.0 - m_frame_logger[i] / m_slowest_frame, GUI_FRAME_LOGGER_BAR_WIDTH, m_frame_logger[i] / m_slowest_frame, 0, 24, M_COLOR_BLACK);
		}
	}

	if (Game::paused) {

		if (m_selectedButton > 1) m_selectedButton = 0;
		if (m_selectedButton < 0) m_selectedButton = 1;

		float textScale = 0.1;

		std::string text = "PAUSED";
		renderer->renderText(0.003, -0.003 - 0.75, textScale, textScale, 0, text, glm::vec4(0.0, 0.0, 0.0, 1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_CENTER);
		renderer->renderText(-0.003, 0.003 - 0.75, textScale, textScale, 0, text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_CENTER);

		//
		if (m_selectedButton == 0) {
			renderer->renderBoxCentered(0.0, -0.25, 0.8, 0.1, 0.0, 24, glm::vec4(0.3, 0.3, 0.3, 1.0));
			renderer->renderBoxCentered(0.0, -0.175, 0.8, 0.1, 0.0, 24, glm::vec4(0.3, 0.3, 0.3, 0.5));
		}
		else {
			renderer->renderBoxCentered(0.0, -0.25, 0.8, 0.1, 0.0, 24, glm::vec4(0.3, 0.3, 0.3, 0.5));
			renderer->renderBoxCentered(0.0, -0.175, 0.8, 0.1, 0.0, 24, glm::vec4(0.3, 0.3, 0.3, 1.0));
		}

		textScale = 0.05;
		text = "RESUME";
		renderer->renderText(0.003, -0.003 - 0.5, textScale, textScale, 0, text, glm::vec4(0.0, 0.0, 0.0, 1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_CENTER);
		renderer->renderText(-0.003, 0.003 - 0.5, textScale, textScale, 0, text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_CENTER);

		textScale = 0.05;
		text = "SAVE AND EXIT";
		renderer->renderText(0.003, -0.003 - 0.35, textScale, textScale, 0, text, glm::vec4(0.0, 0.0, 0.0, 1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_CENTER);
		renderer->renderText(-0.003, 0.003 - 0.35, textScale, textScale, 0, text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_CENTER);
	}
}

void Gui::update() {
	m_playerHealth += m_healthGainRate;

	if (m_staminaRegenCooldown > 200) m_playerStamina += m_staminaGainRate;
	else m_staminaRegenCooldown++;

	if (m_playerHealth > m_maxHealth) m_playerHealth = m_maxHealth;
	if (m_playerHealth < 0) m_playerHealth = 0;
	if (m_playerStamina > m_maxStamina) m_playerStamina = m_maxStamina;
	if (m_playerStamina < 0) m_playerStamina = 0;


	if (Game::advancedDebugMode) {
		m_currentUpdate++;
		if (m_currentUpdate >= GUI_UPDATE_LOGGER_SIZE) m_currentUpdate = 0;
		m_update_logger[m_currentUpdate] = Game::getLoop()->getUMS();
		if (m_update_logger[m_currentUpdate] > m_slowest_update) m_slowest_update = m_update_logger[m_currentUpdate];
	}
}

void Gui::keyPress(int key, int action) {
	if (key == GLFW_KEY_DOWN) { m_selectedButton++; return; }
	if (key == GLFW_KEY_UP) { m_selectedButton--; return; }
	if (key == GLFW_KEY_ENTER) {
		if (m_selectedButton == 0) {
			Game::paused = false;
		}
		if (m_selectedButton == 1) {
			Game::getLoop()->stop();
		}
		return;
	}
}