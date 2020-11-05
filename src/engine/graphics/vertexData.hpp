#pragma once

#include "engine/internal_libs.hpp"



namespace oe::graphics
{
	struct VertexData
	{
		struct VertexData_Internal
		{
			glm::fvec3 position;
			glm::fvec2 uv;
			glm::fvec4 color;
		} data;


		constexpr static inline size_t component_count = sizeof(VertexData_Internal) / sizeof(float);
		constexpr static inline size_t pos_offset = offsetof(VertexData_Internal, position);
		constexpr static inline size_t uv_offset = offsetof(VertexData_Internal, uv);
		constexpr static inline size_t col_offset = offsetof(VertexData_Internal, color);


		constexpr VertexData(const glm::fvec3& position = { 0.0f, 0.0f, 0.0f }, const glm::fvec2& uv = { 0.0f, 0.0f }, const glm::fvec4& color = { 0.0f, 0.0f, 0.0f, 0.0f }) noexcept
			: data()
		{
			data.position = position;
			data.uv = uv;
			data.color = color;
		}

		constexpr VertexData(const glm::fvec2& position, const glm::fvec2& uv, const glm::fvec4& color) noexcept
			: VertexData(glm::fvec3{ position, 0.0f }, uv, color)
		{}

		static void config(const std::function<void(int, int, size_t)>& attrib_fn)
		{
			attrib_fn(0, 3, pos_offset);
			attrib_fn(1, 2, uv_offset);
			attrib_fn(2, 4, col_offset);
		}
	};
}

template <>
struct fmt::formatter<oe::graphics::VertexData> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const oe::graphics::VertexData& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {}, {} ]", d.data.position, d.data.uv, d.data.color);
	}
};