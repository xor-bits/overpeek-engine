#include "creature.h"

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

	m_vbo = new graphics::Buffer(xy, 12, 2, GL_STATIC_DRAW);
	m_tbo = new graphics::Buffer(uv, 12, 2, GL_STATIC_DRAW);
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

void Creature::render() {
	glm::mat4 ml_matrix = glm::mat4(1.0f);
	m_shader->enable();
	m_shader->SetUniformMat4("ml_matrix", ml_matrix);
	m_shader->setUniform1i("overwrite_id", m_texture);

	m_vao->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//graphics::SimpleRenderer::renderBox(TILE_SIZE / -2.0, TILE_SIZE / -2.0, TILE_SIZE, TILE_SIZE, graphics::TextureLoader::getTexture(1));
}

void Creature::update() {
	m_x += m_vel_x;
	m_y += m_vel_y;
	m_vel_x += m_acc_x;
	m_vel_y += m_acc_y;
	m_vel_x *= 0.8;
	m_vel_y *= 0.8;
	m_acc_x = 0;
	m_acc_y = 0;
}