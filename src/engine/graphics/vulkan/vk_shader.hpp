#pragma once
#ifdef BUILD_VULKAN

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

		virtual int getUniformLocation(const std::string& name name) const override;

		virtual void setUniform1f(const std::string& name name, float value) const override;
		virtual void setUniform2f(const std::string& name name, const glm::fvec2& value) const override;
		virtual void setUniform3f(const std::string& name name, const glm::fvec3& value) const override;
		virtual void setUniform4f(const std::string& name name, const glm::fvec4& value) const override;

		virtual void setUniform1i(const std::string& name name, int value) const override;
		virtual void setUniform2i(const std::string& name name, const glm::ivec2& value) const override;
		virtual void setUniform3i(const std::string& name name, const glm::ivec3& value) const override;
		virtual void setUniform4i(const std::string& name name, const glm::ivec4& value) const override;

		virtual void setUniformMat4(const std::string& name name, const glm::mat4& value) const override;

	};

}
#endif