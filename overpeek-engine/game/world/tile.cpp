#include "tile.h"

#include "region.h"
#include "../../engine.h"
#include "../logic/game.h"
#include "../logic/textureManager.h"

Tile::Tile(long int x, long int y, int id) {
	m_id = id; m_x = x; m_y = y;
}

void Tile::render() {
	graphics::SimpleRenderer::renderBox(m_x*TILE_SIZE, m_y*TILE_SIZE, TILE_SIZE, TILE_SIZE, TextureManager::getTexture(m_id));
}

void Tile::update() {

}