#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {

	class Shader {
	private:
		unsigned int mShaderProgram;
	public:
		Shader(const char *vertexPath, const char *fragmentPath);
		
		inline void enable() { glUseProgram(mShaderProgram); }
		inline void disable() { glUseProgram(0); }

		inline GLint getUniformLocation(const char *name) { return glGetUniformLocation(mShaderProgram, name); }
		
		inline void setUniform1f(const char *name, float value) { glUniform1f(getUniformLocation(name), value); }
		inline void setUniform2f(const char *name, glm::fvec2 value) { glUniform2f(getUniformLocation(name), value.x, value.y); }
		inline void setUniform3f(const char *name, glm::fvec3 value) { glUniform3f(getUniformLocation(name), value.x, value.y, value.z); }
		inline void setUniform4f(const char *name, glm::fvec4 value) { glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w); }

		inline void setUniform1i(const char *name, int value) { glUniform1i(getUniformLocation(name), value); }
		inline void setUniform2i(const char *name, glm::ivec2 value) { glUniform2i(getUniformLocation(name), value.x, value.y); }
		inline void setUniform3i(const char *name, glm::ivec3 value) { glUniform3i(getUniformLocation(name), value.x, value.y, value.z); }
		inline void setUniform4i(const char *name, glm::ivec4 value) { glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w); }

		inline void setUniform1d(const char *name, double value) { glUniform1d(getUniformLocation(name), value); }
		inline void setUniform2d(const char *name, glm::dvec2 value) { glUniform2d(getUniformLocation(name), value.x, value.y); }
		inline void setUniform3d(const char *name, glm::dvec3 value) { glUniform3d(getUniformLocation(name), value.x, value.y, value.z); }
		inline void setUniform4d(const char *name, glm::dvec4 value) { glUniform4d(getUniformLocation(name), value.x, value.y, value.z, value.w); }

		inline void SetUniformMat4(const char *name, glm::mat4 value) { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)); }
	};

}