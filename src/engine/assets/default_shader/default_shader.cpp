#include "default_shader.hpp"

#include "shader.vert.hpp" // output to fragment shader
#include "shader.vert.g.hpp" // output to geometry shader

#include "shader.geom.l.hpp" // output to fragment shader
#include "shader.geom.p.hpp" // output to fragment shader

#include "shader.frag.hpp" // input from either geometry- or vertex shader

#include "engine/engine.hpp"
#include "engine/graphics/interface/shader.hpp"



namespace oe::assets {

	constexpr char default_shader_name[] = "asset:default_shader";
	const static std::unordered_map<oe::graphics_api, std::unordered_map<oe::polygon_mode, ShaderInfo>> default_shader_info = {
		{
			oe::graphics_api::OpenGL,
			{
				{
					oe::polygon_mode::fill,
					ShaderInfo {
						default_shader_name,
						{
							ShaderStageInfo {
								shader_stages::vertex_shader,
								shader_vert_gl
							},
							ShaderStageInfo {
								shader_stages::fragment_shader,
								shader_frag_gl
							}
						}
					}
				},
				{
					oe::polygon_mode::lines,
					ShaderInfo {
						default_shader_name,
						{
							ShaderStageInfo {
								shader_stages::vertex_shader,
								shader_vert_geom_gl
							},
							ShaderStageInfo {
								shader_stages::geometry_shader,
								shader_geom_lines_gl
							},
							ShaderStageInfo {
								shader_stages::fragment_shader,
								shader_frag_gl
							}
						}
					}
				},
				{
					oe::polygon_mode::points,
					ShaderInfo {
						default_shader_name,
						{
							ShaderStageInfo {
								shader_stages::vertex_shader,
								shader_vert_geom_gl
							},
							ShaderStageInfo {
								shader_stages::geometry_shader,
								shader_geom_points_gl
							},
							ShaderStageInfo {
								shader_stages::fragment_shader,
								shader_frag_gl
							}
						}
					}
				}
			}
		},
		{
			oe::graphics_api::Vulkan,
			{
				{
					oe::polygon_mode::fill,
					ShaderInfo {
						default_shader_name,
						{
							ShaderStageInfo {
								shader_stages::vertex_shader,
								shader_vert_vk
							},
							ShaderStageInfo {
								shader_stages::fragment_shader,
								shader_frag_vk
							}
						}
					}
				}
			}
		},
		{
			oe::graphics_api::None,
			{
			}
		}
	};

	DefaultShader::DefaultShader(oe::polygon_mode mode)
	{
		auto& info_map_iter = default_shader_info.find(Engine::getSingleton().engine_info.api);
		if(info_map_iter == default_shader_info.end()) { oe_error_terminate("Unknown graphics api"); }
		auto& info_map = info_map_iter->second;
		
		auto& info_iter = info_map.find(mode);
		if(info_map_iter == default_shader_info.end()) { oe_error_terminate("Unknown polygonmode"); }
		auto& info = info_iter->second;

		m_shader = oe::graphics::Shader(info);
	}

	void DefaultShader::bind() const
	{
		m_shader->bind();
	}

	void DefaultShader::unbind() const
	{
		m_shader->unbind();
	}

	void DefaultShader::setProjectionMatrix(const glm::mat4& pr_mat) const
	{
		m_shader->setUniformMat4("pr_matrix", pr_mat);
	}

	void DefaultShader::setViewMatrix(const glm::mat4& vw_mat) const
	{
		m_shader->setUniformMat4("vw_matrix", vw_mat);
	}
		
	void DefaultShader::setModelMatrix(const glm::mat4& ml_mat) const
	{
		m_shader->setUniformMat4("ml_matrix", ml_mat);
	}

	void DefaultShader::useTexture(bool use) const
	{
		m_shader->setUniform1i("usetex", use);
	}

}