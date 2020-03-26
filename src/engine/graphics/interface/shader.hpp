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
		virtual ~Shader();

		virtual ShaderInfo default_shader_info() = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

	public:
		inline const ShaderInfo& getShaderInfo() { return m_shader_info; }
		
		virtual int getUniformLocation(std::string name) const = 0;

		virtual void setUniform1f(std::string name, float value) const = 0;
		virtual void setUniform2f(std::string name, glm::fvec2& value) const = 0;
		virtual void setUniform3f(std::string name, glm::fvec3& value) const = 0;
		virtual void setUniform4f(std::string name, glm::fvec4& value) const = 0;

		virtual void setUniform1i(std::string name, int value) const = 0;
		virtual void setUniform2i(std::string name, glm::ivec2& value) const = 0;
		virtual void setUniform3i(std::string name, glm::ivec3& value) const = 0;
		virtual void setUniform4i(std::string name, glm::ivec4& value) const = 0;

		virtual void setUniformMat4(std::string name, glm::mat4& value) const = 0;
	};

}