#pragma once

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/shader.hpp"



namespace oe::assets {

	class DefaultShader {
	private:
		oe::graphics::Shader* shader;
		const oe::graphics::Window* window;

	public:
		DefaultShader(const graphics::Window* _window);
		~DefaultShader();

		void bind() const;
		void unbind() const;

		void setProjectionMatrix(glm::mat4& pr_mat = glm::mat4(1.0)) const;
		void setViewMatrix(glm::mat4& vw_mat = glm::mat4(1.0)) const;
		void setModelMatrix(glm::mat4& ml_mat = glm::mat4(1.0)) const;
		void useTexture(bool use = true) const;

		oe::graphics::Shader& getShader() { return *shader; }

	};

}