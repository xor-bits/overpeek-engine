#pragma once

#define NOISE_SCALE 0.02

#define LEVEL_WATER 0.5
#define LEVEL_SAND 0.52
#define LEVEL_GRASS 0.7
#define LEVEL_STONE 0.72

#include "tile.h"
#include "../logic/game.h"

class Region {
private:
	int m_x, m_y;
	Tile *m_tiles[REGION_SIZE][REGION_SIZE];
	GLint m_texture_off_array[REGION_SIZE * REGION_SIZE * 6 * 2];
	glm::mat4 m_ml_matrix;

	static graphics::VertexArray *m_vao;
	static graphics::Buffer *m_vbo;
	static graphics::Buffer *m_tbo;
	static graphics::Buffer *m_tid;
	static graphics::Shader *m_shader;
	//static GLuint m_tid;

	void createTiles();
	void loadTiles();

	inline std::string getSaveLocation() {
		std::string regionname = std::to_string(m_x) + ", " + std::to_string(m_y);
		return (SAVE_PATH + WORLD_NAME + "\\regions\\" + regionname).c_str();
	}

public:
	static void initRender(graphics::Shader *shader);

	Region(int x, int y);
	~Region();

	void render();
	void update(float offx, float offy);
	void saveTiles();

	inline Tile *getTile(unsigned int x, unsigned int y) { return m_tiles[x][y]; }
	inline int getX() { return (m_x - floor(RENDER_DST/2.0)) * REGION_SIZE; }
	inline int getY() { return (m_y - floor(RENDER_DST/2.0)) * REGION_SIZE; }

};