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
	Tile m_tiles[REGION_SIZE][REGION_SIZE];
	glm::fvec4 m_texture_off_array[REGION_SIZE * REGION_SIZE * 6 * 2];
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

	inline Tile getTile(unsigned int x, unsigned int y) { return m_tiles[x][y]; }
	inline int getTileId(unsigned int x, unsigned int y) { return m_tiles[x][y].getId(); }
	inline int getTileObjectId(unsigned int x, unsigned int y) { return m_tiles[x][y].getObjectId(); }

	inline void setTileId(unsigned int x, unsigned int y, int id) { m_tiles[x][y].setId(id); }
	inline void setTileObjectId(unsigned int x, unsigned int y, int id) { m_tiles[x][y].setObjectId(id); }
	inline void addObjectHealth(unsigned int x, unsigned int y, float amount) { m_tiles[x][y].addObjectHealth(amount); }
	inline void healObject(unsigned int x, unsigned int y) { m_tiles[x][y].healObject(); }
	
	inline int getTileX(unsigned int x, unsigned int y) { return m_tiles[x][y].getX(); }
	inline int getTileY(unsigned int x, unsigned int y) { return m_tiles[x][y].getY(); }


	inline int getX() { return (m_x - floor(RENDER_DST/2.0)) * REGION_SIZE; }
	inline int getY() { return (m_y - floor(RENDER_DST/2.0)) * REGION_SIZE; }

};