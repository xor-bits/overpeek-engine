#pragma once

#include "engine/internal_libs.hpp"
#include "engine/graphics/interface/instance.hpp"

#include <string>



namespace oe::graphics {

	class Shader {
	protected:
		ShaderInfo m_shader_info;

	public:
		Shader(const ShaderInfo& shader_info);
		~Shader();

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

	public:
		inline const ShaderInfo& getShaderInfo() { return m_shader_info; }
		
		virtual int getUniformLocation(const std::string& name) const = 0;

		virtual void setUniform1f(const std::string& name, float value) const = 0;
		virtual void setUniform2f(const std::string& name, const glm::fvec2& value) const = 0;
		virtual void setUniform3f(const std::string& name, const glm::fvec3& value) const = 0;
		virtual void setUniform4f(const std::string& name, const glm::fvec4& value) const = 0;

		virtual void setUniform1i(const std::string& name, int value) const = 0;
		virtual void setUniform2i(const std::string& name, const glm::ivec2& value) const = 0;
		virtual void setUniform3i(const std::string& name, const glm::ivec3& value) const = 0;
		virtual void setUniform4i(const std::string& name, const glm::ivec4& value) const = 0;

		virtual void setUniformMat4(const std::string& name, const glm::mat4& value) const = 0;
	};

}