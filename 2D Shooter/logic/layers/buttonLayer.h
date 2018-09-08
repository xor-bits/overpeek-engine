#pragma once


#include "layer.h"
#include "../../graphics/window.h"
#include "objectLayer.h"
#include "textLayer.h"

#include <glm/glm.hpp>
#include <string>
#include <iostream>

namespace graphics {

	class ButtonLayer : public Layer {
	private:
		glm::vec2 mTopRightCornerOffset;
		glm::vec2 mBottomRightCornerOffset;
		Window *mWindow;

		TextLayer *mTextLayer = nullptr;
		ObjectLayer *mObjectLayer = nullptr;
		
		bool mHoldingAlready;
		double mMouseX, mMouseY;

		void(*mHoverCallback)();
		void(*mClickCallback)();

	public:
		ButtonLayer(float x, float y, float w, float h, graphics::Window &window);

		void setTextLayer(TextLayer &textLayer);
		void setObjectLayer(ObjectLayer &objectLayer);

		void setHoverCallback(void(*callback)());
		void setClickCallback(void(*callback)());

		void render();
		void update();

	};

}