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
	m_vbo = new graphics::Buffer(xy, xyCount, 2, sizeof(GLfloat), GL_STATIC_DRAW);
	m_tbo = new graphics::Buffer(uv, uvCount, 2, sizeof(GLfloat), GL_STATIC_DRAW);
	m_tid = new graphics::Buffer(0, REGION_SIZE * REGION_SIZE * 6 * 2 * 4, 4, sizeof(GLfloat), GL_DYNAMIC_DRAW);
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

void Region::createTiles() {
	int tmp = 0;
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			float height = (logic::Noise::octaveNoise((tilex + INT_MAX / 2) * NOISE_SCALE, (tiley + INT_MAX / 2) * NOISE_SCALE, 3) + 1.0) / 2.0;
			int id = 1;
			int object_id = 5;

			//height is from 0 to 1
			if (height <= LEVEL_WATER) id = 2;
			else if (height <= LEVEL_SAND) id = 3;
			else if (height <= LEVEL_GRASS) {
				id = 1;
				float d = tools::Random::random(-0.5, 1.5);
				if (d > 1.0) object_id = 6;
				else if (d > 0.5) object_id = 7;
				else if (d > 0.0) object_id = 8;
			}
			else if (height <= LEVEL_STONE) {
				id = 4;
			}
			else {
				id = 4;
				object_id = 9;
			}

			m_tiles[x][y] = Tile(tilex, tiley, id, object_id);

			glm::vec4 arrayid = glm::vec4(1, 1, id % 16, (id - (id % 16)) / 16);
			glm::vec4 arrayobjectid = glm::vec4(1, 1, m_tiles[x][y].getObjectTexture() % 16, (m_tiles[x][y].getObjectTexture() - (m_tiles[x][y].getObjectTexture() % 16)) / 16);

			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
		}
	}
	//for (int i = 0; i < REGION_SIZE * REGION_SIZE * 6 * 2; i++)
	//{
	//	std::cout << i << ", " << m_texture_off_array[i] << std::endl;
	//}
}

void Region::render() {
	m_shader->enable();
	m_shader->SetUniformMat4("ml_matrix", m_ml_matrix);
	m_shader->setUniform4i("overwrite_off", glm::ivec4(0));

	m_vao->bind();
	m_tid->setBufferData(m_texture_off_array, REGION_SIZE * REGION_SIZE * 6 * 2 * 4, 4, sizeof(GLfloat));
	glDrawArrays(GL_TRIANGLES, 0, REGION_SIZE * REGION_SIZE * 6 * 2);
}

void Region::loadTiles() {
	int tmp = 0;
	unsigned char *readData = tools::BinaryIO::read(getSaveLocation());
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			int id = readData[x + (y * REGION_SIZE)];
			int objid = readData[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE];
			m_tiles[x][y] = Tile(tilex, tiley, id, objid);

			glm::vec4 arrayid = glm::vec4(1, 1, id % 16, (id - (id % 16)) / 16);
			glm::vec4 arrayobjectid = glm::vec4(1, 1, m_tiles[x][y].getObjectTexture() % 16, (m_tiles[x][y].getObjectTexture() - (m_tiles[x][y].getObjectTexture() % 16)) / 16);

			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
		}
	}
}

void Region::saveTiles() {
	unsigned char data[REGION_SIZE*REGION_SIZE*2];
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			data[x + (y * REGION_SIZE)] = m_tiles[x][y].getId();
			data[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE] = m_tiles[x][y].getObjectId();
		}
	}
	tools::BinaryIO::write(getSaveLocation(), data, sizeof(data) / sizeof(unsigned char));
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
			m_tiles[x][y].update();

			int id = m_tiles[x][y].getFloorTexture();
			int objid = m_tiles[x][y].getObjectTexture();


			glm::vec4 arrayid = glm::vec4(1, 1, id % 16, (id - (id % 16)) / 16);
			glm::vec4 arrayobjectid = glm::vec4(1, 1, objid % 16, (objid - (objid % 16)) / 16);

			//std::cout << arrayid.x << ", " << arrayid.y << ", " << arrayid.z << ", " << arrayid.w << std::endl;

			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
			m_texture_off_array[tmp + (REGION_SIZE * REGION_SIZE * 6)] = arrayobjectid;
			m_texture_off_array[tmp++] = arrayid;
		}
	}
}