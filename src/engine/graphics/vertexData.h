#pragma once

#include "engine/internal_libs.h"

#include "opengl/buffers/vertexBuffer.h"



namespace oe::graphics {

	struct VertexData {
		static const int component_count = 8;

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
			constexpr size_t pos_offset = offsetof(VertexData, position);
			constexpr size_t uv_offset = offsetof(VertexData, uv);
			constexpr size_t col_offset = offsetof(VertexData, color);
			vbo->attrib(0, 2, pos_offset);
			vbo->attrib(1, 2, uv_offset);
			vbo->attrib(2, 4, col_offset);
		}
	};

}

template <>
struct fmt::formatter<oe::graphics::VertexData> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const oe::graphics::VertexData& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {}, {} ]", d.position, d.uv, d.color);
	}
};