#pragma once

#include "engine/graphics/interface/shader.hpp"
#include "vk_logical_device.hpp"
#include "vk_window.hpp"



namespace oe::graphics {

	class VKShader : public Shader {
	public:
		vk::PipelineLayout m_pipeline_layout;
		vk::Pipeline m_pipeline;
		
		const VKLogicalDevice* m_logical_device;
		const VKWindow* m_window;

	public:
		VKShader(const VKWindow* window, const ShaderInfo& shader_info);
		~VKShader();

		// Inherited via Shader
		virtual void bind() const override;
		virtual void unbind() const override;

		virtual int getUniformLocation(std::string name) const override;

		virtual void setUniform1f(std::string name, float value) const override;
		virtual void setUniform2f(std::string name, glm::fvec2& value) const override;
		virtual void setUniform3f(std::string name, glm::fvec3& value) const override;
		virtual void setUniform4f(std::string name, glm::fvec4& value) const override;
		virtual void setUniform1i(std::string name, int value) const override;
		virtual void setUniform2i(std::string name, glm::ivec2& value) const override;
		virtual void setUniform3i(std::string name, glm::ivec3& value) const override;
		virtual void setUniform4i(std::string name, glm::ivec4& value) const override;
		virtual void setUniformMat4(std::string name, glm::mat4& value) const override;

	};

}