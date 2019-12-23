#pragma once

#include "graphics/shader.h"



namespace oe::graphics {

	class MultiTextureShader : public Shader {
	public:
		MultiTextureShader();

		void modelMatrix(glm::mat4 matrix) const {
			setUniformMat4("ml_matrix", matrix);
		}

		void viewMatrix(glm::mat4 matrix) const {
			setUniformMat4("vw_matrix", matrix);
		}

		void projectionMatrix(glm::mat4 matrix) const {
			setUniformMat4("pr_matrix", matrix);
		}

		void useTexture(bool use_tex) const {
			setUniform1i("usetex", use_tex ? 1 : 0);
		}
	};

}