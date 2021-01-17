#pragma once

#include "engine/enum.hpp"
#include "engine/interfacegen.hpp"



namespace oe::asset
{

	class DefaultShader
	{
	private:
		oe::graphics::Shader m_shader;

		glm::mat4 m_pr_mat = glm::mat4(1.0f);
		glm::mat4 m_vw_mat = glm::mat4(1.0f);
		glm::mat4 m_ml_mat = glm::mat4(1.0f);

	public:
		DefaultShader();
		DefaultShader(oe::shader_stages state_overwrite, std::string_view stage_code);
		DefaultShader(std::string_view vertex_stage_code, std::string_view fragment_stage_code);
		DefaultShader(const std::unordered_map<oe::shader_stages, oe::ShaderStageInfo>& custom_sti);

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