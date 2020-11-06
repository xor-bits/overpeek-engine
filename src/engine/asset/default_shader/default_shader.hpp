#pragma once

#include "engine/enum.hpp"
#include "engine/interfacegen.hpp"



namespace oe::asset
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

		// set projection matrix (for mvp_matrix)
		void setProjectionMatrix(const glm::mat4& pr_mat = glm::mat4(1.0));
		
		// set view matrix (for mvp_matrix)
		void setViewMatrix(const glm::mat4& vw_mat = glm::mat4(1.0));
		
		// set model matrix (for mvp_matrix)
		void setModelMatrix(const glm::mat4& ml_mat = glm::mat4(1.0));
		
		// attempt to update the matrix (mvp_matrix)
		void updateMVP() const;

		// set color (u_color)
		void setColor(const oe::color& color) const;
		
		// set texture (non solid color) mode on/off (u_usetex)
		void setTexture(bool use = true) const;

		// get the base shader
		const oe::graphics::Shader& getShader() { return m_shader; }

	};

}