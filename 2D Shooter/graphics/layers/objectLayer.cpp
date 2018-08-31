#include "objectLayer.h"


namespace graphics {

	ObjectLayer::ObjectLayer(glm::vec2 topRightCorner, glm::vec2 botttomLeftCorner, glm::vec4 color, SimpleRenderer &renderer) {
		setX(0.0f); setY(0.0f);
		setSizeX(1.0f); setSizeY(1.0f);
		setRotation(0.0f);

		mLayerObjectType = LAYER_OBJECT_QUAD;
		mColor = color;
		mRenderer = &renderer;

		mPoints.push_back(glm::vec2(botttomLeftCorner.x, topRightCorner.y));
		mPoints.push_back(glm::vec2(topRightCorner.x, topRightCorner.y));
		mPoints.push_back(glm::vec2(topRightCorner.x, botttomLeftCorner.y));

		mPoints.push_back(glm::vec2(botttomLeftCorner.x, topRightCorner.y));
		mPoints.push_back(glm::vec2(botttomLeftCorner.x, botttomLeftCorner.y));
		mPoints.push_back(glm::vec2(topRightCorner.x, botttomLeftCorner.y));
	}

	ObjectLayer::ObjectLayer(float x, float y, float sizeX, float sizeY, glm::vec4 color, SimpleRenderer &renderer, int layerObjectType) {
		setX(x); setY(y);
		setSizeX(sizeX); setSizeY(sizeY);
		setRotation(0.0f);

		mColor = color;
		mRenderer = &renderer;

		mLayerObjectType = layerObjectType;
		if (mLayerObjectType == LAYER_OBJECT_NONE) {
			//Nothing to push
		}
		else if (mLayerObjectType == LAYER_OBJECT_TRIANGLE) {
			mPoints.push_back(glm::vec2(x + sizeX / 2, y));
			mPoints.push_back(glm::vec2(x + sizeX    , y + sizeY));
			mPoints.push_back(glm::vec2(x            , y + sizeY));
		}
		else if (mLayerObjectType == LAYER_OBJECT_QUAD) {
			mPoints.push_back(glm::vec2(0, 0));
			mPoints.push_back(glm::vec2(sizeX, 0));
			mPoints.push_back(glm::vec2(sizeX, -sizeY));

			mPoints.push_back(glm::vec2(0, 0));
			mPoints.push_back(glm::vec2(0, -sizeY));
			mPoints.push_back(glm::vec2(sizeX, -sizeY));
		}
	}

	void ObjectLayer::render() {
		if (mLayerObjectType == LAYER_OBJECT_NONE) {
			//Nothing to render
		}
		else if (mLayerObjectType == LAYER_OBJECT_TRIANGLE) {
			mRenderer->renderTriangle(mX + mRelativeX, mY + mRelativeY, mPoints, mColor);
		}
		else if (mLayerObjectType == LAYER_OBJECT_QUAD) {
			mRenderer->renderQuad(mX + mRelativeX, mY + mRelativeY, mPoints, mColor);
		}
	}

	void ObjectLayer::update() {
		mX += mVelX; mY += mVelY;
		mVelX += mAccX; mVelY += mAccY;
	}

}