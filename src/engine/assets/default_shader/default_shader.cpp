#include "default_shader.hpp"

#include "shader.vert.hpp"
#include "shader.frag.hpp"

#include "engine/engine.hpp"



namespace oe::assets {

	const std::string default_shader_name = "asset:default_shader";
	const ShaderInfo default_shader_info_vk = ShaderInfo{ default_shader_name, { ShaderStageInfo{ shader_stages::vertex_shader, shader_vert_vk }, ShaderStageInfo{ shader_stages::fragment_shader, shader_frag_vk } } };
	const ShaderInfo default_shader_info_gl = ShaderInfo{ default_shader_name, { ShaderStageInfo{ shader_stages::vertex_shader, shader_vert_gl }, ShaderStageInfo{ shader_stages::fragment_shader, shader_frag_gl } } };

	DefaultShader::DefaultShader(const graphics::Window* _window)
		: window(_window)
	{
		if (oe::Engine::getSingleton().engine_info.api == oe::graphics_api::OpenGL) {
			shader = window->createShader(default_shader_info_gl);
		}
		else {
			shader = window->createShader(default_shader_info_vk);
		}
	}

	DefaultShader::~DefaultShader() {
		window->destroyShader(shader);
	}

	void DefaultShader::bind() const {
		shader->bind();
	}

	void DefaultShader::unbind() const {
		shader->unbind();
	}

	void DefaultShader::setProjectionMatrix(glm::mat4& pr_mat) const {
		shader->setUniformMat4("pr_matrix", pr_mat);
	}

	void DefaultShader::setViewMatrix(glm::mat4& vw_mat) const {
		shader->setUniformMat4("vw_matrix", vw_mat);
	}
		
	void DefaultShader::setModelMatrix(glm::mat4& ml_mat) const {
		shader->setUniformMat4("ml_matrix", ml_mat);
	}

	void DefaultShader::useTexture(bool use) const {
		shader->setUniform1i("usetex", use);
	}

}