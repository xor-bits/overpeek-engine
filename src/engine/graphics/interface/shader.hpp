#pragma once

#include "engine/internal_libs.hpp"
#include "engine/graphics/interface/instance.hpp"

#include <string>



namespace oe::graphics {

	class IShader {
	protected:
		ShaderInfo m_shader_info;

	public:
		IShader(const ShaderInfo& shader_info);
		virtual ~IShader();

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

	public:
		inline const ShaderInfo& getShaderInfo() { return m_shader_info; }
		
		virtual int getUniformLocation(const std::string& name) = 0;

		virtual void setUniform1f(const std::string& name, float value) = 0;
		virtual void setUniform2f(const std::string& name, const glm::fvec2& value) = 0;
		virtual void setUniform3f(const std::string& name, const glm::fvec3& value) = 0;
		virtual void setUniform4f(const std::string& name, const glm::fvec4& value) = 0;

		virtual void setUniform1i(const std::string& name, int value) = 0;
		virtual void setUniform2i(const std::string& name, const glm::ivec2& value) = 0;
		virtual void setUniform3i(const std::string& name, const glm::ivec3& value) = 0;
		virtual void setUniform4i(const std::string& name, const glm::ivec4& value) = 0;

		virtual void setUniformMat4(const std::string& name, const glm::mat4& value) = 0;
	};

}