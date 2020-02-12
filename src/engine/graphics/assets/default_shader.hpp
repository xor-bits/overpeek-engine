#pragma once

#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/interface/instance.hpp"



namespace oe::graphics {

	class DefaultShader {
	private:
		const Window* m_window;
	
	public:
		Shader *shader; // inheritance not possible

	public:
		DefaultShader(const Window* instance);
		~DefaultShader();

		inline void bind() { shader->bind(); }
		inline void unbind() { shader->unbind(); }

		inline void modelMatrix(glm::mat4 matrix) const {
			shader->setUniformMat4("ml_matrix", matrix);
		}
		
		inline void viewMatrix(glm::mat4 matrix) const {
			shader->setUniformMat4("vw_matrix", matrix);
		}
		
		inline void projectionMatrix(glm::mat4 matrix) const {
			shader->setUniformMat4("pr_matrix", matrix);
		}
		
		inline void useTexture(bool use_tex) const {
			shader->setUniform1i("usetex", use_tex ? 1 : 0);
		}

	};
}