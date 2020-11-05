#pragma once

#include "engine/enum.hpp"
#include "engine/interfacegen.hpp"



namespace oe::assets
{
	// close to 1 to 1 copy from DefaultShader with SDF addition and removed polygonmode
	class FontShader
	{
	private:
		oe::graphics::Shader m_shader;

		glm::mat4 m_pr_mat = glm::mat4(1.0f);
		glm::mat4 m_vw_mat = glm::mat4(1.0f);
		glm::mat4 m_ml_mat = glm::mat4(1.0f);

	public:
		FontShader();

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
		
		// SDF font specifics
		
		// set SDF mode on/off (u_usesdf)
		void setSDF(bool use = true) const;

		// set font width (u_sdf_width)
		void setWidth(float width) const;

		// set font AA edge width (u_sdf_edge)
		void setEdge(float edge) const;

		// set font outline width (u_sdf_width)
		void setOutlineWidth(float width) const;

		// set font outline AA edge width (u_sdf_edge)
		void setOutlineEdge(float edge) const;

		// set font outline color (u_sdf_outline_color)
		void setOutlineColor(const oe::color& c) const;

		// get the base shader
		const oe::graphics::Shader& getShader() { return m_shader; }
	};

}