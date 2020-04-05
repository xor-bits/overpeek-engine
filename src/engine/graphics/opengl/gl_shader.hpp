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