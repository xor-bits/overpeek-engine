#include "textLayer.h"


namespace graphics {

	TextLayer::TextLayer(float x, float y, glm::vec3 color, std::string text, FontLoader &fontLoader) {
		setX(x); setY(y);
		setSizeX(1.0f); setSizeY(1.0f);
		setRotation(1.0f);

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
		mFontLoader->renderText(mText, mX + mRelativeX, mY + mRelativeY, mSizeX, mSizeY, mColor);
	}

	void TextLayer::update() {
		mX += mVelX; mY += mVelY;
		mVelX += mAccX; mVelY += mAccY;
	}

}