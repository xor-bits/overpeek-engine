#pragma once

#include "engine/internal_libs.h"

#include "opengl/buffers/vertexBuffer.h"



namespace oe::graphics {

	struct VertexData {
		static const int component_count = 10;

		glm::fvec2 position;
		glm::fvec2 uv;
		glm::fvec4 color;

		VertexData()
			: position(0.0f), uv(0.0f), color(0.0f)
		{}

		VertexData(glm::fvec2 _position, glm::fvec2 _uv, glm::fvec4 _color)
			: position(_position), uv(_uv), color(_color)
		{}

		static void configVBO(VertexBuffer *vbo) {
			vbo->attrib(0, 2, offsetof(VertexData, position));
			vbo->attrib(1, 2, offsetof(VertexData, uv));
			vbo->attrib(2, 4, offsetof(VertexData, color));
		}
	};

}