#include "shader.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/utility/filereader.h"



namespace oe::graphics {

	unsigned int Shader::loadShader(const std::string& source, unsigned int shadertype) {
		//Load and compile
		const char* shaderChar = source.c_str();

		GLuint shader;
		shader = glCreateShader(shadertype);
		glShaderSource(shader, 1, &shaderChar, NULL);
		glCompileShader(shader);

		//Get errors
		shaderLog(shader, GL_COMPILE_STATUS);
		return shader;
	}

	void Shader::shaderLog(unsigned int shader, unsigned int type) const {
		int success = false;
		glGetShaderiv(shader, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength]();
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);

			oe_error_terminate("{}: shader compilation: \n{}", p_shader_name, infoLog);
			delete[] infoLog;
		}
	}

	void Shader::programLog(int program, unsigned int type) const {
		int success;
		glGetProgramiv(program, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength]();
			glGetProgramInfoLog(program, 512, nullptr, infoLog);

			oe_error_terminate("{}: program linking: \n{}", p_shader_name, infoLog);
			delete[] infoLog;
		}
	}




	Shader::Shader(std::string name) 
	: p_shader_name(name), p_shader_program(0) {}

	void Shader::load(const std::string& compute_shader_source) {
		// Compute shader
		GLuint compute_shader = loadShader(compute_shader_source, GL_COMPUTE_SHADER);

		// Shader program
		p_shader_program = glCreateProgram();
		glAttachShader(p_shader_program, compute_shader);
		glLinkProgram(p_shader_program);

		//Get shader program linking error
		programLog(p_shader_program, GL_LINK_STATUS);

		//Free up data
		glDeleteShader(compute_shader);
	}

	
	void Shader::load(const std::string& vertex_shader_source, const std::string& fragment_shader_source) {

		// Vertex shader
		GLuint vertex_shader = loadShader(vertex_shader_source, GL_VERTEX_SHADER);

		// Fragment shader
		GLuint fragment_shader = loadShader(fragment_shader_source, GL_FRAGMENT_SHADER);

		// Shader program
		p_shader_program = glCreateProgram();
		glAttachShader(p_shader_program, vertex_shader);
		glAttachShader(p_shader_program, fragment_shader);
		glLinkProgram(p_shader_program);

		// Get shader program linking error
		programLog(p_shader_program, GL_LINK_STATUS);

		// Free up data
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}
	
	void Shader::load(const std::string& vertex_shader_source, const std::string& fragment_shader_source, const std::string& geometry_shader_source) {

		// Vertex shader
		GLuint vertex_shader = loadShader(vertex_shader_source, GL_VERTEX_SHADER);

		// Fragment shader
		GLuint fragment_shader = loadShader(fragment_shader_source, GL_FRAGMENT_SHADER);

		// Geometry shader
		GLuint geometry_shader = loadShader(geometry_shader_source, GL_GEOMETRY_SHADER);

		// Shader program
		p_shader_program = glCreateProgram();
		glAttachShader(p_shader_program, vertex_shader);
		glAttachShader(p_shader_program, fragment_shader);
		glAttachShader(p_shader_program, geometry_shader);
		glLinkProgram(p_shader_program);

		// Get shader program linking error
		programLog(p_shader_program, GL_LINK_STATUS);

		// Free up data
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}



	void Shader::bind() const { glUseProgram(p_shader_program); }
	void Shader::unbind() const { glUseProgram(0); }
	
	int Shader::getUniformLocation(std::string name) const { return glGetUniformLocation(p_shader_program, name.c_str()); }
	
	void Shader::setUniform1f(std::string name, float value) const { bind(); glUniform1f(getUniformLocation(name), value); }
	void Shader::setUniform2f(std::string name, glm::fvec2 &value) const { bind(); glUniform2f(getUniformLocation(name), value.x, value.y); }
	void Shader::setUniform3f(std::string name, glm::fvec3 &value) const { bind(); glUniform3f(getUniformLocation(name), value.x, value.y, value.z); }
	void Shader::setUniform4f(std::string name, glm::fvec4 &value) const { bind(); glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w); }
	void Shader::setUniform1i(std::string name, int value) const { bind(); glUniform1i(getUniformLocation(name), value); }
	void Shader::setUniform2i(std::string name, glm::ivec2 &value) const { bind(); glUniform2i(getUniformLocation(name), value.x, value.y); }
	void Shader::setUniform3i(std::string name, glm::ivec3 &value) const { bind(); glUniform3i(getUniformLocation(name), value.x, value.y, value.z); }
	void Shader::setUniform4i(std::string name, glm::ivec4 &value) const { bind(); glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w); }
	void Shader::setUniformMat4(std::string name, glm::mat4 &value) const { bind(); glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)); }
}
