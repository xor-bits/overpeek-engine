#pragma once

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"

#include <string>



namespace oe::graphics {

	class IShader {
	public:
		IShader(const ShaderInfo& shader_info);
		virtual ~IShader();

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

	public:
		virtual int getUniformLocation(const std::string& name) = 0;

		// int and int array
		virtual void setUniform(const std::string& name, const float value) = 0;
		virtual void setUniform(const std::string& name, const glm::fvec2& value) = 0;
		virtual void setUniform(const std::string& name, const glm::fvec3& value) = 0;
		virtual void setUniform(const std::string& name, const glm::fvec4& value) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const float* values) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const glm::fvec2* values) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const glm::fvec3* values) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const glm::fvec4* values) = 0;

		// float and float array
		virtual void setUniform(const std::string& name, const int value) = 0;
		virtual void setUniform(const std::string& name, const glm::ivec2& value) = 0;
		virtual void setUniform(const std::string& name, const glm::ivec3& value) = 0;
		virtual void setUniform(const std::string& name, const glm::ivec4& value) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const int* values) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const glm::ivec2* values) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const glm::ivec3* values) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const glm::ivec4* values) = 0;

		// mat4 and mat4 array
		virtual void setUniform(const std::string& name, const glm::mat4& value) = 0;
		virtual void setUniform(const std::string& name, const size_t count, const glm::mat4* values) = 0;

		template<typename arr_type>
		void setUniform(const std::string& name, const arr_type& arr, typename std::enable_if<oe::is_container<arr_type>::value >::type* = 0)
		{
			if (arr.size() == 0) return;
			setUniform(name, arr.size(), arr.data());
		}
	};

}