#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "window.h"

namespace graphics {

	class Camera {
	private:
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		glm::vec3 mPos;
		GLfloat mPitch;
		GLfloat mYaw;
		GLuint mFov;
		GLfloat mAspect;

	public:
		Camera(glm::vec3 pos, GLfloat pitch, GLfloat yaw, GLuint fov, GLfloat aspect);

		void updateCamera();

		void setPosition(glm::vec3 pos) { mPos = pos; updateCamera(); }
		void setRotation(GLfloat yaw, GLfloat pitch) { mPitch = pitch; mYaw = yaw; updateCamera(); }
		void setProjection(GLuint fov, GLfloat aspect);
		
		inline glm::mat4 getViewMatrix() { return mViewMatrix; }
		inline glm::mat4 getProjectionMatrix() { return mProjectionMatrix; }

	};

}