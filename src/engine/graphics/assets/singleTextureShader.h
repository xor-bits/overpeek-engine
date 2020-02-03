#pragma once

#include "engine/graphics/shader.h"



namespace oe::graphics {

	class SingleTextureShader : public Shader {
	public:
		SingleTextureShader();

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

	typedef SingleTextureShader SpriteShader;
}