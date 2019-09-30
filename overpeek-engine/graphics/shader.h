#pragma once

#include "../internal_libs.h"

#include <string>



namespace oe {

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

		unsigned int loadShader(std::string source, unsigned int shadertype);
		void shaderLog(unsigned int shader, unsigned int type);
		void programLog(int program, unsigned int type);

	public:
		Shader(std::string name = "default_shader");

		void load(std::string compute_shader_source);
		void load(std::string vertex_shader_source, std::string fragment_shader_source);
		void load(std::string vertex_shader_source, std::string fragment_shader_source, std::string geometry_shader_source);
		
		void bind();
		void unbind();

		int getUniformLocation(std::string name);
		
		void setUniform1f	(std::string name, float value);
		void setUniform2f	(std::string name, glm::fvec2 &value);
		void setUniform3f	(std::string name, glm::fvec3 &value);
		void setUniform4f	(std::string name, glm::fvec4 &value);
		void setUniform1i	(std::string name, int value);
		void setUniform2i	(std::string name, glm::ivec2 &value);
		void setUniform3i	(std::string name, glm::ivec3 &value);
		void setUniform4i	(std::string name, glm::ivec4 &value);
		void setUniformMat4	(std::string name, glm::mat4 &value);
	};

}