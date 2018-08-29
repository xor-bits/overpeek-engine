#pragma once


#include <iostream>

namespace graphics {

	class Layer {
	protected:
		float mX = 0.0f;
		float mVelX = 0.0f;
		float mAccX = 0.0f;

		float mY = 0.0f;
		float mVelY = 0.0f;
		float mAccY = 0.0f;

		float mRotationZ = 0.0f;
		float mRotationSpeedZ = 0.0f;
		float mRotationAccZ = 0.0f;

		float mSizeX = 1.0f;
		float mSizeY = 1.0f;

	public:
		virtual void render() = 0;
		virtual void update() = 0;

		//Setters
		inline void setX(float x) { mX = x; }
		inline void setVelX(float velX) { mVelX = velX; }
		inline void setAccX(float accX) { mAccX = accX; }

		inline void setY(float y) { mY = y; }
		inline void setVelY(float velY) { mVelY = velY; }
		inline void setAccY(float accY) { mAccY = accY; }

		inline void setRotation(float rotation) { mRotationZ = rotation; }
		inline void setRotationSpeed(float rotationSpeed) { mRotationSpeedZ = rotationSpeed; }
		inline void setRotationAcc(float rotationAcc) { mRotationAccZ = rotationAcc; }

		inline void setSizeX(float sizeX) { mSizeX = sizeX; }
		inline void setSizeY(float sizeY) { mSizeY = sizeY; }

		//Getters
		inline float getX() { return mX; }
		inline float getVelX() { return mVelX; }
		inline float getAccX() { return mAccX; }
					 
		inline float getY() { return mY; }
		inline float getVelY() { return mVelY; }
		inline float getAccY() { return mAccY; }
					 
		inline float getRotation() { return mRotationZ; }
		inline float getRotationSpeed() { return mRotationSpeedZ; }
		inline float getRotationAcc() { return mRotationAccZ; }
					 
		inline float getSizeX() { return mSizeX; }
		inline float getSizeY() { return mSizeY; }

	};

}