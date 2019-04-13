#pragma once

#include <glm/glm.hpp>

namespace oe {

	struct VertexData
	{
		glm::fvec3 position;
		glm::fvec2 size_or_uv;
		float texture;
		glm::fvec4 color;

		VertexData(glm::fvec3 _position, glm::fvec2 _size_or_uv, int _texture, glm::fvec4 _color)
			: position(_position), size_or_uv(_size_or_uv), texture(_texture), color(_color) {}
	};

}