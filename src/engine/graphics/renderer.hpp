#pragma once

#include "engine/interfacegen_renderer.hpp"
#include "interface/primitive_renderer.hpp"
#include "primitives.hpp"

#include <set>
#include <unordered_set>



namespace oe::graphics
{
	class Renderer
	{
	public:
		using iter_t = std::vector<Quad*>::iterator;
		using citer_t = std::vector<Quad*>::const_iterator;
		using renderpass = std::tuple<int32_t, int32_t, ITexture*>; // first = first primitive, second = primitive count, third = texture
		
		// sort by largest z if not removed and by removed otherwise
		static constexpr auto sort_lz = [](const Quad* l, const Quad* r)->bool
		{
			bool removed = (l->getRemoved() || r->getRemoved());
			return
				removed  && (l->getRemoved() < r->getRemoved()) ||
				!removed && (l->getPosition().z < r->getPosition().z);
		};
		
		// sort by smallest z if not removed and by removed otherwise
		static constexpr auto sort_sz = [](const Quad* l, const Quad* r)->bool
		{
			bool removed = (l->getRemoved() || r->getRemoved());
			return
				removed  && (l->getRemoved() < r->getRemoved()) ||
				!removed && (l->getPosition().z > r->getPosition().z);
		};
		
		// sort by texture id if not removed and by removed otherwise
		static constexpr auto sort_t = [](const Quad* l, const Quad* r)->bool
		{
			bool removed = (l->getRemoved() || r->getRemoved());
			return
				removed  && (l->getRemoved() < r->getRemoved()) ||
				!removed && (l->getSprite()->m_owner.get() < l->getSprite()->m_owner.get());
		};
		
		// sort by largest z if opaque and not removed and by texture if not removed and by removed otherwise
		static constexpr auto sort_lz_t = [](const Quad* l, const Quad* r)->bool
		{
			bool opaque = (l->getOpacityMode() || r->getOpacityMode());
			return
				opaque && sort_lz(l, r) ||
				opaque && sort_t(l, r);
		};
		
		// sort by smallest z if opaque and not removed and by texture if not removed and by removed otherwise
		static constexpr auto sort_sz_t = [](const Quad* l, const Quad* r)->bool
		{
			bool opaque = (l->getOpacityMode() || r->getOpacityMode());
			return
				opaque && sort_sz(l, r) ||
				opaque && sort_t(l, r);
		};

	private:
		bool m_mapped = false;
		oe::graphics::PrimitiveRenderer m_primitive_renderer;
		std::vector<int32_t> m_empty_spaces;
		std::vector<Quad*> m_quads;
		std::vector<std::unique_ptr<Quad>> m_forgotten_quads;

	public:
		void attemptMap();
		void render_c() const;
		void update_quads(bool sorted);
		void fill_zeroes(const Quad *);
	
	public:
		Renderer(int32_t max_primitives = 100000);
		~Renderer();

		// create and submit new quad that can be modified easily
		std::unique_ptr<Quad> create();

		// removes a single quad from the renderer
		void remove(const std::unique_ptr<Quad>&);

		// removes a single quad from the renderer
		void remove(const Quad *);

		// removes a single quad from the renderer
		void remove(iter_t iter);

		// takes ownership of the Quad ptr
		void forget(std::unique_ptr<Quad>&&);

		// Will sort all quads in vbo. Default behaviour: largest z first. Useful when dealing with transparencies
		template<typename Callable>
		void sort(const Callable& comparator, bool removed_are_sorted)
		{
			std::sort(m_quads.begin(), m_quads.end(), comparator);
			update_quads(removed_are_sorted);
		}
		
		// Will sort all quads in vbo. Default behaviour: largest z first. Useful when dealing with transparencies
		void sort();

		// removes all quads (including forgotten ones)
		void clear();

		// render all quads sprites will automatically be bound
		void render();

	public:
		oe::graphics::PrimitiveRenderer& getPrimitiveRenderer() { return m_primitive_renderer; }
	};
	
}