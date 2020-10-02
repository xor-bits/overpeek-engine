#pragma once

#include "engine/interfacegen_renderer.hpp"
#include "interface/primitive_renderer.hpp"
#include "primitives.hpp"

#include <set>
#include <unordered_set>
#include <functional>



namespace oe::graphics
{
	// sort by largest z
	static constexpr auto sort_lz = [](const Quad* l, const Quad* r)->bool
	{
		return (l->getPosition().z < r->getPosition().z);
	};
	
	// sort by smallest z
	static constexpr auto sort_sz = [](const Quad* l, const Quad* r)->bool
	{
		return (l->getPosition().z > r->getPosition().z);
	};
	
	// sort by texture id
	static constexpr auto sort_t = [](const Quad* l, const Quad* r)->bool
	{
		return (l->getSprite()->m_owner.get() < l->getSprite()->m_owner.get());
	};
	
	// sort by largest z if opaque and by texture otherwise
	static constexpr auto sort_lz_t = [](const Quad* l, const Quad* r)->bool
	{
		const bool opaque = (l->getOpacityMode() || r->getOpacityMode());
		const bool opaque_diff = (l->getOpacityMode() != r->getOpacityMode());
		return
			opaque  && !opaque_diff && sort_lz(l, r) ||
			!opaque && !opaque_diff && sort_t(l, r) ||
			opaque_diff && (l->getOpacityMode() < r->getOpacityMode());
	};
	
	// sort by smallest z if opaque and by texture otherwise
	static constexpr auto sort_sz_t = [](const Quad* l, const Quad* r)->bool
	{
		const bool opaque = (l->getOpacityMode() || r->getOpacityMode());
		const bool opaque_diff = (l->getOpacityMode() != r->getOpacityMode());
		return
			opaque  && sort_sz(l, r) ||
			!opaque && sort_t(l, r) ||
			opaque_diff && (l->getOpacityMode() < r->getOpacityMode());
	};
	
	class Renderer
	{
	public:
		using comparator_t = std::function<bool(const Quad*,const Quad*)>;
		using quads_t = std::vector<Quad*>;
		using iter_t = quads_t::iterator;
		using citer_t = quads_t::const_iterator;
		using renderpass = std::tuple<int32_t, int32_t, ITexture*>; // first = first primitive, second = primitive count, third = texture

	private:
		bool m_mapped = false;
		oe::graphics::PrimitiveRenderer m_primitive_renderer;
		quads_t m_quads;
		std::vector<std::unique_ptr<Quad>> m_forgotten_quads;
		comparator_t m_comparator;

	public:
		void attemptMap();
	
	public:
		Renderer(int32_t max_primitives = 100000, const comparator_t& comparator = sort_lz_t);
		~Renderer();

		// create and submit new quad that can be modified easily
		std::unique_ptr<Quad> create();

		// removes a single quad from the renderer
		void remove(const std::unique_ptr<Quad>&);

		// removes a single quad from the renderer
		void remove(Quad *);

		// removes a single quad from the renderer
		void remove(iter_t iter);

		// takes ownership of the Quad ptr
		void forget(std::unique_ptr<Quad>&&);

		// removes all quads (including forgotten ones)
		void clear();

		// render all quads sprites will automatically be bound
		void render();

	public:
		oe::graphics::PrimitiveRenderer& getPrimitiveRenderer() { return m_primitive_renderer; }
	};
	
}