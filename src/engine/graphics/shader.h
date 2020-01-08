#pragma once

#include "engine/internal_libs.h"

#include <string>



namespace oe::graphics {

	class ShaderException : public std::exception {
	private:
		std::string p_error;

	public:
		ShaderException(std::string error) {
			p_error = error;
		}

		virtual const char* what() const throw() override {
			return p_error.c_str();
		}
	};

	class Shader {
	private:
		unsigned int p_shader_program;
		std::string p_shader_name;

		unsigned int loadShader(const std::string& source, unsigned int shadertype);
		void shaderLog(unsigned int shader, unsigned int type) const;
		void programLog(int program, unsigned int type) const;

	public:
		Shader(std::string name = "default_shader");

		void load(const std::string& compute_shader_source);
		void load(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
		void load(const std::string& vertex_shader_source, const std::string& fragment_shader_source, const std::string& geometry_shader_source);
		
		void bind() const;
		void unbind() const;

		int getUniformLocation(std::string name) const;
		
		void setUniform1f	(std::string name, float value) const;
		void setUniform2f	(std::string name, glm::fvec2 &value) const;
		void setUniform3f	(std::string name, glm::fvec3 &value) const;
		void setUniform4f	(std::string name, glm::fvec4 &value) const;
		void setUniform1i	(std::string name, int value) const;
		void setUniform2i	(std::string name, glm::ivec2 &value) const;
		void setUniform3i	(std::string name, glm::ivec3 &value) const;
		void setUniform4i	(std::string name, glm::ivec4 &value) const;
		void setUniformMat4	(std::string name, glm::mat4 &value) const;
	};

}