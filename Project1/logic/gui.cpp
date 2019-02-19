#include "../pch.h"

#include "gui.h"
#include "game.h"
#include "../creatures/player.h"

Gui::Gui(float maxHealth, float maxStamina, float healthGainRate, float staminaGainRate) {
	for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
		m_frame_logger[i] = 0;
	}

	m_currentUpdate = 0;
	m_currentFrame = 0;
}

void Gui::renderBlur(oe::Renderer *renderer) {
	glm::vec2 pos = glm::vec2(Game::getWindow()->getAspect() - 0.6, -0.9);
	glm::vec2 size = glm::vec2(0.5, 0.05);
	renderer->renderPoint(pos, size, 24, M_COLOR_BLACK);
	//->renderBox(pos, size, 24, M_COLOR_BLACK);
	pos = glm::vec2(Game::getWindow()->getAspect() - 0.595, -0.895);
	size = glm::vec2(0.49 * (Game::getPlayer()->getHealth() / Game::getPlayer()->getMaxHealth()), 0.04);
	renderer->renderPoint(pos, size, 24, M_COLOR_HEALTH_BAR);
	//renderer->renderBox(pos, size, 24, M_COLOR_HEALTH_BAR);

	pos = glm::vec2(Game::getWindow()->getAspect() - 0.6, -0.84);
	size = glm::vec2(0.5, 0.05);
	renderer->renderPoint(pos, size, 24, M_COLOR_BLACK);
	//renderer->renderBox(pos, size, 24, M_COLOR_BLACK);
	pos = glm::vec2(Game::getWindow()->getAspect() - 0.595, -0.835);
	size = glm::vec2(0.49 * (Game::getPlayer()->getStamina() / Game::getPlayer()->getMaxStamina()), 0.04);
	renderer->renderPoint(pos, size, 24, M_COLOR_STAMINA_BAR);
	//renderer->renderBox(pos, size, 24, M_COLOR_STAMINA_BAR);
}

void Gui::renderNoBlur(oe::Renderer *renderer) {
	//std::string test = "";
	//for (char i = 32; i < 127; i++)
	//{
	//	test += i;
	//}
	//std::cout << test.c_str() << std::endl;
	//renderer->renderText(glm::vec2(1.0, 0.1), glm::vec2(0.1), test.c_str(), glm::vec3(1.0), oe::center);
	//renderer->renderText(glm::vec2(-1.0, -0.1), glm::vec2(0.1), test.c_str(), glm::vec3(1.0), oe::center);

	float textScale = 0.05;
	float x = -Game::getWindow()->getAspect();
	if (Game::debugMode) {
		//Debug mode text
		std::string text = "FPS: " + std::to_string(Game::getLoop()->getFPS());
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 0)), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::topLeft);
		text = "UPS: " + std::to_string(Game::getLoop()->getUPS());
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 1)), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::topLeft);
	}
	

	if (Game::advancedDebugMode) {
		//Advanced debug mode text
		std::string text = "Position X: " + std::to_string(Game::getPlayer()->getX()) + ", Y: " + std::to_string(Game::getPlayer()->getY());
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 2)), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::topLeft);
		text = "Renderer: " + Game::getWindow()->getRenderer();
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 3)), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::topLeft);

		m_currentFrame++;
		if (m_currentFrame >= GUI_FRAME_LOGGER_SIZE) m_currentFrame = 0;
		m_frame_logger[m_currentFrame] = Game::getLoop()->getFMS();
		if (m_frame_logger[m_currentFrame] > m_slowest_frame) m_slowest_frame = m_frame_logger[m_currentFrame];

		glm::vec2 pos = glm::vec2(-Game::getWindow()->getAspect(), 0.005);
		glm::vec2 size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.01);
		text = "frame micro-s: " + std::to_string((int)m_slowest_frame);
		renderer->renderPoint(pos, size, 24, glm::vec4(0.0, 1.0, 0.0, 1.0));
		renderer->renderText(glm::vec2(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE / 2.0 - 1)), -0.02),
			glm::vec2(0.05, 0.05), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::bottomCenter);


		text = "update micro-s: " + std::to_string((int)m_slowest_update);
		renderer->renderPoint(glm::vec2(Game::getWindow()->getAspect() - GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.005), glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.01), 24, glm::vec4(0.0, 1.0, 0.0, 1.0));
		renderer->renderText(glm::vec2(Game::getWindow()->getAspect() - (GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE / 2.0 - 1)), -0.02), glm::vec2(0.05, 0.05), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::bottomCenter);

		for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
			pos = glm::vec2(Game::getWindow()->getAspect() - (GUI_FRAME_LOGGER_BAR_WIDTH * (i + 1)), 1.0 - m_frame_logger[i] / m_slowest_frame);
			size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH, m_frame_logger[i] / m_slowest_frame);
			renderer->renderPoint(pos, size, 24, M_COLOR_BLACK);
		}
		for (int i = 0; i < GUI_UPDATE_LOGGER_SIZE; i++) {
			pos = glm::vec2(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * i), 1.0 - m_update_logger[i] / m_slowest_update);
			size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH, m_update_logger[i] / m_slowest_update);
			renderer->renderPoint(pos, size, 24, M_COLOR_BLACK);
		}
	}

	if (Game::paused) {

		if (m_selectedButton > 1) m_selectedButton = 0;
		if (m_selectedButton < 0) m_selectedButton = 1;

		float textScale = 0.1;

		std::string text = "PAUSED";
		renderer->renderText(glm::vec2(0.003, -0.003 - 0.75), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(0.0, 0.0, 0.0, 1.0), oe::center);
		renderer->renderText(glm::vec2(-0.003, 0.003 - 0.75), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::center);

		if (m_selectedButton == 0) {
			renderer->renderBox(glm::vec2(0.0 - 0.4, -0.50 - 0.05), glm::vec2(0.8, 0.1), 24, glm::vec4(0.3, 0.3, 0.3, 1.0));
			renderer->renderBox(glm::vec2(0.0 - 0.4, -0.35 - 0.05), glm::vec2(0.8, 0.1), 24, glm::vec4(0.3, 0.3, 0.3, 0.5));
		}
		else {
			renderer->renderBox(glm::vec2(0.0 - 0.4, -0.50 - 0.05), glm::vec2(0.8, 0.1), 24, glm::vec4(0.3, 0.3, 0.3, 0.5));
			renderer->renderBox(glm::vec2(0.0 - 0.4, -0.35 - 0.05), glm::vec2(0.8, 0.1), 24, glm::vec4(0.3, 0.3, 0.3, 1.0));
		}

		textScale = 0.05;
		text = "RESUME";
		renderer->renderText(glm::vec2(0.003, -0.003 - 0.5), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(0.0, 0.0, 0.0, 1.0), oe::center);
		renderer->renderText(glm::vec2(-0.003, 0.003 - 0.5), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::center);

		textScale = 0.05;
		text = "SAVE AND EXIT";
		renderer->renderText(glm::vec2(0.003, -0.003 - 0.35), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(0.0, 0.0, 0.0, 1.0), oe::center);
		renderer->renderText(glm::vec2(-0.003, 0.003 - 0.35), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::center);
	}
}

void Gui::update() {
	if (Game::advancedDebugMode) {
		m_currentUpdate++;
		if (m_currentUpdate >= GUI_UPDATE_LOGGER_SIZE) m_currentUpdate = 0;
		m_update_logger[m_currentUpdate] = Game::getLoop()->getUMS();
		if (m_update_logger[m_currentUpdate] > m_slowest_update) m_slowest_update = m_update_logger[m_currentUpdate];
	}
}

void Gui::keyPress(int key, int action) {
	if (key == OE_KEY_DOWN) { m_selectedButton++; return; }
	if (key == OE_KEY_UP) { m_selectedButton--; return; }
	if (key == OE_KEY_ENTER) {
		if (m_selectedButton == 0) {
			Game::paused = false;
		}
		if (m_selectedButton == 1) {
			Game::getLoop()->stop();
		}
		return;
	}
}