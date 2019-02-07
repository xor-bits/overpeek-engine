#include "shader.h"
#include "window.h"
#include "../tools/filereader.h"
#include "../tools/logger.h"

namespace graphics {

	GLuint Shader::loadShader(GLenum shadertype, const char *path) {
		//Load and compile
		tools::Logger::info("Reading shader...");
		std::string shaderStr = tools::readFile(path);
		const char *shaderChar = shaderStr.c_str();
		//std::cout << shaderChar << std::endl;

		tools::Logger::info("Compiling shader...");
		GLuint shader;
		shader = glCreateShader(shadertype);
		glShaderSource(shader, 1, &shaderChar, NULL);
		glCompileShader(shader);

		//Get errors
		tools::Logger::info("Checking shader compilation errors...");
		shaderLog("Shader compilation failed!", shader, GL_COMPILE_STATUS);
		tools::Logger::info("Shader compiled");
		return shader;
	}

	void Shader::shaderLog(const char* text, GLuint shader, GLenum type) {
		int success = false;
		char infoLog[512];
		glGetShaderiv(shader, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);

			tools::Logger::critical(text);
			std::cout << infoLog << std::endl;
			delete infoLog;
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	void Shader::programLog(const char* text, GLuint program, GLenum type) {
		int success;
		char infoLog[512];
		glGetProgramiv(program, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(program, 512, nullptr, infoLog);

			tools::Logger::critical(text);
			std::cout << infoLog << std::endl;
			delete infoLog;
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	Shader::Shader(const char *vertexPath, const char *fragmentPath) {

		//Vertex shader
		GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);

		//Fragment shader
		GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

		//Shader program
		tools::Logger::info("Linking shaders");
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, vertexShader);
		glAttachShader(mShaderProgram, fragmentShader);
		glLinkProgram(mShaderProgram);

		//Get shader program linking error
		tools::Logger::info("Checking shader program linking errors...");
		programLog("Shader program linking failed!", mShaderProgram, GL_LINK_STATUS);

		//Free up data
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {

		//Vertex shader
		GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);

		//Fragment shader
		GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

		//Geometry shader
		GLuint geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryPath);

		//Shader program
		tools::Logger::info("Linking shaders");
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, vertexShader);
		glAttachShader(mShaderProgram, fragmentShader);
		glAttachShader(mShaderProgram, geometryShader);
		glLinkProgram(mShaderProgram);

		//Get shader program linking error
		tools::Logger::info("Checking shader program linking errors...");
		programLog("Shader program linking failed!", mShaderProgram, GL_LINK_STATUS);

		//Free up data
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);
	}

}