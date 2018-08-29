#pragma once


#include "layer.h"
#include "../fontLoader.h"
#include "../shader.h"

#include <string>
#include <glm/glm.hpp>

namespace graphics {

	class TextLayer : public Layer {
	private:
		std::string mText;
		glm::vec3 mColor;
		FontLoader *mFontLoader;
		Shader *mShader;

	public:
		TextLayer(float x, float y, glm::vec3 color, std::string text, FontLoader &fontLoader);
		TextLayer(float x, float y, float sizeX, float sizeY, float rotationZ, glm::vec3 color, std::string text, FontLoader &fontLoader);

		void render();
		void update();

	};

}