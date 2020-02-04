#pragma once

#include "engine/internal_libs.h"

#include <string>



namespace oe::graphics {

	class Shader {
	protected:
		ShaderInfo m_shader_info;

	public:
		Shader(const ShaderInfo& shader_info);

		virtual void bind() const;
		virtual void unbind() const;


		virtual int getUniformLocation(std::string name) const;

		virtual void setUniform1f(std::string name, float value) const;
		virtual void setUniform2f(std::string name, glm::fvec2& value) const;
		virtual void setUniform3f(std::string name, glm::fvec3& value) const;
		virtual void setUniform4f(std::string name, glm::fvec4& value) const;

		virtual void setUniform1i(std::string name, int value) const;
		virtual void setUniform2i(std::string name, glm::ivec2& value) const;
		virtual void setUniform3i(std::string name, glm::ivec3& value) const;
		virtual void setUniform4i(std::string name, glm::ivec4& value) const;

		virtual void setUniformMat4(std::string name, glm::mat4& value) const;
	};

}