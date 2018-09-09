#include "buttonLayer.h"


namespace graphics {

	ButtonLayer::ButtonLayer(float x, float y, float w, float h, graphics::Window &window) {
		setX(x); setY(y);
		setSizeX(w); setSizeY(h);
		setRotation(0.0f);

		mWindow = &window;
	}

	void ButtonLayer::setTextLayer(TextLayer &textLayer) {
		mTextLayer = &textLayer;
	}

	void ButtonLayer::setObjectLayer(ObjectLayer &objectLayer) {
		mObjectLayer = &objectLayer;
	}

	void ButtonLayer::setClickCallback(void(*callback)()) {
		mClickCallback = callback;
	}

	void ButtonLayer::setHoverCallback(void(*callback)()) {
		mHoverCallback = callback;
	}

	void ButtonLayer::render() {
		if (mObjectLayer != nullptr) mObjectLayer->render();
		if (mTextLayer != nullptr) mTextLayer->render();
	}

	void ButtonLayer::update() {
		mX += mVelX; mY += mVelY;
		mVelX += mAccX; mVelY += mAccY;

		if (mObjectLayer != nullptr) {
			mObjectLayer->setRelativeX(mX);
			mObjectLayer->setRelativeY(mY);
		}
		if (mTextLayer != nullptr) {
			mTextLayer->setRelativeX(mX);
			mTextLayer->setRelativeY(mY);
		}

		mMouseX = mWindow->getMousePos().x;
		mMouseY = mWindow->getMousePos().y;
		if ((mMouseX >= mX && mMouseX <= mX + mSizeX) && (mMouseY >= mY - mSizeY && mMouseY <= mY)) {
			(*mHoverCallback)();
			if (mWindow->getButton(GLFW_MOUSE_BUTTON_1)) {
				if (!mHoldingAlready) {
					(*mClickCallback)();
				}
			} else {
				mHoldingAlready = false;
			}
		}
		else {
			if (mWindow->getButton(GLFW_MOUSE_BUTTON_1)) {
				mHoldingAlready = true;
			}
			else {
				mHoldingAlready = false;
			}
		}
	}

}