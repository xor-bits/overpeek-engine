#include "region.h"

#include "../../engine.h"
#include "../logic/game.h"
#include "../../engine.h"

graphics::VertexArray *Region::m_vao;
graphics::Buffer *Region::m_vbo;
graphics::Buffer *Region::m_tbo;
graphics::Shader *Region::m_shader;
graphics::Buffer *Region::m_tid;

void Region::initRender(graphics::Shader *shader) {
	m_shader = shader;
	m_vao = new graphics::VertexArray();

	//Vertex buffer object
	const int xyCount = REGION_SIZE * REGION_SIZE * 6 * 2 * 2;
	const int uvCount = REGION_SIZE * REGION_SIZE * 6 * 2 * 2;
	GLfloat xy[xyCount];
	GLfloat uv[uvCount];

	int tmp = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int x = 0; x < REGION_SIZE; x++)
		{
			for (int y = 0; y < REGION_SIZE; y++)
			{
				xy[tmp++] = (x * TILE_SIZE);
				xy[tmp++] = (y * TILE_SIZE);

				xy[tmp++] = (x * TILE_SIZE) + TILE_SIZE;
				xy[tmp++] = (y * TILE_SIZE);

				xy[tmp++] = (x * TILE_SIZE) + TILE_SIZE;
				xy[tmp++] = (y * TILE_SIZE) + TILE_SIZE;


				xy[tmp++] = (x * TILE_SIZE);
				xy[tmp++] = (y * TILE_SIZE);

				xy[tmp++] = (x * TILE_SIZE) + TILE_SIZE;
				xy[tmp++] = (y * TILE_SIZE) + TILE_SIZE;

				xy[tmp++] = (x * TILE_SIZE);
				xy[tmp++] = (y * TILE_SIZE) + TILE_SIZE;
			}
		}
	}

	tmp = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int x = 0; x < REGION_SIZE; x++)
		{
			for (int y = 0; y < REGION_SIZE; y++)
			{
				uv[tmp++] = 0.0;
				uv[tmp++] = 0.0;

				uv[tmp++] = 1.0;
				uv[tmp++] = 0.0;

				uv[tmp++] = 1.0;
				uv[tmp++] = 1.0;


				uv[tmp++] = 0.0;
				uv[tmp++] = 0.0;

				uv[tmp++] = 1.0;
				uv[tmp++] = 1.0;

				uv[tmp++] = 0.0;
				uv[tmp++] = 1.0;
			}
		}
	}

	//Texture buffer object
	m_vbo = new graphics::Buffer(xy, xyCount, 2, GL_STATIC_DRAW);
	m_tbo = new graphics::Buffer(uv, uvCount, 2, GL_STATIC_DRAW);
	m_tid = new graphics::Buffer(0, REGION_SIZE * REGION_SIZE * 6 * 2 * 2, 1, GL_STATIC_DRAW);
	m_vao->addBuffer(m_vbo, 0);
	m_vao->addBuffer(m_tbo, 1);
	m_vao->addBuffer(m_tid, 2);
}

Region::Region(int x, int y) {
	m_x = x; m_y = y;
	if (tools::BinaryIO::read(getSaveLocation()) != nullptr) loadTiles();
	else createTiles();
}

Region::~Region() {
	saveTiles();
}

Region::~Region() {
	for (Tile *i : *m_tiles) {
		delete i;
	}
}

void Region::createTiles() {
	int tmp = 0;
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			float height = logic::Noise::octaveNoise((tilex + INT_MAX / 2) * NOISE_SCALE, (tiley + INT_MAX / 2) * NOISE_SCALE, 3);
			int id = 1;
			int object_id = 5;

			if (height <= LEVEL_WATER) id = 2;
			else if (height <= LEVEL_SAND) id = 3;
			else if (height <= LEVEL_GRASS) id = 1;
			else {
				float d = tools::Random::random(-0.5, 1.5);
				if (d > 1.0) object_id = 6;
				else if (d > 0.5) object_id = 7;
				else if (d > 0.0) object_id = 7 + 16;
			}

			m_tiles[x][y] = new Tile(tilex, tiley, id, object_id);
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = id;
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = id;
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = id;
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = id;
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = id;
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = id;
		}
	}
}

<<<<<<< HEAD
void Region::render() {
	m_shader->enable();
	m_shader->SetUniformMat4("ml_matrix", m_ml_matrix);
	m_shader->setUniform1i("overwrite_id", 0);

	m_vao->bind();
	m_tid->setBufferData(m_texture_id_array, REGION_SIZE * REGION_SIZE * 6 * 2, 1);
	glDrawArrays(GL_TRIANGLES, 0, REGION_SIZE * REGION_SIZE * 6 * 2);
=======
void Region::loadTiles() {
	unsigned char *readData = tools::BinaryIO::read(getSaveLocation());
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			int id = readData[x + (y * REGION_SIZE)];
			int objid = readData[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE];
			m_tiles[x][y] = new Tile(tilex, tiley, id, objid);
		}
	}
}

void Region::saveTiles() {
	unsigned char data[REGION_SIZE*REGION_SIZE*2];
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			data[x + (y * REGION_SIZE)] = m_tiles[x][y]->getId();
			data[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE] = m_tiles[x][y]->getObjectId();
		}
	}
	tools::BinaryIO::write(getSaveLocation(), data, sizeof(data) / sizeof(unsigned char));
}

void Region::render(float offx, float offy) {
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			m_tiles[x][y]->render(offx, offy);
		}
	}
>>>>>>> 58354a52ddbe18f20adb822ed668e871de978bb7
}

void Region::update(float offx, float offy) {
	float rx = (m_x - RENDER_DST / 2.0) * (float)TILE_SIZE * REGION_SIZE + offx;
	float ry = (m_y - RENDER_DST / 2.0) * (float)TILE_SIZE * REGION_SIZE + offy;
	m_ml_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(rx, ry, 0.0));

	int tmp = 0;
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = m_tiles[x][y]->getId();
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = m_tiles[x][y]->getId();
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = m_tiles[x][y]->getId();
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = m_tiles[x][y]->getId();
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = m_tiles[x][y]->getId();
			m_texture_id_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = m_tiles[x][y]->getObjectId();
			m_texture_id_array[tmp++] = m_tiles[x][y]->getId();
		}
	}
}