#include "shader.h"

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"
#include "../utility/logger.h"
#include "../utility/filereader.h"



namespace oe {

	unsigned int Shader::loadShader(unsigned int shadertype, const char* shaderText) {
		GLuint shader;
		shader = glCreateShader(shadertype);
		glShaderSource(shader, 1, &shaderText, NULL);
		glCompileShader(shader);

		//Get errors
		shaderLog("Shader compilation failed!", shader, GL_COMPILE_STATUS);
		return shader;
	}

	unsigned int Shader::loadShaderFile(unsigned int shadertype, const char *path) {
		//Load and compile
		std::string shaderStr = oe::readFile(path);
		const char *shaderChar = shaderStr.c_str();
		
		return loadShader(shadertype, shaderChar);
	}

	void Shader::shaderLog(const char* text, unsigned int shader, unsigned int type) {
		int success = false;
		char infoLog[512];
		glGetShaderiv(shader, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);

			oe::Logger::out(text, oe::critical);
			std::cout << infoLog << std::endl;
			delete[] infoLog;
			Window::terminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	void Shader::programLog(const char* text, unsigned int program, unsigned int type) {
		int success;
		char infoLog[512];
		glGetProgramiv(program, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(program, 512, nullptr, infoLog);

			oe::Logger::out(text, oe::critical);
			std::cout << infoLog << std::endl;
			delete[] infoLog;
			Window::terminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	Shader::Shader() {

		//Vertex shader
		GLuint vertexShader = loadShader(GL_VERTEX_SHADER, 
			"#version 330 core\n"
			"layout(location = 0) in vec3 vertex_pos;\n"
			"layout(location = 1) in vec2 texture_uv;\n"
			"layout(location = 2) in float texture_id;\n"
			"layout(location = 3) in vec4 vertex_color;\n"

			"out vec2 shader_uv;\n"
			"out vec4 shader_color;\n"
			"flat out int shader_id;\n"
			"\n"
			"uniform mat4 pr_matrix = mat4(1.0);\n"
			"uniform mat4 ml_matrix = mat4(1.0);\n"
			"uniform mat4 vw_matrix = mat4(1.0);\n"
			"\n"
			"void main()\n"
			"{\n"
			"   mat4 mvp = pr_matrix * vw_matrix * ml_matrix;\n"
			"	gl_Position = mvp * vec4(vertex_pos.x, vertex_pos.y, vertex_pos.z, 1.0f);\n"
			"	shader_uv = texture_uv;\n"
			"	shader_id = int(floor(texture_id));\n"
			"	shader_color = vertex_color;\n"
			"}\n"
		);

		//Fragment shader
		GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER,
			"#version 330 core\n"
			"\n"
			"in vec2 shader_uv;\n"
			"in vec4 shader_color;\n"
			"flat in int shader_id;\n"
			"\n"
			"layout(location = 0) out vec4 color;\n"
			"\n"
			"uniform sampler2DArray tex;\n"
			"uniform int textured = 0;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	if (textured != 0) color = texture(tex, vec3(shader_uv, shader_id)) * shader_color;\n"
			"	else color = shader_color;\n"
			"}\n"
		);

		//Shader program
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, vertexShader);
		glAttachShader(mShaderProgram, fragmentShader);
		glLinkProgram(mShaderProgram);

		//Get shader program linking error
		programLog("Shader program linking failed!", mShaderProgram, GL_LINK_STATUS);

		//Default projection matrix
		glm::mat4 pr = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f);
		enable();
		SetUniformMat4("pr_matrix", pr);

		//Free up data
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::Shader(const char *vertexPath, const char *fragmentPath) {

		//Vertex shader
		GLuint vertexShader = loadShaderFile(GL_VERTEX_SHADER, vertexPath);

		//Fragment shader
		GLuint fragmentShader = loadShaderFile(GL_FRAGMENT_SHADER, fragmentPath);


		//Shader program
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, vertexShader);
		glAttachShader(mShaderProgram, fragmentShader);
		glLinkProgram(mShaderProgram);


		//Get shader program linking error
		programLog("Shader program linking failed!", mShaderProgram, GL_LINK_STATUS);

		//Free up data
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {

		//Vertex shader
		GLuint vertexShader = loadShaderFile(GL_VERTEX_SHADER, vertexPath);

		//Fragment shader
		GLuint fragmentShader = loadShaderFile(GL_FRAGMENT_SHADER, fragmentPath);

		//Geometry shader
		GLuint geometryShader = loadShaderFile(GL_GEOMETRY_SHADER, geometryPath);

		//Shader program
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, vertexShader);
		glAttachShader(mShaderProgram, fragmentShader);
		glAttachShader(mShaderProgram, geometryShader);
		glLinkProgram(mShaderProgram);

		//Get shader program linking error
		programLog("Shader program linking failed!", mShaderProgram, GL_LINK_STATUS);

		//Free up data
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);
	}


	void Shader::enable() { glUseProgram(mShaderProgram); }
	void Shader::disable() { glUseProgram(0); }
	
	int Shader::getUniformLocation(const char *name) { return glGetUniformLocation(mShaderProgram, name); }
	
	void Shader::setUniform1f(const char *name, float value) { enable(); glUniform1f(getUniformLocation(name), value); }
	void Shader::setUniform2f(const char *name, glm::fvec2 &value) { enable(); glUniform2f(getUniformLocation(name), value.x, value.y); }
	void Shader::setUniform3f(const char *name, glm::fvec3 &value) { enable(); glUniform3f(getUniformLocation(name), value.x, value.y, value.z); }
	void Shader::setUniform4f(const char *name, glm::fvec4 &value) { enable(); glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w); }
	void Shader::setUniform1i(const char *name, int value) { enable(); glUniform1i(getUniformLocation(name), value); }
	void Shader::setUniform2i(const char *name, glm::ivec2 &value) { enable(); glUniform2i(getUniformLocation(name), value.x, value.y); }
	void Shader::setUniform3i(const char *name, glm::ivec3 &value) { enable(); glUniform3i(getUniformLocation(name), value.x, value.y, value.z); }
	void Shader::setUniform4i(const char *name, glm::ivec4 &value) { enable(); glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w); }
	void Shader::setUniform1d(const char *name, double value) { enable(); glUniform1d(getUniformLocation(name), value); }
	void Shader::setUniform2d(const char *name, glm::dvec2 &value) { enable(); glUniform2d(getUniformLocation(name), value.x, value.y); }
	void Shader::setUniform3d(const char *name, glm::dvec3 &value) { enable(); glUniform3d(getUniformLocation(name), value.x, value.y, value.z); }
	void Shader::setUniform4d(const char *name, glm::dvec4 &value) { enable(); glUniform4d(getUniformLocation(name), value.x, value.y, value.z, value.w); }
	void Shader::SetUniformMat4(const char *name, glm::mat4 &value) { enable(); glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)); }
}