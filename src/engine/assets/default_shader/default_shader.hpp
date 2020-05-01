#pragma once

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/shader.hpp"



namespace oe::assets {

	class DefaultShader {
	private:
		oe::graphics::Shader* shader;

	public:
		DefaultShader();
		~DefaultShader();

		void bind() const;
		void unbind() const;

		void setProjectionMatrix(const glm::mat4& pr_mat = glm::mat4(1.0)) const;
		void setViewMatrix(const glm::mat4& vw_mat = glm::mat4(1.0)) const;
		void setModelMatrix(const glm::mat4& ml_mat = glm::mat4(1.0)) const;
		void useTexture(bool use = true) const;

		oe::graphics::Shader& getShader() { return *shader; }

	};

}