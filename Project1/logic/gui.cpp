#include "../pch.h"

#include "gui.h"
#include "game.h"
#include "../creatures/player.h"

#define BUTTONS	3

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
	renderer->renderPoint(pos, size, 20, M_COLOR_BLACK);
	//->renderBox(pos, size, 20, M_COLOR_BLACK);
	pos = glm::vec2(Game::getWindow()->getAspect() - 0.595, -0.895);
	size = glm::vec2(0.49 * (Game::getPlayer()->getHealth() / Game::getPlayer()->getMaxHealth()), 0.04);
	renderer->renderPoint(pos, size, 20, M_COLOR_HEALTH_BAR);
	//renderer->renderBox(pos, size, 20, M_COLOR_HEALTH_BAR);

	pos = glm::vec2(Game::getWindow()->getAspect() - 0.6, -0.84);
	size = glm::vec2(0.5, 0.05);
	renderer->renderPoint(pos, size, 20, M_COLOR_BLACK);
	//renderer->renderBox(pos, size, 20, M_COLOR_BLACK);
	pos = glm::vec2(Game::getWindow()->getAspect() - 0.595, -0.835);
	size = glm::vec2(0.49 * (Game::getPlayer()->getStamina() / Game::getPlayer()->getMaxStamina()), 0.04);
	renderer->renderPoint(pos, size, 20, M_COLOR_STAMINA_BAR);
	//renderer->renderBox(pos, size, 20, M_COLOR_STAMINA_BAR);
}

void Gui::renderNoBlur(oe::Renderer *renderer) {
	float textScale = 0.1;
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
		renderer->renderPoint(pos, size, 20, glm::vec4(0.0, 1.0, 0.0, 1.0));
		renderer->renderText(glm::vec2(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE / 2.0 - 1)), -0.02),
			glm::vec2(0.05, 0.05), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::bottomCenter);


		text = "update micro-s: " + std::to_string((int)m_slowest_update);
		renderer->renderPoint(glm::vec2(Game::getWindow()->getAspect() - GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.005), glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.01), 20, glm::vec4(0.0, 1.0, 0.0, 1.0));
		renderer->renderText(glm::vec2(Game::getWindow()->getAspect() - (GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE / 2.0 - 1)), -0.02), glm::vec2(0.05, 0.05), text.c_str(), glm::vec4(1.0, 1.0, 1.0, 1.0), oe::bottomCenter);

		for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
			pos = glm::vec2(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * i), 1.0 - m_frame_logger[i] / m_slowest_frame);
			size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH, m_frame_logger[i] / m_slowest_frame);
			renderer->renderPoint(pos, size, 20, M_COLOR_BLACK);
		}
		for (int i = 0; i < GUI_UPDATE_LOGGER_SIZE; i++) {
			pos = glm::vec2(Game::getWindow()->getAspect() - (GUI_FRAME_LOGGER_BAR_WIDTH * (i + 1)), 1.0 - m_update_logger[i] / m_slowest_update);
			size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH, m_update_logger[i] / m_slowest_update);
			renderer->renderPoint(pos, size, 20, M_COLOR_BLACK);
		}
	}

	if (Game::paused) {

		if (m_selectedButton > BUTTONS - 1) m_selectedButton = 0;
		if (m_selectedButton < 0) m_selectedButton = BUTTONS - 1;

		float textScale = 0.2;

		std::string text = "PAUSED";
		renderer->renderText(glm::vec2(0.003, -0.003 - 0.75), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0), oe::center);

		for (int i = 0; i < 3; i++)
		{
			float shade = 0.1f;
			if (i == m_selectedButton) shade = 0.2f;
			renderer->renderPoint(glm::vec2(0.0 - 0.4, -0.50 - 0.05 + (i * 0.15)), glm::vec2(0.8, 0.1), 20, glm::vec4(0.0, 0.0, 0.0, shade));
		}

		textScale = 0.05;
		text = "RESUME";
		renderer->renderText(glm::vec2(0.0, -0.5), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0), oe::center);

		textScale = 0.05;
		text = "SETTINGS";
		renderer->renderText(glm::vec2(0.0, -0.35), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0), oe::center);

		textScale = 0.05;
		text = "SAVE AND EXIT";
		renderer->renderText(glm::vec2(0.0, -0.20), glm::vec2(textScale, textScale), text.c_str(), glm::vec4(1.0), oe::center);
	}

	if (m_chat_opened || m_chat_opened_timer > 0) {
		if (m_chat_opened) renderer->renderPoint(glm::vec2(-Game::getWindow()->getAspect(), 1.0 - textScale), glm::vec2(Game::getWindow()->getAspect() * 2, textScale), 20, glm::vec4(0.0, 0.0, 0.0, 0.2));
		for (int i = 0; i < m_text_lines.size(); i++)
		{
			renderer->renderText(glm::vec2(-Game::getWindow()->getAspect(), 1.0 - ((i + 1.5) * textScale * 1.2)), glm::vec2(textScale, textScale), m_text_lines[m_text_lines.size() - 1 - i].c_str(), glm::vec4(1.0), oe::bottomLeft);
		}
		renderer->renderText(glm::vec2(-Game::getWindow()->getAspect(), 1.0), glm::vec2(textScale, textScale), m_current_line.c_str(), glm::vec4(1.0), oe::bottomLeft);
	}
}

void Gui::addChatLine(std::string text) {
	m_chat_opened_timer = 500;
	m_text_lines.push_back(text);
	if (m_text_lines.size() > MAX_TEXT_LINES) m_text_lines.erase(m_text_lines.begin());
}

void Gui::update() {
	m_chat_opened_timer--;
	if (Game::advancedDebugMode) {
		m_currentUpdate++;
		if (m_currentUpdate >= GUI_UPDATE_LOGGER_SIZE) m_currentUpdate = 0;
		m_update_logger[m_currentUpdate] = Game::getLoop()->getUMS();
		if (m_update_logger[m_currentUpdate] > m_slowest_update) m_slowest_update = m_update_logger[m_currentUpdate];
	}
}

void Gui::keyPress(int key, int action) {
	if (Game::paused) {
		if (key == OE_KEY_DOWN) { m_selectedButton++; return; }
		if (key == OE_KEY_UP) { m_selectedButton--; return; }
		if (key == OE_KEY_ENTER || key == OE_KEY_SPACE) {
			if (m_selectedButton == 0) {
				Game::paused = false;
				return;
			}
			if (m_selectedButton == 1) {
				oe::Logger::critical("TODO: Open settings!");
				return;
			}
			if (m_selectedButton == 2) {
				Game::getLoop()->stop();
				return;
			}
			return;
		}
	}
	if (key == OE_KEY_T) {
		if (m_chat_opened) return;

		m_chat_opened = true;
		m_chat_just_opened = true;
		//oe::Logger::info("Opened chat");
		return;
	}
	if (key == OE_KEY_ESCAPE && m_chat_opened) {
		m_chat_opened = false;
		m_chat_opened_timer = 500;
		//oe::Logger::info("Closed chat");
		return;
	}
	if (key == OE_KEY_ENTER && m_chat_opened) {
		m_chat_opened = false;
		m_chat_opened_timer = 500;

		//Check if command is typed
		if (m_current_line[0] == '/') {
			int space = m_current_line.find(' ');
			std::string command = m_current_line.substr(1, space - 1);
			std::string arguments = m_current_line.substr(space + 1);
			std::vector<std::string> argumentVec;

			//std::cout << "Command: \"" << command << "\"" << std::endl;
			//std::cout << "Arguments: \"" << arguments << "\"" << std::endl;

			//COMMANDS
			//TP COMMAND
			if (command == "tp") {
				if (space == -1) {
					oe::Logger::warning("No command arguments given");
					addChatLine("No command arguments given");
					addChatLine("Type \"/help\" to see list of commands and their uses");
					goto notcommand;
				}
				std::string leftArguments = arguments;
				int nextArg = 0;
				while (nextArg != -1) {
					nextArg = leftArguments.find(' ');
					std::string thisargument = leftArguments.substr(0, nextArg);
					leftArguments = leftArguments.substr(nextArg + 1);
					argumentVec.push_back(thisargument);
					//std::cout << "\"" << thisargument << "\"" << std::endl;
				}

				if (argumentVec.size() < 2) {
					oe::Logger::warning("Too few arguments");
					addChatLine("Too few arguments");
					goto notcommand;
				}

				//Use arguments
				float posX = stof(argumentVec[0]);
				float posY = stof(argumentVec[1]);

				Game::getPlayer()->setX(posX);
				Game::getPlayer()->setY(posY);

				oe::Logger::info("Player ran command: " + m_current_line);
			}

			//HELP COMMAND
			if (command == "help") {
				addChatLine("--List of commands--");
				addChatLine("/tp [x] [y]");
			}
		}
		else {
			notcommand:
			addChatLine("Player: " + m_current_line);
			oe::Logger::info("Player", m_current_line.c_str());
		}

		m_current_line = "";
		return;
	}
	if (key == OE_KEY_BACKSPACE) {
		if (m_current_line.length() > 0) m_current_line.erase(m_current_line.end() - 1);
		//oe::Logger::info("Erased last character");
		return;
	}
}

void Gui::typing(unsigned int codepoint, int mods) {
	///TODO: IMPROVE
	if (m_chat_opened) {
		if (m_chat_just_opened) {
			m_chat_just_opened = false;
			return;
		}
		else {
			m_current_line += (char)codepoint;
		}
	}
}