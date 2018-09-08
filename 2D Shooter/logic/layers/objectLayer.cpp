#include "objectLayer.h"


namespace graphics {

	ObjectLayer::ObjectLayer(float x, float y, float sizeX, float sizeY, glm::vec4 color, SimpleRenderer &renderer, int layerObjectType, bool center) {
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
			if (center) {
				mPoints.push_back(glm::vec2(0, -sizeY/2));
				mPoints.push_back(glm::vec2(sizeX/2, sizeY/2));
				mPoints.push_back(glm::vec2(-sizeX/2, sizeY/2));
			}
			else {
				mPoints.push_back(glm::vec2(sizeX / 2, 0));
				mPoints.push_back(glm::vec2(sizeX, sizeY));
				mPoints.push_back(glm::vec2(0, sizeY));
			}
		}
		else if (mLayerObjectType == LAYER_OBJECT_QUAD) {
			if (center) {
				mPoints.push_back(glm::vec2(-sizeX/2, sizeY/2));
				mPoints.push_back(glm::vec2(sizeX/2, sizeY/2));
				mPoints.push_back(glm::vec2(sizeX/2, -sizeY/2));

				mPoints.push_back(glm::vec2(-sizeX / 2, sizeY/2));
				mPoints.push_back(glm::vec2(-sizeX / 2, -sizeY/2));
				mPoints.push_back(glm::vec2(sizeX/2, -sizeY/2));
			}
			else {
				mPoints.push_back(glm::vec2(0, 0));
				mPoints.push_back(glm::vec2(sizeX, 0));
				mPoints.push_back(glm::vec2(sizeX, -sizeY));

				mPoints.push_back(glm::vec2(0, 0));
				mPoints.push_back(glm::vec2(0, -sizeY));
				mPoints.push_back(glm::vec2(sizeX, -sizeY));
			}
		}
	}

	void ObjectLayer::render() {
		if (mLayerObjectType == LAYER_OBJECT_NONE) {
			//Nothing to render
		}
		else if (mLayerObjectType == LAYER_OBJECT_TRIANGLE) {
			glm::mat4 model;
			model = glm::translate(glm::mat4(1.0f), glm::vec3(mX + mRelativeX, mY + mRelativeY, 0.0f));
			model *= glm::rotate(glm::mat4(1.0f), mRotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
			//mRenderer->renderTriangle(model, 0.0f, 0.0f, mPoints, mColor);
		}
		else if (mLayerObjectType == LAYER_OBJECT_QUAD) {
			//glm::mat4 model;
			//model = glm::translate(glm::mat4(1.0f), glm::vec3(mX + mRelativeX, mY + mRelativeY, 0.0f));
			//model *= glm::rotate(glm::mat4(1.0f), mRotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
			//mRenderer->renderQuad(model, 0.0f, 0.0f, mPoints, mColor);
		}
		else if (mLayerObjectType == LAYER_OBJECT_CIRCLE) {
			//mRenderer->renderCircle(glm::mat4(1.0f), mX, mY, mSizeX, 16, mColor);
		}
	}

	void ObjectLayer::update() {
		mX += mVelX; mY += mVelY;
		mVelX += mAccX; mVelY += mAccY;
	}

}