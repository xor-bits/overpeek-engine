#include "tile.h"

#include "region.h"
#include "../logic/game.h"

Tile::Tile(long int x, long int y, int id, int object_id) {
	m_id = id; m_object_id = object_id; m_x = x; m_y = y;
	m_object_health = 1.0;
}

Tile::Tile() {
	m_id = 0; m_object_id = 0; m_x = 0; m_y = 0;
	m_object_health = 0.0;
}

int Tile::getFloorTexture() {
	return idToTextureId(m_id);
}

int Tile::getObjectTexture() {
	if (m_object_health <= 0) {
		m_object_id = 0;
	}

	glm::vec2 off = glm::vec2(m_object_id, 0);
	if (m_object_id == 9) {
		Tile* right = Game::getTile(m_x + 1, m_y);
		Tile* top = Game::getTile(m_x, m_y - 1);
		Tile* left = Game::getTile(m_x - 1, m_y);
		Tile* bottom = Game::getTile(m_x, m_y + 1);
		
		Tile* topright = Game::getTile(m_x + 1, m_y - 1);
		Tile* topleft = Game::getTile(m_x - 1, m_y - 1);
		Tile* bottomleft = Game::getTile(m_x - 1, m_y + 1);
		Tile* bottomright = Game::getTile(m_x + 1, m_y + 1);

		bool rightAir = right && !(right->getObjectId() >= 9 && right->getObjectId() <= 17);
		bool topAir = top && !(top->getObjectId() >= 9 && top->getObjectId() <= 17);
		bool leftAir = left && !(left->getObjectId() >= 9 && left->getObjectId() <= 17);
		bool bottomAir = bottom && !(bottom->getObjectId() >= 9 && bottom->getObjectId() <= 17);

		bool topRightAir = topright && !(topright->getObjectId() >= 9 && topright->getObjectId() <= 17);
		bool topLeftAir = topleft && !(topleft->getObjectId() >= 9 && topleft->getObjectId() <= 17);
		bool bottomLeftAir = bottomleft && !(bottomleft->getObjectId() >= 9 && bottomleft->getObjectId() <= 17);
		bool bottomRightAir = bottomright && !(bottomright->getObjectId() >= 9 && bottomright->getObjectId() <= 17);

		bool allSidesAir = rightAir && topAir && leftAir && bottomAir;
		bool allCornersAir = topRightAir && topLeftAir && bottomLeftAir && bottomRightAir;
		bool noSideAir = !rightAir && !topAir && !leftAir && !bottomAir;
		bool noCornerAir = !topRightAir && !topLeftAir && !bottomLeftAir && !bottomRightAir;

		if (!rightAir && topAir && leftAir && !bottomAir && !bottomRightAir) {
			off = glm::vec2(0, 1);
		}
		else if (!rightAir && topAir && !leftAir && !bottomAir && !bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(1, 1);
		}
		else if (rightAir && topAir && !leftAir && !bottomAir && !bottomLeftAir) {
			off = glm::vec2(2, 1);
		}
		else if (!rightAir && !topAir && leftAir && !bottomAir && !topRightAir && !bottomRightAir) {
			off = glm::vec2(0, 2);
		}
		else if (!rightAir && !topAir && !leftAir && !bottomAir && !topRightAir && !topLeftAir && !bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(1, 2);
		}
		else if (rightAir && !topAir && !leftAir && !bottomAir && !topLeftAir && !bottomLeftAir) {
			off = glm::vec2(2, 2);
		}
		else if (!rightAir && !topAir && leftAir && bottomAir && !topRightAir) {
			off = glm::vec2(0, 3);
		}
		else if (!rightAir && !topAir && !leftAir && bottomAir && !topRightAir && !topLeftAir) {
			off = glm::vec2(1, 3);
		}
		else if (rightAir && !topAir && !leftAir && bottomAir && !topLeftAir) {
			off = glm::vec2(2, 3);
		}

		else if (noSideAir && !topRightAir && !topLeftAir && !bottomLeftAir && bottomRightAir) {
			off = glm::vec2(0, 4);
		}
		else if (noSideAir && !topRightAir && !topLeftAir && bottomLeftAir && bottomRightAir) {
			off = glm::vec2(1, 4);
		}
		else if (noSideAir && !topRightAir && !topLeftAir && bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(2, 4);
		}
		else if (noSideAir && topRightAir && !topLeftAir && !bottomLeftAir && bottomRightAir) {
			off = glm::vec2(0, 5);
		}
		else if (noSideAir && topRightAir && topLeftAir && bottomLeftAir && bottomRightAir) {
			off = glm::vec2(1, 5);
		}
		else if (noSideAir && !topRightAir && topLeftAir && bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(2, 5);
		}
		else if (noSideAir && topRightAir && !topLeftAir && !bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(0, 6);
		}
		else if (noSideAir && topRightAir && topLeftAir && !bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(1, 6);
		}
		else if (noSideAir && !topRightAir && topLeftAir && !bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(2, 6);
		}

		else if (rightAir && !topAir && leftAir && !bottomAir) {
			off = glm::vec2(0, 7);
		}
		else if (!rightAir && topAir && !leftAir && bottomAir) {
			off = glm::vec2(1, 7);
		}
		else if (rightAir && topAir && leftAir && !bottomAir) {
			off = glm::vec2(2, 7);
		}
		else if (!rightAir && topAir && leftAir && bottomAir) {
			off = glm::vec2(0, 8);
		}
		else if (rightAir && topAir && !leftAir && bottomAir) {
			off = glm::vec2(1, 8);
		}
		else if (rightAir && !topAir && leftAir && bottomAir) {
			off = glm::vec2(2, 8);
		}

		else if (!rightAir && topAir && leftAir && !bottomAir && bottomRightAir) {
			off = glm::vec2(0, 9);
		}
		else if (!rightAir && topAir && !leftAir && !bottomAir && bottomRightAir && bottomLeftAir) {
			off = glm::vec2(1, 9);
		}
		else if (rightAir && topAir && !leftAir && !bottomAir && bottomLeftAir) {
			off = glm::vec2(2, 9);
		}
		else if (!rightAir && !topAir && leftAir && !bottomAir && topRightAir && bottomRightAir) {
			off = glm::vec2(0, 10);
		}
		else if (!rightAir && !topAir && !leftAir && !bottomAir && topRightAir && topLeftAir && bottomRightAir && bottomLeftAir) {
			off = glm::vec2(1, 10);
		}
		else if (rightAir && !topAir && !leftAir && !bottomAir && topLeftAir && bottomLeftAir) {
			off = glm::vec2(2, 10);
		}
		else if (!rightAir && !topAir && leftAir && bottomAir && topRightAir) {
			off = glm::vec2(0, 11);
		}
		else if (!rightAir && !topAir && !leftAir && bottomAir && topRightAir && topLeftAir) {
			off = glm::vec2(1, 11);
		}
		else if (rightAir && !topAir && !leftAir && bottomAir && topLeftAir) {
			off = glm::vec2(2, 11);
		}

		else if (noSideAir && !topRightAir && topLeftAir && bottomLeftAir && bottomRightAir) {
			off = glm::vec2(0, 12);
		}
		else if (noSideAir && topRightAir && !topLeftAir && bottomLeftAir && bottomRightAir) {
			off = glm::vec2(1, 12);
		}
		else if (noSideAir && topRightAir && topLeftAir && bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(0, 13);
		}
		else if (noSideAir && topRightAir && topLeftAir && !bottomLeftAir && bottomRightAir) {
			off = glm::vec2(1, 13);
		}

		else if (!rightAir && !topAir && leftAir && !bottomAir && !topRightAir && bottomRightAir) {
			off = glm::vec2(0, 14);
		}
		else if (rightAir && !topAir && !leftAir && !bottomAir && !topLeftAir && bottomLeftAir) {
			off = glm::vec2(1, 14);
		}
		else if (!rightAir && !topAir && leftAir && !bottomAir && topRightAir && !bottomRightAir) {
			off = glm::vec2(0, 15);
		}
		else if (rightAir && !topAir && !leftAir && !bottomAir && topLeftAir && !bottomLeftAir) {
			off = glm::vec2(1, 15);
		}

		else if (!rightAir && topAir && !leftAir && !bottomAir && !bottomLeftAir && bottomRightAir) {
			off = glm::vec2(2, 12);
		}
		else if (!rightAir && topAir && !leftAir && !bottomAir && bottomLeftAir && !bottomRightAir) {
			off = glm::vec2(2, 13);
		}
		else if (!rightAir && !topAir && !leftAir && bottomAir && !topLeftAir && topRightAir) {
			off = glm::vec2(2, 14);
		}
		else if (!rightAir && !topAir && !leftAir && bottomAir && topLeftAir && !topRightAir) {
			off = glm::vec2(2, 15);
		}

		else if (noSideAir && bottomLeftAir && topRightAir) {
			off = glm::vec2(3, 12);
		}
		else if (noSideAir && bottomRightAir && topLeftAir) {
			off = glm::vec2(3, 13);
		}

		else {
			off = glm::vec2(3, 14);
		}
		return off.x + (off.y * 16);
	}
	return m_object_id;
}

void Tile::update() {}

int Tile::idToTextureId(int id) {
	switch (id)
	{
	case 1: //Lawn floor
		return 1 + (16 * 0);
		break;
	case 2: //Water floor
		return 2 + (16 * 0);
		break;
	case 3: //Sand floor
		return 3 + (16 * 0);
		break;
	case 4: //Stone floor
		return 9 + (16 * 0);
		break;
	case 5: //Air
		return 5 + (16 * 0);
		break;
	case 6: //Grass
		return 6 + (16 * 0);
		break;
	case 7: //Tree 1
		return 7 + (16 * 0);
		break;
	case 8: //Tree 2
		return 7 + (16 * 1);
		break;
	case 9: //Stone top left corner
		return 0 + (16 * 1);
		break;
	case 10: //Stone top
		return 1 + (16 * 1);
		break;
	case 11: //Stone top right corner
		return 2 + (16 * 1);
		break;
	case 12: //Stone middle left
		return 0 + (16 * 2);
		break;
	case 13: //Stone middle
		return 1 + (16 * 2);
		break;
	case 14: //Stone middle right
		return 2 + (16 * 2);
		break;
	case 15: //Stone bottom left corner
		return 0 + (16 * 3);
		break;
	case 16: //Stone bottom
		return 1 + (16 * 3);
		break;
	case 17: //Stone bottom right corner
		return 2 + (16 * 3);
		break;
	case 18: //Stone floor
		return 3 + (16 * 1);
		break;
	default:
		return 0;
		break;
	}
}