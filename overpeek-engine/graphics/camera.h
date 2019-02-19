#pragma once

#include ""
#include <glm/glm.hpp>

namespace oe {

	class Camera {
	private:
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		glm::vec3 mPos;
		float mPitch;
		float mYaw;
		unsigned int mFov;
		float mAspect;

	public:
		Camera(glm::vec3 pos, float pitch, float yaw, unsigned int fov, float aspect);

		void updateCamera();

		void setPosition(glm::vec3 pos) { mPos = pos; updateCamera(); }
		void setRotation(float yaw, float pitch) { mPitch = pitch; mYaw = yaw; updateCamera(); }
		void setProjection(unsigned int fov, float aspect);
		
		inline glm::mat4 getViewMatrix() { return mViewMatrix; }
		inline glm::mat4 getProjectionMatrix() { return mProjectionMatrix; }

	};

}