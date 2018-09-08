#include "textLayer.h"


namespace graphics {

	TextLayer::TextLayer(float x, float y, glm::vec3 color, std::string text, FontLoader &fontLoader) {
		setX(x); setY(y);
		setSizeX(1.0f); setSizeY(1.0f);
		setRotation(0.0f);

		mText = text;
		mColor = color;
		mFontLoader = &fontLoader;
	}

	TextLayer::TextLayer(float x, float y, float sizeX, float sizeY, float rotationZ, glm::vec3 color, std::string text, FontLoader &fontLoader) {
		setX(x); setY(y);
		setSizeX(sizeX); setSizeY(sizeY);
		setRotation(rotationZ);

		mText = text;
		mColor = color;
		mFontLoader = &fontLoader;
	}

	void TextLayer::render() {
		glm::mat4 ml_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(mX, mY, 0.0f));
		ml_matrix *= glm::rotate(glm::mat4(1.0f), mRotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
		ml_matrix *= glm::scale(glm::mat4(1.0f), glm::vec3(mSizeX, mSizeY, 1.0f));
		mFontLoader->renderText(mText, ml_matrix, mColor, mXAlign, mYAlign);
	}

	void TextLayer::update() {
		mX += mVelX; mY += mVelY;
		mVelX += mAccX; mVelY += mAccY;
		mRotationZ += mRotationSpeedZ;
		mRotationSpeedZ += mRotationAccZ;
	}

}