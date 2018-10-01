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
	if (m_object_health < 0) {
		m_object_id = 5;
	}

	glm::vec2 off = glm::vec2(m_object_id, 0);
	if (m_object_id == 9) {
		int right = Game::getTileObjectId(m_x + 1, m_y);
		int top = Game::getTileObjectId(m_x, m_y - 1);
		int left = Game::getTileObjectId(m_x - 1, m_y);
		int bottom = Game::getTileObjectId(m_x, m_y + 1);

		int topright = Game::getTileObjectId(m_x + 1, m_y - 1);
		int topleft = Game::getTileObjectId(m_x - 1, m_y - 1);
		int bottomleft = Game::getTileObjectId(m_x - 1, m_y + 1);
		int bottomright = Game::getTileObjectId(m_x + 1, m_y + 1);

		bool rightAir = right != 0 && !(right >= 9 && right <= 17);
		bool topAir = top != 0 && !(top >= 9 && top <= 17);
		bool leftAir = left != 0 && !(left >= 9 && left <= 17);
		bool bottomAir = bottom != 0 && !(bottom >= 9 && bottom <= 17);

		bool topRightAir = topright != 0 && !(topright >= 9 && topright <= 17);
		bool topLeftAir = topleft != 0 && !(topleft >= 9 && topleft <= 17);
		bool bottomLeftAir = bottomleft != 0 && !(bottomleft >= 9 && bottomleft <= 17);
		bool bottomRightAir = bottomright != 0 && !(bottomright >= 9 && bottomright <= 17);

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