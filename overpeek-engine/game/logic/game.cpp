#include "game.h"

#include "../creatures/player.h"
#include "../world/region.h"
#include "textureManager.h"

graphics::Shader *Game::m_shader;
graphics::SimpleRenderer *Game::m_renderer;
Region *Game::m_region[RENDER_DST * 2][RENDER_DST * 2];
Player *Game::m_player;

void Game::init(graphics::Shader *shader, graphics::Window * window) {
	m_shader = shader; m_window = window;

	TextureManager::loadTexture("textures/null.png", 0);
	TextureManager::loadTexture("textures/grass.png", 1);
	TextureManager::loadTexture("textures/water.png", 2);

	for (int x = -RENDER_DST; x < RENDER_DST; x++)
	{
		for (int y = -RENDER_DST; y < RENDER_DST; y++)
		{
			m_region[x][y] = new Region(x, y);
		}
	}
	m_player = new Player(0.0, 0.0);
}

void Game::render() {
	for (int x = -RENDER_DST; x < RENDER_DST; x++)
	{
		for (int y = -RENDER_DST; y < RENDER_DST; y++)
		{
			m_region[x][y]->render();
		}
	}
	m_player->render();
}

void Game::update() {
	if (m_window->getKey(GLFW_KEY_A)) {
		m_player.
	}

	m_player->update();
	for (int x = -RENDER_DST; x < RENDER_DST; x++)
	{
		for (int y = -RENDER_DST; y < RENDER_DST; y++)
		{
			//m_region[x][y]->update();
		}
	}
}