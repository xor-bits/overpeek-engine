#include "creature.h"
#include "../world/tile.h"
#include "../../logic/aabb.h"

graphics::VertexArray *Creature::m_vao;
graphics::Buffer *Creature::m_vbo;
graphics::Buffer *Creature::m_tbo;
graphics::Shader *Creature::m_shader;
bool Creature::renderInited = false;

void Creature::initRender() {
	renderInited = true;
	m_vao = new graphics::VertexArray();

	GLfloat xy[] = {
		-TILE_SIZE / 2.0, -TILE_SIZE / 2.0,
		 TILE_SIZE / 2.0, -TILE_SIZE / 2.0,
		 TILE_SIZE / 2.0,  TILE_SIZE / 2.0,

		-TILE_SIZE / 2.0, -TILE_SIZE / 2.0,
		 TILE_SIZE / 2.0,  TILE_SIZE / 2.0,
		-TILE_SIZE / 2.0,  TILE_SIZE / 2.0
	};
	GLfloat uv[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 0.0,
		1.0, 1.0,
		0.0, 1.0
	};

	m_vbo = new graphics::Buffer(xy, 12, 2, sizeof(GLfloat), GL_STATIC_DRAW);
	m_tbo = new graphics::Buffer(uv, 12, 2, sizeof(GLfloat), GL_STATIC_DRAW);
	m_vao->addBuffer(m_vbo, 0);
	m_vao->addBuffer(m_tbo, 1);
}

Creature::Creature(float x, float y, graphics::Shader *shader) {
	m_x = x; m_y = y;
	m_vel_x = 0;
	m_vel_y = 0;
	m_acc_x = 0;
	m_acc_y = 0;
	m_shader = shader;

	if (!renderInited) initRender();
}

void Creature::render(float renderOffsetX, float renderOffsetY) {
	glm::mat4 ml_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_x + renderOffsetX, m_y + renderOffsetY, 0.0));
	m_shader->enable();
	m_shader->SetUniformMat4("ml_matrix", ml_matrix);
	//m_shader->setUniform2i("overwrite_off", glm::vec2(m_texture, 0.0));
	m_shader->setUniform1i("overwrite_off", m_texture);

	m_vao->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//graphics::SimpleRenderer::renderBox(TILE_SIZE / -2.0, TILE_SIZE / -2.0, TILE_SIZE, TILE_SIZE, graphics::TextureLoader::getTexture(1));
}

void Creature::update() {
	m_x += m_vel_x * 1;
	m_y += m_vel_y * 1;
	m_vel_x += m_acc_x;
	m_vel_y += m_acc_y;
	m_vel_x *= 0.8;
	m_vel_y *= 0.8;
	m_acc_x = 0;
	m_acc_y = 0;
}

void Creature::collide() {
	std::cout << m_x << ", " << m_y << std::endl;
	for (int x = -2; x < 3; x++)
	{
		for (int y = -2; y < 3; y++)
		{
			Tile* tile = Game::getTile(floor(m_x) + x, floor(m_y) + y);
			if (!tile) continue;

			if (tile->getObjectId() == 9) {
				//LEFT COLLIDER
				if (logic::AABB(
					glm::vec2(m_x - CREAURE_WIDTH / 2.0, (m_y - CREAURE_HEIGHT / 2.0) + 0.3),
					glm::vec2(CREAURE_WIDTH / 2.0, CREAURE_HEIGHT - 0.6),
					glm::vec2(tile->getX(), tile->getY()),
					glm::vec2(1, 1)
				)) {
					m_x = tile->getX() + 1 + (CREAURE_WIDTH / 2.0);
				}
				//RIGHT COLLIDER
				if (logic::AABB(
					glm::vec2(m_x, (m_y - CREAURE_HEIGHT / 2.0) + 0.3),
					glm::vec2(CREAURE_WIDTH / 2.0, CREAURE_HEIGHT - 0.6),
					glm::vec2(tile->getX(), tile->getY()),
					glm::vec2(1, 1)
				)) {
					m_x = tile->getX() - (CREAURE_WIDTH / 2.0);
				}
				//TOP COLLIDER
				if (logic::AABB(
					glm::vec2((m_x - CREAURE_WIDTH / 2.0) + 0.3, m_y - CREAURE_HEIGHT / 2.0),
					glm::vec2(CREAURE_WIDTH - 0.6, CREAURE_HEIGHT / 2.0),
					glm::vec2(tile->getX(), tile->getY()),
					glm::vec2(1, 1)
				)) {
					m_y = tile->getY() + 1 + (CREAURE_HEIGHT / 2.0);
				}
				//BOTTOM COLLIDER
				if (logic::AABB(
					glm::vec2((m_x - CREAURE_WIDTH / 2.0) + 0.3, m_y),
					glm::vec2(CREAURE_WIDTH - 0.6, CREAURE_HEIGHT / 2.0),
					glm::vec2(tile->getX(), tile->getY()),
					glm::vec2(1, 1)
				)) {
					m_y = tile->getY() - (CREAURE_HEIGHT / 2.0);
				}
			}
		}
	}
}