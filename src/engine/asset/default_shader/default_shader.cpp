#include "default_shader.hpp"

#include "engine/engine.hpp"
#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/interface/instance.hpp"
#include "engine/utility/formatted_error.hpp"
#include "engine/asset/asset_loader.hpp"



namespace oe::asset {

	constexpr char default_shader_name[] = "asset:default_shader";
	const static std::unordered_map<oe::graphics_api, ShaderInfo> default_shader_info = {
		{
			oe::graphics_api::OpenGL,
			ShaderInfo {
				default_shader_name,
				{
					{
						shader_stages::vertex_shader,
						{
							oe::asset::AssetLoader::resource_string("shader/default_shader/shader.vert.glsl"),
							{}
						}
					},
					{
						shader_stages::fragment_shader,
						{
							oe::asset::AssetLoader::resource_string("shader/default_shader/shader.frag.glsl"),
							{}
						}
					}
				}
			}
		},
		{
			oe::graphics_api::Vulkan,
			ShaderInfo {
				default_shader_name,
				{
					{
						shader_stages::vertex_shader,
						{
							oe::asset::AssetLoader::resource_string("shader/default_shader/shader.vert.glsl"),
							{}
						}
					},
					{
						shader_stages::fragment_shader,
						{
							oe::asset::AssetLoader::resource_string("shader/default_shader/shader.frag.glsl"),
							{}
						}
					}
				}
			}
		},
		{
			oe::graphics_api::none,
			{}
		}
	};



	DefaultShader::DefaultShader()
	{
		try{
			m_shader = { default_shader_info.at(Engine::getSingleton().engine_info.api) };
		}catch(...){
			throw oe::utils::formatted_error("Unknown graphics api: {}", static_cast<int>(Engine::getSingleton().engine_info.api));
		}
	}

	DefaultShader::DefaultShader(oe::shader_stages state_overwrite, std::string_view stage_code)
		: DefaultShader([state_overwrite, stage_code](){
				auto sti = default_shader_info.at(Engine::getSingleton().engine_info.api).stages;
				sti.at(state_overwrite).source = stage_code;
				return sti;
			}())
	{}

	DefaultShader::DefaultShader(std::string_view vertex_stage_code, std::string_view fragment_stage_code)
		: DefaultShader([vertex_stage_code, fragment_stage_code](){
			auto sti = default_shader_info.at(Engine::getSingleton().engine_info.api).stages;
			sti.at(oe::shader_stages::vertex_shader).source = vertex_stage_code;
			sti.at(oe::shader_stages::fragment_shader).source = fragment_stage_code;
			return sti;
		}())
	{}
	
	DefaultShader::DefaultShader(const std::unordered_map<oe::shader_stages, oe::ShaderStageInfo>& custom_sti)
	{
		oe::ShaderInfo info_copy = default_shader_info.at(Engine::getSingleton().engine_info.api);
		info_copy.stages = custom_sti;
		m_shader = { info_copy };
	}

	void DefaultShader::bind() const
	{
		m_shader->bind();
	}

	void DefaultShader::unbind() const
	{
		m_shader->unbind();
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