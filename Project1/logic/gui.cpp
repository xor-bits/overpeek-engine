#include "../pch.h"

#include "gui.h"
#include "game.h"
#include "../world/map.h"
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
	//Health bar
	glm::vec2 pos = glm::vec2(Game::getWindow()->getAspect() - 0.6 * Game::renderScale(), -1.0 + 0.1 * Game::renderScale());
	glm::vec2 size = glm::vec2(0.5, 0.05) * Game::renderScale();
	renderer->renderPoint(pos, size, 20, M_COLOR_BLACK);
	pos = glm::vec2(Game::getWindow()->getAspect() - 0.595 * Game::renderScale(), -1.0 + 0.105 * Game::renderScale());
	size = glm::vec2(0.49 * (Game::getPlayer()->getHealth() / Game::getPlayer()->getMaxHealth()), 0.04) * Game::renderScale();
	renderer->renderPoint(pos, size, 20, M_COLOR_HEALTH_BAR);

	//Stealth bar
	pos = glm::vec2(Game::getWindow()->getAspect() - 0.6 * Game::renderScale(), -1.0 + 0.16 * Game::renderScale());
	size = glm::vec2(0.5, 0.05) * Game::renderScale();
	renderer->renderPoint(pos, size, 20, M_COLOR_BLACK);
	pos = glm::vec2(Game::getWindow()->getAspect() - 0.595 * Game::renderScale(), -1.0 + 0.165 * Game::renderScale());
	size = glm::vec2(0.49 * (Game::getPlayer()->getStamina() / Game::getPlayer()->getMaxStamina()), 0.04) * Game::renderScale();
	renderer->renderPoint(pos, size, 20, M_COLOR_STAMINA_BAR);

	float textScale = 0.08 * Game::renderScale();
	float x = -Game::getWindow()->getAspect();
	if (Game::debugMode) {
		//Debug mode text
		std::string text = "UPS: " + std::to_string(Game::getLoop()->getUPS());
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 1)), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft);
	}
	if (Game::advancedDebugMode) {
		//Advanced debug mode text
		std::string text = "Position X: " + std::to_string(Game::getPlayer()->getX()) + ", Y: " + std::to_string(Game::getPlayer()->getY());
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 2)), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft);
		text = "Renderer: " + Game::getWindow()->getRenderer();
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 3)), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft);
		text = "Vendor: " + Game::getWindow()->getVendor();
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 4)), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft);

		m_currentFrame++;
		if (m_currentFrame >= GUI_FRAME_LOGGER_SIZE) m_currentFrame = 0;
		m_frame_logger[m_currentFrame] = Game::getLoop()->getFMS();
		if (m_frame_logger[m_currentFrame] > m_slowest_frame) m_slowest_frame = m_frame_logger[m_currentFrame];

		glm::vec2 pos = glm::vec2(-Game::getWindow()->getAspect(), 2.0 / 3.0);
		glm::vec2 size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * (GUI_FRAME_LOGGER_SIZE - 1), 0.005) * Game::renderScale();
		glm::vec2 textPos = glm::vec2(-Game::getWindow()->getAspect(), 2.0 / 3.0 - 0.02 * Game::renderScale());
		glm::vec2 textSize = glm::vec2(textScale * 0.8);
		renderer->renderPoint(pos, size, 20, glm::vec4(1.0, 0.0, 0.0, 1.0));
		if (m_selected_logger) {
			//Top bar
			text = "frame: " + std::to_string((int)m_slowest_frame);
			renderer->renderText(textPos, textSize, text.c_str(), oe::bottomLeft);

			//Actual logger
			for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
				float bar_height = (m_frame_logger[i] / m_slowest_frame) / 3.0;
				pos = glm::vec2(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * Game::renderScale() * i), 1.0 - bar_height);
				size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * Game::renderScale(), bar_height);
				renderer->renderPoint(pos, size, 20, M_COLOR_BLACK);
			}
		}
		else {
			text = "update: " + std::to_string((int)m_slowest_update);
			renderer->renderText(textPos, textSize, text.c_str(), oe::bottomLeft);

			for (int i = 0; i < GUI_FRAME_LOGGER_SIZE; i++) {
				float bar_height = (m_update_logger[i] / m_slowest_update) / 3.0;
				pos = glm::vec2(-Game::getWindow()->getAspect() + (GUI_FRAME_LOGGER_BAR_WIDTH * Game::renderScale() * i), 1.0 - bar_height);
				size = glm::vec2(GUI_FRAME_LOGGER_BAR_WIDTH * Game::renderScale(), bar_height);
				renderer->renderPoint(pos, size, 20, M_COLOR_BLACK);
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
		renderer->renderText(glm::vec2(x, -1.0 + (textScale * 0)), glm::vec2(textScale, textScale), text.c_str(), oe::topLeft);
	}

	if (Game::paused) {

		if (m_selectedButton > BUTTONS - 1) m_selectedButton = 0;
		if (m_selectedButton < 0) m_selectedButton = BUTTONS - 1;

		float textScale = 0.1 * Game::renderScale();

		std::string text = "PAUSED";
		renderer->renderText(glm::vec2(0.003, -0.003 - 0.75), glm::vec2(textScale, textScale), text.c_str(), oe::center);

		for (int i = 0; i < 3; i++)
		{
			float shade = 0.1f;
			if (i == m_selectedButton) shade = 0.2f;
			renderer->renderPoint(glm::vec2(0.0 - 0.4, -0.50 - 0.05 + (i * 0.15)), glm::vec2(0.8, 0.1), 20, glm::vec4(0.0, 0.0, 0.0, shade));
		}

		textScale = 0.05;
		text = "RESUME";
		renderer->renderText(glm::vec2(0.0, -0.5), glm::vec2(textScale, textScale), text.c_str(), oe::center);

		textScale = 0.05;
		text = "SETTINGS";
		renderer->renderText(glm::vec2(0.0, -0.35), glm::vec2(textScale, textScale), text.c_str(), oe::center);

		textScale = 0.05;
		text = "SAVE AND EXIT";
		renderer->renderText(glm::vec2(0.0, -0.20), glm::vec2(textScale, textScale), text.c_str(), oe::center);
	}

	if (m_chat_opened || m_chat_opened_timer > 0) {
		if (m_chat_opened) {
			renderer->renderPoint(glm::vec2(-Game::getWindow()->getAspect(), 1.0 - textScale - 0.05 * Game::renderScale()), glm::vec2(Game::getWindow()->getAspect() * 2, textScale + 0.05 * Game::renderScale()), 20, glm::vec4(0.0, 0.0, 0.0, 0.2));
			renderer->renderText(glm::vec2(-Game::getWindow()->getAspect(), 1.0 - 0.05 * Game::renderScale()), glm::vec2(textScale, textScale), m_current_line.c_str(), oe::bottomLeft);
		}

		for (int i = 0; i < m_text_lines.size(); i++)
		{
			renderer->renderText(glm::vec2(-Game::getWindow()->getAspect(), 1.0 - ((i + 1.5) * textScale * 1.2)), glm::vec2(textScale, textScale), m_text_lines[m_text_lines.size() - 1 - i].c_str(), oe::bottomLeft);
		}
	}
}

void Gui::addChatLine(std::string text) {
	oe::Logger::out(oe::info, "chat:", text.c_str());
	m_chat_opened_timer = 500;
	m_text_lines.push_back(text);
	if (m_text_lines.size() > MAX_TEXT_LINES) m_text_lines.erase(m_text_lines.begin());
}

void Gui::addTextToLatest(std::string text) {
	m_last_inputs.push_back(text);
	if (m_last_inputs.size() > MAX_TEXT_LINES) m_last_inputs.erase(m_last_inputs.begin());
}

void Gui::update() {
	m_chat_opened_timer--;
	if (Game::advancedDebugMode) {
		m_currentUpdate++;
		if (m_currentUpdate >= GUI_FRAME_LOGGER_SIZE) m_currentUpdate = 0;
		m_update_logger[m_currentUpdate] = Game::getLoop()->getUMS();
		if (m_update_logger[m_currentUpdate] > m_slowest_update) m_slowest_update = m_update_logger[m_currentUpdate];
	}
}

void Gui::keyPress(int key, int action) {
	if (Game::paused) {
		//Pause menu navigation
		if (key == OE_KEY_DOWN) { m_selectedButton++; return; }
		if (key == OE_KEY_UP) { m_selectedButton--; return; }
		if (key == OE_KEY_ENTER || key == OE_KEY_SPACE) {
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
		if (key == OE_KEY_T) {
			//Only opens chat
			if (m_chat_opened) return;

			m_chat_opened = true;
			m_chat_just_opened = true;
			return;
		}
		if (key == OE_KEY_ESCAPE && m_chat_opened) {
			//Only closes chat if its opened
			m_chat_opened = false;
			m_chat_opened_timer = 500;
			return;
		}
		if (key == OE_KEY_ENTER && m_chat_opened) {
			//Only closes chat if its opened
			m_chat_opened = false;
			m_chat_opened_timer = 500;
			userInput();
			addTextToLatest(m_current_line);
			m_current_line = "";
			m_current_line_reserved = "";
			m_current_input_history = -1;
			return;
		}
		if (key == OE_KEY_BACKSPACE && m_chat_opened) {
			//Removes last character of current text input
			//if chat is currently opened
			if (m_current_line.length() > 0) m_current_line.erase(m_current_line.end() - 1);
			return;
		}
		if (key == OE_KEY_UP && m_chat_opened) {
			if (m_current_input_history == -1) m_current_line_reserved = m_current_line;
			m_current_input_history++;
			selectInputHistory();
		}
		if (key == OE_KEY_DOWN && m_chat_opened) {
			m_current_input_history--;
			selectInputHistory();
		}
	}
	if (key == OE_KEY_F9) m_selected_logger = !m_selected_logger;
}

void Gui::selectInputHistory() {
	if (m_last_inputs.size() == 0) return;
	m_current_input_history = oe::clamp(m_current_input_history, -1, (int)m_last_inputs.size() - 1);

	if (m_current_input_history == -1) {
		m_current_line = m_current_line_reserved;
		return;
	}
	m_current_line = m_last_inputs[(int)m_last_inputs.size() - 1 - m_current_input_history];
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
		std::string command = m_current_line.substr(1, space - 1);
		std::string arguments = m_current_line.substr(space + 1);
		std::vector<std::string> argumentVec;

		//Check if has arguments
		if (space != -1) {

			std::string leftArguments = arguments;
			int nextArg = 0;
			while (nextArg != -1) {
				nextArg = leftArguments.find(' ');
				std::string thisargument = leftArguments.substr(0, nextArg);
				leftArguments = leftArguments.substr(nextArg + 1);
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

			float id;
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
			for (int i = 0; i < (int)n; i++) Creature *tmp = Game::getMap()->addCreature(posX, posY, (int)id, item);
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

			float id;

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
		//HELP COMMAND
		else if (command == "help") {
			addChatLine("--List of commands--");
			addChatLine("/clear -- clears chat");
			addChatLine("/tp [x] [y] -- teleports to [x] [y]");
			addChatLine("/respawn -- kills the player");
			addChatLine("/spawn [id] <x y> <item> <n> -- spawns creature");
			addChatLine("/setspawn <x y> -- sets spawnpoint");
			addChatLine("/object [id] <x y> -- places object");
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
	addChatLine("Too few arguments");
	addChatLine("Type \"/help\" to see list of commands and their uses");
}