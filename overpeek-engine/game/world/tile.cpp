#include "tile.h"

#include "region.h"
#include "../../engine.h"
#include "../logic/game.h"
#include "../logic/textureManager.h"

Tile::Tile(long int x, long int y, int id, int object_id) {
	m_id = id; m_object_id = object_id; m_x = x; m_y = y;
	m_object_health = 1.0;
	m_texture = TextureManager::getTexture(m_id);
}

Tile::~Tile() {}

void Tile::render(float offx, float offy) {
	if (m_x*TILE_SIZE + offx >= Game::getWindow()->getAspect()) return;
	if (m_x*TILE_SIZE + offx + TILE_SIZE <= -Game::getWindow()->getAspect()) return;
	if (m_y*TILE_SIZE + offy >= 1.0) return;
	if (m_y*TILE_SIZE + offy + TILE_SIZE <= -1.0) return;
	graphics::SimpleRenderer::renderBox(m_x*TILE_SIZE + offx, m_y*TILE_SIZE + offy, TILE_SIZE, TILE_SIZE, TextureManager::getTexture(m_id));
	if (m_object_id != 0) graphics::SimpleRenderer::renderBox(m_x*TILE_SIZE + offx, m_y*TILE_SIZE + offy, TILE_SIZE, TILE_SIZE, TextureManager::getTexture(m_object_id));
}

void Tile::update() {
}