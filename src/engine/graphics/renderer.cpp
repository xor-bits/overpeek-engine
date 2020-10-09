#include "renderer.hpp"

#include "engine/graphics/interface/texture.hpp"



namespace oe::graphics
{
	Renderer::Renderer(int32_t max_primitives, const comparator_t& comparator)
		: m_primitive_renderer({ max_primitives, oe::types::dynamic_type, oe::types::static_type, nullptr })
		, m_quads()
		, m_comparator(comparator)
	{}

	Renderer::~Renderer()
	{

	}

	void Renderer::attemptMap()
	{
		if(!m_mapped)
			m_primitive_renderer->begin();

		m_mapped = true;
	}

	std::unique_ptr<Quad> Renderer::create()
	{
		m_primitive_renderer->vertexCount() += 4;

		auto quad = std::make_unique<Quad>(*this);
		quad->setQuadIndex(-1);
		m_quads.push_back(quad.get());
		return quad;
	}

	void Renderer::remove(const std::unique_ptr<Quad>& quad)
	{
		remove(quad.get());
	}

	void Renderer::remove(Quad* quad)
	{
		if(m_quads.size() == 0)
			return;

		// m_quads.find cant work here because it uses comparator and the quads can be out of order
		remove(std::find(m_quads.begin(), m_quads.end(), quad));
	}

	void Renderer::remove(iter_t iter)
	{
		if(iter == m_quads.end())
			return;

		m_primitive_renderer->vertexCount() -= 4;
		m_quads.erase(iter);
	}

	void Renderer::forget(std::unique_ptr<Quad>&& quad)
	{
		m_forgotten_quads.push_back(std::forward<std::unique_ptr<Quad>>(quad));
	}

	void Renderer::clear()
	{
		m_quads.clear();
		m_forgotten_quads.clear();

		m_primitive_renderer->clear();
	}

	constexpr bool debug_renderpasses = false;
	void Renderer::render()
	{
		if(m_quads.size() == 0)
			return;
			
		attemptMap();
		auto* vertices = m_primitive_renderer->modifyVertex(m_primitive_renderer->vertexCount(), 0);

		// generate vector of renderpasses
		std::vector<renderpass> renderpasses;
		ITexture* latest_texture_ptr = nullptr;
		renderpasses.reserve(m_quads.size());
		size_t index = 0;
		std::sort(m_quads.begin(), m_quads.end(), m_comparator);
		for(const auto& quad : m_quads)
		{
			// reorder any possibly moved elements
			quad->setQuadIndex(index++);
			quad->update(vertices);

			auto this_texture_ptr = quad->getSprite()->m_owner.get();
			if(latest_texture_ptr == this_texture_ptr)
				continue;
			else
			{
				if(renderpasses.size() != 0) std::get<1>(renderpasses.back()) = quad->getQuadIndex() - std::get<0>(renderpasses.back());
				renderpasses.push_back({ quad->getQuadIndex(), 1, this_texture_ptr });
			}

			latest_texture_ptr = this_texture_ptr;
		}
		if(m_quads.size() != 0) std::get<1>(renderpasses.back()) = (*m_quads.rbegin())->getQuadIndex() - std::get<0>(renderpasses.back()) + 1;
		if constexpr(debug_renderpasses) spdlog::debug("Quads: {}, Vertices: {}, Renderpasses: {}", m_quads.size(), m_primitive_renderer->vertexCount(), renderpasses.size());
		
		// unmap buffer
		if(m_mapped)
			m_primitive_renderer->end();
		m_mapped = false;

		// render using renderpasses
		std::for_each(renderpasses.cbegin(), renderpasses.cend(), [this](const renderpass& pass){
			if constexpr(debug_renderpasses) spdlog::debug(" - rp {},{},{:x}", std::get<0>(pass), std::get<1>(pass), (size_t)std::get<2>(pass));
			std::get<2>(pass)->bind();
			m_primitive_renderer->render(std::get<0>(pass), std::get<1>(pass));
		});
	}
}