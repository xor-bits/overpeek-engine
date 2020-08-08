#pragma once

#include "engine/enum.hpp"
#include "engine/interfacegen.hpp"



namespace oe::assets
{
	class DefaultShader
	{
	private:
		oe::graphics::Shader m_shader;
		oe::polygon_mode m_mode;

	public:
		DefaultShader(oe::polygon_mode mode = oe::polygon_mode::fill);

		void bind() const;
		void unbind() const;

		void setProjectionMatrix(const glm::mat4& pr_mat = glm::mat4(1.0)) const;
		void setViewMatrix(const glm::mat4& vw_mat = glm::mat4(1.0)) const;
		void setModelMatrix(const glm::mat4& ml_mat = glm::mat4(1.0)) const;
		void useTexture(bool use = true) const;

		const oe::graphics::Shader& getShader() { return m_shader; }

	};

}