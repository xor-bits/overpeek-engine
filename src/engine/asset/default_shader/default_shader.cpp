#include "default_shader.hpp"

#include "engine/engine.hpp"
#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/interface/instance.hpp"
#include "engine/utility/formatted_error.hpp"
#include "engine/asset/asset_loader.hpp"



namespace oe::asset {

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
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.vert.glsl"),
								{}
							},
							ShaderStageInfo {
								shader_stages::fragment_shader,
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.frag.glsl"),
								{}
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
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.vert.g.glsl"),
								{}
							},
							ShaderStageInfo {
								shader_stages::geometry_shader,
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.geom.l.glsl"),
								{}
							},
							ShaderStageInfo {
								shader_stages::fragment_shader,
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.frag.glsl"),
								{}
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
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.vert.g.glsl"),
								{}
							},
							ShaderStageInfo {
								shader_stages::geometry_shader,
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.geom.p.glsl"),
								{}
							},
							ShaderStageInfo {
								shader_stages::fragment_shader,
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.frag.glsl"),
								{}
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
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.vert.glsl"),
								{}
							},
							ShaderStageInfo {
								shader_stages::fragment_shader,
								oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.frag.glsl"),
								{}
							}
						}
					}
				}
			}
		},
		{
			oe::graphics_api::none,
			{
			}
		}
	};

	

	DefaultShader::DefaultShader(oe::polygon_mode mode)
		: m_mode(oe::polygon_mode::fill)
	{
		if(oe::Engine::getSingleton().instance->versionNumber() < 33)
		{
			m_mode = mode; // glPolygonmode for gl 3.2 or earlier
			mode = oe::polygon_mode::fill; // construct shader without geometry stage
		}

		try{
			m_shader = oe::graphics::Shader(default_shader_info.at(Engine::getSingleton().engine_info.api).at(mode));
		}catch(...){
			throw oe::utils::formatted_error("Unknown polygonmode: {} or graphics api: {}", static_cast<int>(mode), static_cast<int>(Engine::getSingleton().engine_info.api));
		}
	}

	void DefaultShader::bind() const
	{
		m_shader->bind();
		if(m_mode != oe::polygon_mode::fill) oe::Engine::getSingleton().polygonMode(m_mode);
	}

	void DefaultShader::unbind() const
	{
		m_shader->unbind();
		oe::Engine::getSingleton().polygonMode(oe::polygon_mode::fill);
	}

	void DefaultShader::setProjectionMatrix(const glm::mat4& pr_mat)
	{
		if(m_pr_mat == pr_mat) return;
		m_pr_mat = pr_mat;
		updateMVP();
	}

	void DefaultShader::setViewMatrix(const glm::mat4& vw_mat)
	{
		if(m_vw_mat == vw_mat) return;
		m_vw_mat = vw_mat;
		updateMVP();
	}
		
	void DefaultShader::setModelMatrix(const glm::mat4& ml_mat)
	{
		if(m_ml_mat == ml_mat) return;
		m_ml_mat = ml_mat;
		updateMVP();
	}

	void DefaultShader::updateMVP() const
	{
		m_shader->setUniform("mvp_matrix", m_pr_mat * m_vw_mat * m_ml_mat);
	}

	void DefaultShader::setColor(const oe::color& color) const
	{
		m_shader->setUniform("u_color", color);
	}

	void DefaultShader::setTexture(bool use) const
	{
		m_shader->setUniform("u_usetex", (int)use);
	}

}