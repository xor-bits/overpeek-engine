#pragma once
#include "engine/enum.hpp"

#include <cstdint>



namespace oe::graphics
{
	template<typename T>
	struct static_array // just a dumb pointer
	{
	private:
		T* m_ptr;
		int32_t m_size;

	public:
		static_array(T* ptr, size_t size) 
			: m_ptr(ptr)
			, m_size(static_cast<int32_t>(std::clamp<size_t>(size, 0, std::numeric_limits<int32_t>::max())))
		{}

		~static_array() {
			delete[] m_ptr;
		}

		const T* ptr() const { return m_ptr; }
		const int32_t& size() const { return m_size; }
	};

	template<oe::primitive_types type>
	struct BasicBufferGen {
	private:
		int32_t m_primitive_count;

	public:
		BasicBufferGen(int32_t primitive_count)
			: m_primitive_count(primitive_count)
		{}

		int32_t vertex_per_primitive() const
		{
			switch (type)
			{
			case oe::primitive_types::points:
				return 1;
			case oe::primitive_types::lines:
				return 2;
			case oe::primitive_types::triangles:
				return 3;
			case oe::primitive_types::quads:
				return 4;
			default:
				return 0;
			}
		}

		int32_t index_per_primitive() const
		{
			switch (type)
			{
			case oe::primitive_types::points:
				return 1;
			case oe::primitive_types::lines:
				return 2;
			case oe::primitive_types::triangles:
				return 3;
			case oe::primitive_types::quads:
				return 6;
			default:
				return 0;
			}
		}

		int32_t vertex_count() const
		{
			return m_primitive_count * vertex_per_primitive();
		}

		int32_t index_count() const
		{
			return m_primitive_count * index_per_primitive();
		}

		oe::primitive_types render_primitive() const
		{
			switch (type)
			{
			case oe::primitive_types::points:
				return oe::primitive_types::points;
			case oe::primitive_types::lines:
				return oe::primitive_types::lines;
			case oe::primitive_types::triangles:
				return oe::primitive_types::triangles;
			case oe::primitive_types::quads:
				return oe::primitive_types::triangles;
			default:
				return oe::primitive_types::triangles;
			}
		}

		static uint16_t* gen_simple(int32_t primitive_count)
		{
			uint16_t* index_array = new uint16_t[primitive_count];
			for (int i = 0; i < primitive_count; i++) {
				index_array[i] = i;
			}
			return index_array;
		}

		static_array<uint16_t> gen_points() const
		{
			return { gen_simple(m_primitive_count), sizeof(uint16_t) * m_primitive_count };
		}

		static_array<uint16_t> gen_lines() const
		{
			return { gen_simple(m_primitive_count * 2), sizeof(uint16_t) * m_primitive_count * 2 };
		}

		static_array<uint16_t> gen_triangles() const
		{
			return { gen_simple(m_primitive_count * 3), sizeof(uint16_t) * m_primitive_count * 3 };
		}

		static_array<uint16_t> gen_quads() const
		{
			uint16_t* index_array = new uint16_t[m_primitive_count * 6];
			int index_counter = 0;
			for (int i = 0; i < m_primitive_count; i++) {
				index_array[i * 6 + 0] = index_counter + 0;
				index_array[i * 6 + 1] = index_counter + 1;
				index_array[i * 6 + 2] = index_counter + 2;
				index_array[i * 6 + 3] = index_counter + 0;
				index_array[i * 6 + 4] = index_counter + 2;
				index_array[i * 6 + 5] = index_counter + 3;
				index_counter += 4;
			}
			return { index_array, sizeof(uint16_t) * m_primitive_count * 6 };
		}

		template<typename T>
		static_array<float> optional_vertex_gen(const void* copied = nullptr) const
		{
			float* vb;
			int32_t size = std::abs(vertex_count() * (int32_t)T::component_count);
			
			if (copied == nullptr) {
				vb = nullptr;
			}
			else {
				vb = new float[size];
				memcpy(vb, copied, size);
			}
			return { vb, sizeof(float) * size };
		}

		static_array<uint16_t> gen() const
		{
			switch (type)
			{
			case oe::primitive_types::points:
				return gen_points();
			case oe::primitive_types::lines:
				return gen_lines();
			case oe::primitive_types::triangles:
				return gen_triangles();
			case oe::primitive_types::quads:
				return gen_quads();
			default:
				return { nullptr, 0 };
			}
		}

	};

}