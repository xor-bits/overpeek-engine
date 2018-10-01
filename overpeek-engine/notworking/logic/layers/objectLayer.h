#pragma once


#include "layer.h"
#include "../../graphics/shader.h"
#include "../../graphics/simpleRenderer.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define LAYER_OBJECT_NONE 0
#define LAYER_OBJECT_TRIANGLE 1
#define LAYER_OBJECT_QUAD 2
#define LAYER_OBJECT_PENTAGON 3
#define LAYER_OBJECT_HEXAGON 4
#define LAYER_OBJECT_CIRCLE 5

namespace graphics {

	class ObjectLayer : public Layer {
	private:
		int mLayerObjectType;
		std::vector<glm::vec2> mPoints;
		glm::vec4 mColor;
		SimpleRenderer *mRenderer;

	public:
		ObjectLayer(float x, float y, float sizeX, float sizeY, glm::vec4 color, SimpleRenderer &renderer, int layerObjectType, bool center);

		void render();
		void update();

		inline void setPoint(unsigned int atArray, glm::vec2 point) { mPoints.at(atArray) = point; }
		inline std::vector<glm::vec2> getPoints() { return mPoints; }
		//inline void pushPoint(glm::vec2 point) { mPoints.push_back(point); }
		//inline glm::vec2 getPoint(unsigned int atArray) { return mPoints.at(atArray); }


		inline void setColor(glm::vec4 color) { mColor = color; }
		inline glm::vec4 getColor() { return mColor; }

	};

}