#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace oe {

	class Shader {
	private:
		unsigned int mShaderProgram;

		unsigned int loadShader(unsigned int shadertype, const char* shaderText);
		unsigned int loadShaderFile(unsigned int shadertype, const char *path);
		void shaderLog(const char* text, unsigned int shader, unsigned int type);
		void programLog(const char* text, unsigned int program, unsigned int type);
	public:
		Shader();
		Shader(const char *vertexPath, const char *fragmentPath);
		Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath);
		
		void bind();
		void unbind();

		int getUniformLocation(const char *name);
		void setUniform1f(const char *name, float value);
		void setUniform2f(const char *name, glm::fvec2 &value);
		void setUniform3f(const char *name, glm::fvec3 &value);
		void setUniform4f(const char *name, glm::fvec4 &value);
		void setUniform1i(const char *name, int value);
		void setUniform2i(const char *name, glm::ivec2 &value);
		void setUniform3i(const char *name, glm::ivec3 &value);
		void setUniform4i(const char *name, glm::ivec4 &value);
		void SetUniformMat4(const char *name, glm::mat4 &value);
	};

}