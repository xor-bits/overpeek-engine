#include "../pch.h"

#include "gui.h"
#include "game.h"
#include "../world/map.h"
#include "../creatures/player.h"

#include <cmath>

#define BUTTONS	3

Gui::Gui(float maxHealth, float maxStamina, float healthGainRate, float staminaGainRate) {
	m_frame_logger = new float[GUI_FRAME_LOGGER_SIZE];
	m_update_logger = new float[GUI_FRAME_LOGGER_SIZE];

	for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
		m_frame_logger[i] = 0;
		m_update_logger[i] = 0;
	}

	m_current_input_history = 0;
	m_chat_just_opened = false;
	m_chat_opened = false;
	m_chat_opened_timer = 0.0f;
	m_currentUpdate = 0;
	m_currentFrame = 0;
	m_selectedButton = 0;
	m_selected_logger = 0;
	m_avg_frame = 0.0f;
	m_avg_update = 0.0f;
}

char *GetProcessorName()
{
	int CPUInfo[4] = { -1 };
	char *CPUBrandString = new char[0x40];
	__cpuid(CPUInfo, 0x80000000);
	unsigned int nExIds = CPUInfo[0];

	memset(CPUBrandString, 0, sizeof(CPUBrandString));

	// Get the information associated with each extended ID.
	for (int i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string.
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}

	return CPUBrandString;
}

void Gui::renderBlur(oe::Renderer *renderer) {
	//Health bar
	glm::vec3 pos = glm::vec3(Game::getWindow()->getAspect() - 0.6f, -1.0f + 0.1f, 0.0f) * Game::renderScale();
	glm::vec2 size = glm::vec2(0.5, 0.05) * Game::renderScale();
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, OE_COLOR_BLACK));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, OE_COLOR_BLACK));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, OE_COLOR_BLACK));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, OE_COLOR_BLACK));

	pos = glm::vec3(Game::getWindow()->getAspect() - 0.595f, -1.0f + 0.105f, 0.0f) * Game::renderScale();
	size = glm::vec2(0.49 * (Game::getPlayer()->getHealth() / Game::getPlayer()->getMaxHealth()), 0.04) * Game::renderScale();
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, M_COLOR_HEALTH_BAR));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, M_COLOR_HEALTH_BAR));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, M_COLOR_HEALTH_BAR));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, M_COLOR_HEALTH_BAR));

	//Stealth bar
	pos = glm::vec3(Game::getWindow()->getAspect() - 0.6f, -1.0f + 0.16f, 0.0f);
	size = glm::vec2(0.5, 0.05) * Game::renderScale();
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, OE_COLOR_BLACK));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, OE_COLOR_BLACK));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, OE_COLOR_BLACK));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, OE_COLOR_BLACK));

	pos = glm::vec3(Game::getWindow()->getAspect() - 0.595f, -1.0f + 0.165f, 0.0f) * Game::renderScale();
	size = glm::vec2(0.49 * (Game::getPlayer()->getStamina() / Game::getPlayer()->getMaxStamina()), 0.04) * Game::renderScale();
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, M_COLOR_STAMINA_BAR));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, M_COLOR_STAMINA_BAR));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, M_COLOR_STAMINA_BAR));
	renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, M_COLOR_STAMINA_BAR));

	float textScale = 0.08 * Game::renderScale();
	float x = -Game::getWindow()->getAspect();
	if (Game::debugMode) {
		//Debug mode text
		std::string text = "UPS: " + std::to_string(Game::getLoop()->getUPS());
		renderer->fontRenderer->renderText(glm::vec3(x, -1.0f + (textScale * 1), 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft, M_COLOR_TEXT_GB);
	}
	if (Game::advancedDebugMode) {
		//Advanced debug mode text
		char *cpuInfo = GetProcessorName();

		std::string text = "Position X: " + std::to_string(Game::getPlayer()->getX()) + ", Y: " + std::to_string(Game::getPlayer()->getY());
		renderer->fontRenderer->renderText(glm::vec3(x, -1.0f + (textScale * 2), 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft, M_COLOR_TEXT_GB);
		text = "Renderer: " + Game::getWindow()->getRenderer();
		renderer->fontRenderer->renderText(glm::vec3(x, -1.0f + (textScale * 4), 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft, M_COLOR_TEXT_GB);
		text = "CPU: " + std::string(cpuInfo);
		renderer->fontRenderer->renderText(glm::vec3(x, -1.0f + (textScale * 5), 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft, M_COLOR_TEXT_GB);

		m_currentFrame++;
		if (m_currentFrame >= GUI_FRAME_LOGGER_SIZE) m_currentFrame = 0;
		m_frame_logger[m_currentFrame] = Game::getLoop()->getFMS();

		m_avg_frame = 0;
		for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++)
		{
			m_avg_frame += m_frame_logger[i];
		}
		m_avg_frame /= (float)GUI_FRAME_LOGGER_SIZE;

		glm::vec3 pos = glm::vec3(-Game::getWindow()->getAspect(), 2.0f / 3.0f, 0.0f);
		glm::vec2 size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.005f) * Game::renderScale();
		glm::vec3 textPos = glm::vec3(-Game::getWindow()->getAspect(), 2.0f / 3.0f - 0.02f * Game::renderScale(), 0.0f);
		glm::vec2 textSize = glm::vec2(textScale * 0.8f);
		renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, OE_COLOR_GREEN));
		renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, OE_COLOR_GREEN));
		renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, OE_COLOR_GREEN));
		renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, OE_COLOR_GREEN));
		if (m_selected_logger) {
			//Top bar
			text = "frame: " + std::to_string((int)m_avg_frame);
			renderer->fontRenderer->renderText(textPos, textSize, text.c_str(), oe::bottomLeft, M_COLOR_TEXT_GB);

			//Actual logger
			for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
				float bar_height = (m_frame_logger[i] / m_avg_frame) / 3.0;
				pos = glm::vec3(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * Game::renderScale() * i), 1.0 - bar_height, 0.0f);
				size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * Game::renderScale(), bar_height);


				glm::vec4 barColor(max(0.0f, bar_height - 0.5f), max(0.0f, 0.5f - bar_height), 0.0f, 1.0f);

				renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, barColor));
				renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, barColor));
				renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, barColor));
				renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, barColor));
			}
		}
		else {
			text = "update: " + std::to_string((int)m_avg_update);
			renderer->fontRenderer->renderText(textPos, textSize, text.c_str(), oe::bottomLeft, M_COLOR_TEXT_GB);

			for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
				float bar_height = (m_update_logger[i] / m_avg_update) / 3.0;
				pos = glm::vec3(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * Game::renderScale() * i), 1.0 - bar_height, 0.0f);
				size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * Game::renderScale(), bar_height);

				float br = max(0.0f, bar_height);
				glm::vec4 barColor(br, 1.0f - br, 0.0f, 1.0f);

				renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, barColor));
				renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, barColor));
				renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, barColor));
				renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, barColor));
			}
		}
	}
}

void Gui::renderNoBlur(oe::Renderer *renderer) {
	float textScale = 0.08 * Game::renderScale();
	float x = -Game::getWindow()->getAspect();
	if (Game::debugMode) {
		//Debug mode text
		std::string text = "FPS: " + std::to_string(Game::getLoop()->getFPS());
		renderer->fontRenderer->renderText(glm::vec3(x, -1.0, 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft, M_COLOR_TEXT_GB);
	}

	if (Game::paused) {

		if (m_selectedButton > BUTTONS - 1) m_selectedButton = 0;
		if (m_selectedButton < 0) m_selectedButton = BUTTONS - 1;

		float textScale = 0.1 * Game::renderScale();

		std::string text = "PAUSED";
		renderer->fontRenderer->renderText(glm::vec3(0.003, -0.003 - 0.75, 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::center, M_COLOR_TEXT_GB);

		for (int i = 0; i < 3; i++)
		{
			float shade = 0.1f;
			if (i == m_selectedButton) shade = 0.2f;
			glm::vec3 pos = glm::vec3(0.0 - 0.4, -0.50 - 0.05 + (i * 0.15), 0.0f);
			glm::vec2 size = glm::vec2(0.8, 0.1);

			renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, glm::vec4(0.0, 0.0, 0.0, shade)));
			renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, glm::vec4(0.0, 0.0, 0.0, shade)));
			renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, glm::vec4(0.0, 0.0, 0.0, shade)));
			renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, glm::vec4(0.0, 0.0, 0.0, shade)));
		}

		textScale = 0.05;
		text = "RESUME";
		renderer->fontRenderer->renderText(glm::vec3(0.0, -0.5, 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::center);

		textScale = 0.05;
		text = "SETTINGS";
		renderer->fontRenderer->renderText(glm::vec3(0.0, -0.35, 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::center);

		textScale = 0.05;
		text = "SAVE AND EXIT";
		renderer->fontRenderer->renderText(glm::vec3(0.0, -0.20, 0.0f), glm::vec2(textScale, textScale), text.c_str(), oe::center);
	}

	if (m_chat_opened || m_chat_opened_timer > 0) {
		if (m_chat_opened) {
			glm::vec3 pos = glm::vec3(-Game::getWindow()->getAspect(), 1.0 - textScale, 0.0f);
			glm::vec2 size = glm::vec2(textScale);

			//renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, glm::vec4(0.0, 0.0, 0.0, 0.2)));
			//renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y - size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, glm::vec4(0.0, 0.0, 0.0, 0.2)));
			//renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + Game::getWindow()->getAspect() * 2, pos.y - size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, glm::vec4(0.0, 0.0, 0.0, 0.2)));
			//renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + Game::getWindow()->getAspect() * 2, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, glm::vec4(0.0, 0.0, 0.0, 0.2)));
			
			const char* rendertext = " ";
			if (m_current_line.length() != 0) {
				rendertext = m_current_line.c_str();
			}
			renderer->fontRenderer->renderText(pos, size, rendertext, oe::bottomLeft, M_COLOR_TEXT_GB);
		}

		for (int i = 0; i < m_text_lines.size(); i++)
		{
			glm::vec3 pos = glm::vec3(-Game::getWindow()->getAspect(), 1.0f - (float(i + 3) * textScale), 0.0f);
			glm::vec2 size = glm::vec2(textScale);

			renderer->fontRenderer->renderText(pos, size, m_text_lines[m_text_lines.size() - 1 - i].c_str(), oe::bottomLeft, M_COLOR_TEXT_GB);
		}
	}
}

void Gui::addChatLine(std::string text) {
	oe::Logger::out(oe::info, "chat:", text.c_str());
	m_chat_opened_timer = 5.0f;
	m_text_lines.push_back(text);
	if (m_text_lines.size() > MAX_TEXT_LINES) m_text_lines.erase(m_text_lines.begin());
}

void Gui::addTextToLatest(std::string text) {
	m_last_inputs.push_back(text);
	if (m_last_inputs.size() > MAX_TEXT_LINES) m_last_inputs.erase(m_last_inputs.begin());
}

void Gui::update(float divider) {
	m_chat_opened_timer -= 1.0f / divider;
	if (Game::advancedDebugMode) {
		m_currentUpdate++;
		if (m_currentUpdate >= GUI_FRAME_LOGGER_SIZE) m_currentUpdate = 0;
		m_update_logger[m_currentUpdate] = Game::getLoop()->getUMS();

		m_avg_update = 0;
		for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++)
		{
			m_avg_update += m_update_logger[i];
		}
		m_avg_update /= (float)GUI_FRAME_LOGGER_SIZE;
	}
}

void Gui::keyPress(int key, int action) {
	if (Game::paused) {
		//Pause menu navigation
		if (action == OE_PRESS && key == OE_KEY_DOWN) { m_selectedButton++; return; }
		if (action == OE_PRESS && key == OE_KEY_UP) { m_selectedButton--; return; }
		if (action == OE_PRESS && key == OE_KEY_ENTER || key == OE_KEY_SPACE) {
			if (m_selectedButton == 0) {
				Game::paused = false;
				return;
			}
			if (m_selectedButton == 1) {
				oe::Logger::out(oe::critical, "TODO: Open settings!");
				return;
			}
			if (m_selectedButton == 2) {
				Game::getLoop()->stop();
				return;
			}
			return;
		}
	}
	else {
		if (action == OE_PRESS && key == OE_KEY_T) {
			//Only opens chat
			if (m_chat_opened) return;

			m_chat_opened = true;
			m_chat_just_opened = true;
			return;
		}
		if (action == OE_PRESS && key == OE_KEY_ESCAPE && m_chat_opened) {
			//Only closes chat if its opened
			m_chat_opened = false;
			m_chat_opened_timer = 5.0f;
			return;
		}
		if (action == OE_PRESS && key == OE_KEY_ENTER && m_chat_opened) {
			//Only closes chat if its opened
			m_chat_opened = false;
			m_chat_opened_timer = 5.0f;
			userInput();
			addTextToLatest(m_current_line);
			m_current_line = "";
			m_current_line_reserved = "";
			m_current_input_history = -1;
			return;
		}
		if (action == OE_PRESS && key == OE_KEY_BACKSPACE && m_chat_opened) {
			//Removes last character of current text input
			//if chat is currently opened
			if (m_current_line.length() > 0) m_current_line.erase(m_current_line.end() - 1);
			return;
		}
		if (action == OE_PRESS && key == OE_KEY_UP && m_chat_opened) {
			if (m_current_input_history == -1) m_current_line_reserved = m_current_line;
			m_current_input_history++;
			selectInputHistory();
			return;
		}
		if (action == OE_PRESS && key == OE_KEY_DOWN && m_chat_opened) {
			m_current_input_history--;
			selectInputHistory();
			return;
		}
	}
	if (action == OE_PRESS && key == OE_KEY_F9) { m_selected_logger = !m_selected_logger; return; }
}

void Gui::selectInputHistory() {
	if (m_last_inputs.size() == 0) return;
	m_current_input_history = oe::clamp(m_current_input_history, -1, (int)m_last_inputs.size() - 1);

	if (m_current_input_history == -1) {
		m_current_line = m_current_line_reserved;
		return;
	}
	m_current_line = m_last_inputs[(int)(m_last_inputs.size() - 1 - m_current_input_history)];
}

void Gui::typing(unsigned int codepoint, int mods) {
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

void Gui::userInput() {
	//Check if command is typed
	if (m_current_line[0] == '/') {
		int space = m_current_line.find(' ');
		std::string command = m_current_line.substr(1, (size_t)space - (size_t)1);
		std::string arguments = m_current_line.substr((size_t)space + (size_t)1);
		std::vector<std::string> argumentVec;

		//Check if has arguments
		if (space != -1) {

			std::string leftArguments = arguments;
			int nextArg = 0;
			while (nextArg != -1) {
				nextArg = leftArguments.find(' ');
				std::string thisargument = leftArguments.substr(0, nextArg);
				leftArguments = leftArguments.substr((size_t)nextArg + (size_t)1);
				argumentVec.push_back(thisargument);
				//std::cout << "\"" << thisargument << "\"" << std::endl;
			}

		}
		else {
			//No arguments
		}

		//COMMANDS
		if (command == "clear") {
			//Easter egg :DD
			tooFewArguments();
			addChatLine("Haha, just kidding ya :D");

			//Execute the command
			m_text_lines.clear();
		}
		//TP COMMAND
		else if (command == "tp") {
			if (argumentVec.size() < 2) {
				tooFewArguments();
			}

			//Use arguments
			float posX = Game::getPlayer()->getX();
			float posY = Game::getPlayer()->getY();

			//Position arguments
			if (argumentVec.size() >= 2) {

				if (argToPos(posX, posY, argumentVec[0], argumentVec[1])) {
					addChatLine("Invalid <x y> arguments!");
					return;
				}

			}

			//Execute the command
			Game::getPlayer()->setX(posX);
			Game::getPlayer()->setY(posY);

			oe::Logger::out(oe::info, "Teleported player to ", posX, ", ", posY);
			addChatLine("Teleported player to " + std::to_string(posX) + ", " + std::to_string(posY));
		}
		//RESPAWN COMMAND
		else if (command == "respawn") {
			Game::getPlayer()->die();
			addChatLine("The player went crazy and decided to commit not alive");
		}
		//SPAWN COMMAND
		else if (command == "spawn") {
			if (argumentVec.size() < 1) {
				tooFewArguments();
				return;
			}
			float id = 0;
			float n = 1;
			bool item = false;

			float posX = Game::getPlayer()->getX();
			float posY = Game::getPlayer()->getY();


			//Id argument
			if (argumentVec.size() >= 1) {

				if (argToDouble(id, argumentVec[0])) {
					addChatLine("Invalid [id] argument!");
					return;
				}

			}

			//Position arguments
			if (argumentVec.size() >= 3) {

				if (argToPos(posX, posY, argumentVec[1], argumentVec[2])) {
					addChatLine("Invalid <x y> arguments!");
					return;
				}

			}

			//Position arguments
			if (argumentVec.size() >= 4) {

				if (argumentVec[3] == "true") {
					item = true;
				}
				else if (argumentVec[3] == "false") {
					item = false;
				}
				else {
					addChatLine("Invalid <item> argument!");
					return;
				}
			}

			//Position arguments
			if (argumentVec.size() >= 5) {

				if (argToDouble(n, argumentVec[4])) {
					addChatLine("Invalid <n> argument!");
					return;
				}

			}

			//Check errors
			if (!item) {
				if (id >= Database::creatures.size()) {
					addChatLine(std::to_string(id) + " isnt a valid creature");
				}
			}
			else {
				if (id >= Database::items.size()) {
					addChatLine(std::to_string(id) + " isnt a valid item");
				}
			}

			//Execute the command
			if (item) {
				addChatLine(std::to_string((int)n) + " " + Database::items[(int)id].name + "(s) spawned at " + std::to_string((int)posX) + ", " + std::to_string((int)posY));
			}
			else {
				addChatLine(std::to_string((int)n) + " " + Database::creatures[(int)id].name + "(s) spawned " + std::to_string((int)posX) + ", " + std::to_string((int)posY));
			}
			for (int i = 0; i < (int)n; i++) Creature* tmp = Game::getMap()->addCreature(posX, posY, (int)id, item);
		}
		//SETSPAWN COMMAND
		else if (command == "setspawn") {
			float posX = Game::getPlayer()->getX();
			float posY = Game::getPlayer()->getY();

			//Position arguments
			if (argumentVec.size() >= 2) {

				if (argToPos(posX, posY, argumentVec[0], argumentVec[1])) {
					addChatLine("Invalid <x y> arguments!");
					return;
				}

			}

			//Execute the command
			Game::getPlayer()->setSpawnPoint(posX, posY);
		}
		//OBJECT COMMAND
		else if (command == "object") {
			if (argumentVec.size() < 1) {
				tooFewArguments();
				return;
			}

			float id = 0;

			float posX = Game::getPlayer()->getX();
			float posY = Game::getPlayer()->getY();


			//Id argument
			if (argumentVec.size() >= 1) {

				if (argToDouble(id, argumentVec[0])) {
					addChatLine("Invalid [id] argument!");
					return;
				}

			}

			//Position arguments
			if (argumentVec.size() >= 3) {

				if (argToPos(posX, posY, argumentVec[1], argumentVec[2])) {
					addChatLine("Invalid <x y> arguments!");
					return;
				}

			}

			//Check errors
			if (id >= Database::objects.size()) {
				addChatLine(std::to_string(id) + " isnt a valid object");
			}

			//Execute the command
			addChatLine(Database::objects[(int)id].name + " placed at " + std::to_string((int)posX) + ", " + std::to_string((int)posY));
			Game::getMap()->getTile(posX, posY)->m_object = id;
			Game::getMap()->getTile(posX, posY)->m_objectHealth = Database::objects[id].health;
		}
		//SAVE COMMAND
		else if (command == "save") {
			Game::saveWorld();
		}
		//LOAD COMMAND
		else if (command == "load") {
			std::string name = Game::getMap()->getName();
			//Id argument
			if (argumentVec.size() >= 1) {
				name = argumentVec[0].c_str();
			}

			oe::Logger::out(oe::info, name);
			Game::loadWorld(name.c_str(), false);
		}
		//UPDATEWARP COMMAND
		else if (command == "warp") {
			float updates = 0;
			//Id argument
			if (argumentVec.size() >= 1) {
				if (argToDouble(updates, argumentVec[0])) {
					addChatLine("Invalid [updates] argument!");
					return;
				}
			}

			if (updates < 0) {
				addChatLine("Cannot warp <0 updates");
				return;
			}

			updates = unsigned int(updates);

			addChatLine("Warping " + std::to_string((int)updates) + " updates");

			for (int i = 0; i < updates; i++)
			{
				Game::update();
			}
		}
		//GOD COMMAND
		else if (command == "god") {
			if (!Game::getPlayer()->getGodmode()) addChatLine("Godmode: ON");
			else addChatLine("Godmode: OFF");

			Game::getPlayer()->setGodmode(!Game::getPlayer()->getGodmode());
		}
		//NOCLIP COMMAND
		else if (command == "noclip") {
			if (!Game::getPlayer()->getClipmode()) addChatLine("Clip: NO");
			else addChatLine("Clip: YES");

			Game::getPlayer()->setClipmode(!Game::getPlayer()->getClipmode());
		}
		//HELP COMMAND
		else if (command == "help") {
			float page = 0;

			if (argumentVec.size() >= 1) {
				if (argToDouble(page, argumentVec[0])) {
					addChatLine("Invalid <0-2> argument!");
					return;
				}
				page = (int)page;
			}

			addChatLine("-- List of commands [" + std::to_string((int)page) + "] --");

			if (page == 0) {
				addChatLine("$0/$4help $6<0-2>                    $7-- list of commands");
				addChatLine("$0/$4clear                           $7-- clears chat");
				addChatLine("$0/$4tp $3[x] [y]                    $7-- teleports to [x] [y]");
				addChatLine("$0/$4respawn                         $7-- kills the player");
				addChatLine("$0/$4spawn $3[id] $6<x y> <item> <n> $7-- spawns creature");
			}
			else if (page == 1) {
				addChatLine("$0/$4setspawn $6<x y>                $7-- sets spawnpoint");
				addChatLine("$0/$4object $3[id] $6<x y>           $7-- places object");
				addChatLine("$0/$4save                            $7-- saves the world");
				addChatLine("$0/$4load $6<name>                   $7-- load world");
				addChatLine("$0/$4warp $3[updates]                $7-- update warping");
			}
			else if (page == 2) {
				addChatLine("$0/$4god                               $7-- toggle godmode");
				addChatLine("$0/$4noclip                            $7-- toggle noclip");
				//addChatLine("empty                        -- template for new commands");
				//addChatLine("empty                        -- template for new commands");
				//addChatLine("empty                        -- template for new commands");
			}
		}
		//COMMAND NOT FOUND
		else {
			addChatLine("Command \"" + m_current_line + "\", was not found");
		}
	}
	else {
		//Not a command but regular text
		addChatLine("Player: " + m_current_line);
		oe::Logger::out(oe::info, "Player", m_current_line.c_str());
	}
}

bool Gui::argToDouble(float &n, std::string string) {
	n = 0;

	try
	{
		n += stof(string);
	}
	catch (const std::exception&)
	{
		return true;
	}
	return false;
}

bool Gui::argToPos(float &x, float &y, std::string x_string, std::string y_string) {
	x = 0;
	y = 0;

	if (x_string[0] == '*') {
		x = Game::getPlayer()->getX();
		x_string.erase(x_string.begin(), x_string.begin() + 1);
	}
	if (y_string[0] == '*') {
		y = Game::getPlayer()->getY();
		y_string.erase(y_string.begin(), y_string.begin() + 1);
	}

	try
	{
		if(x_string.length() != 0) x += stof(x_string);
		if (y_string.length() != 0) y += stof(y_string);
	}
	catch (const std::exception&)
	{
		return true;
	}
	return false;
}

void Gui::tooFewArguments() {
	addChatLine("Not enough arguments");
	addChatLine("Type \"/help\" to see list of commands and their uses");
}