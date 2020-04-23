#pragma once

#include "engine/graphics/interface/shader.hpp"



namespace oe::graphics {

	class GLShader : public Shader {
	private:
		unsigned int p_shader_program;

		unsigned int loadShader(const std::string& source, unsigned int shadertype);
		void shaderLog(unsigned int shader, unsigned int type) const;
		void programLog(int program, unsigned int type) const;

	public:
		GLShader(const ShaderInfo& shader_info);
		~GLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual int getUniformLocation(const std::string& name) const override;

		virtual void setUniform1f(const std::string& name, float value) const override;
		virtual void setUniform2f(const std::string& name, const glm::fvec2& value) const override;
		virtual void setUniform3f(const std::string& name, const glm::fvec3& value) const override;
		virtual void setUniform4f(const std::string& name, const glm::fvec4& value) const override;
		
		virtual void setUniform1i(const std::string& name, int value) const override;
		virtual void setUniform2i(const std::string& name, const glm::ivec2& value) const override;
		virtual void setUniform3i(const std::string& name, const glm::ivec3& value) const override;
		virtual void setUniform4i(const std::string& name, const glm::ivec4& value) const override;

		virtual void setUniformMat4(const std::string& name, const glm::mat4& value) const override;
	};

}