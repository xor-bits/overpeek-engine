#pragma once

#include <glm/glm.hpp>

namespace oe {

	struct VertexData
	{
		static const int component_count = 10;

		glm::fvec3 position;
		glm::fvec2 size_or_uv;
		float texture;
		glm::fvec4 color;

		VertexData(glm::fvec3 _position, glm::fvec2 _size_or_uv, int _texture, glm::fvec4 _color)
			: position(_position), size_or_uv(_size_or_uv), texture(_texture), color(_color)
		{}

		VertexData(glm::fvec2 _position, glm::fvec2 _size_or_uv, int _texture, glm::fvec4 _color)
			: position(_position, 0.0f), size_or_uv(_size_or_uv), texture(_texture), color(_color)
		{}

		static void configVBO(VertexBuffer &vbo) {
			vbo.attrib(0, 3, component_count, 0 * sizeof(float));
			vbo.attrib(1, 2, component_count, 3 * sizeof(float));
			vbo.attrib(2, 1, component_count, 5 * sizeof(float));
			vbo.attrib(3, 4, component_count, 6 * sizeof(float));
		}
	};

}