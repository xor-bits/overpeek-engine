#pragma once

#define NOISE_SCALE 0.01

#define LEVEL_WATER 0.0
#define LEVEL_SAND 0.05
#define LEVEL_GRASS 0.15

#include "tile.h"
#include "../logic/game.h"

class Region {
private:
	int m_x, m_y;
	Tile *m_tiles[REGION_SIZE][REGION_SIZE];
	GLfloat m_texture_id_array[REGION_SIZE * REGION_SIZE * 6 * 2];
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

<<<<<<< HEAD
	void render();
	void update(float offx, float offy);
=======
	void render(float offx, float offy);
	void update();
	void saveTiles();
>>>>>>> 58354a52ddbe18f20adb822ed668e871de978bb7

	inline Tile *getTile(unsigned int x, unsigned int y) { return m_tiles[x][y]; }
	inline int getX() { return (m_x - floor(RENDER_DST/2.0)) * REGION_SIZE; }
	inline int getY() { return (m_y - floor(RENDER_DST/2.0)) * REGION_SIZE; }

};