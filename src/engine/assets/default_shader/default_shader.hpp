#pragma once

#include "engine/enum.hpp"
#include "engine/interfacegen.hpp"



namespace oe::assets
{
	class DefaultShader
	{
	private:
		oe::graphics::Shader m_shader;
		oe::polygon_mode m_mode; // if geometry shaders are not supported (OGL 3.3 >= ) (like Mesa 20.0.8)

		glm::mat4 m_pr_mat = glm::mat4(1.0f);
		glm::mat4 m_vw_mat = glm::mat4(1.0f);
		glm::mat4 m_ml_mat = glm::mat4(1.0f);

	public:
		DefaultShader(oe::polygon_mode mode = oe::polygon_mode::fill);

		void bind() const;
		void unbind() const;

		void setProjectionMatrix(const glm::mat4& pr_mat = glm::mat4(1.0));
		void setViewMatrix(const glm::mat4& vw_mat = glm::mat4(1.0));
		void setModelMatrix(const glm::mat4& ml_mat = glm::mat4(1.0));
		void updateMVP() const;

		void setColor(const glm::vec4& color) const; // set uniform color
		void setTexture(bool use = true) const; // false = use vertex color and uniform color, true = use texture, vertex color and uniform color

		const oe::graphics::Shader& getShader() { return m_shader; }

	};

}