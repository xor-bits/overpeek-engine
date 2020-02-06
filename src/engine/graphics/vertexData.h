#pragma once

#include "engine/internal_libs.h"

#include "opengl/buffers/vertexBuffer.h"



namespace oe::graphics {

	struct VertexData_internal {
		glm::fvec3 position;
		glm::fvec2 uv;
		glm::fvec4 color;

		VertexData_internal()
			: position(0.0f), uv(0.0f), color(0.0f)
		{}

		VertexData_internal(glm::fvec3 _position, glm::fvec2 _uv, glm::fvec4 _color)
			: position(_position), uv(_uv), color(_color)
		{}

		VertexData_internal(glm::fvec2 _position, glm::fvec2 _uv, glm::fvec4 _color)
			: position(_position, 0.0f), uv(_uv), color(_color)
		{}
	};

	struct VertexData : public VertexData_internal {
		static constexpr size_t component_count = sizeof(VertexData_internal) / sizeof(float);
		static constexpr size_t pos_offset = offsetof(VertexData_internal, position);
		static constexpr size_t uv_offset = offsetof(VertexData_internal, uv);
		static constexpr size_t col_offset = offsetof(VertexData_internal, color);

		VertexData()
			: VertexData_internal()
		{}

		VertexData(glm::fvec3 position, glm::fvec2 uv, glm::fvec4 color)
			: VertexData_internal(position, uv, color)
		{}

		VertexData(glm::fvec2 position, glm::fvec2 uv, glm::fvec4 color)
			: VertexData_internal(position, uv, color)
		{}

		static void configVBO(VertexBuffer *vbo) {
			vbo->attrib(0, 3, pos_offset);
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