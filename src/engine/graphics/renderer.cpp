#include "renderer.hpp"

#include "engine/graphics/interface/texture.hpp"



namespace oe::graphics
{
	Renderer::Renderer(int32_t max_primitives)
		: m_primitive_renderer({ max_primitives, oe::types::dynamic_type, oe::types::static_type, nullptr })
	{
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::attemptMap()
	{
		if(!m_mapped)
			m_primitive_renderer->begin();

		m_mapped = true;
	}

	void Renderer::update_quads(bool sorted)
	{
		attemptMap();
		auto* vertices = m_primitive_renderer->modifyVertex(m_primitive_renderer->vertexCount(), 0);

		int32_t total_primitive_count = 0;
		for(int32_t i = 0; i < m_quads.size(); i++)
		{
			auto& quad = m_quads.at(i);
			if(quad->getRemoved())
				if(sorted)
					break;
				else
					continue;
			
			total_primitive_count += 1;
			bool got_moved = quad->getQuadIndex() != total_primitive_count;
			quad->setQuadIndex(total_primitive_count);

			if(got_moved)
				quad->updateForce(vertices + quad->getQuadIndex() * 4);
		}
		m_primitive_renderer->vertexCount() = total_primitive_count * 4;
	}

	std::unique_ptr<Quad> Renderer::create()
	{
		auto quad_creator = [this](int32_t index){
			auto quad = std::make_unique<Quad>(*this);
			m_quads.push_back(quad.get());
			quad->setQuadIndex(index);
			return quad;
		};

		if(m_empty_spaces.empty())
		{
			auto insert_at = m_primitive_renderer->vertexCount();
			m_primitive_renderer->vertexCount() += 4;
			return quad_creator(insert_at / 4);
		}
		
		auto insert_at = m_empty_spaces.back();
		m_empty_spaces.pop_back();
		return quad_creator(insert_at);
	}

	void Renderer::remove(const std::unique_ptr<Quad>& quad)
	{
		remove(quad.get());
	}

	void Renderer::remove(const Quad* quad)
	{
		if(m_quads.size() == 0)
			return;
		remove(std::find(m_quads.begin(), m_quads.end(), quad));
	}

	void Renderer::remove(iter_t iter)
	{
		if(iter == m_quads.end())
			return;
			
		auto quad = *iter;
		fill_zeroes(quad);
		m_quads.erase(iter);
	}
	
	void Renderer::fill_zeroes(const Quad* quad)
	{
		if(quad->getQuadIndex() * 4 + 4 == m_primitive_renderer->vertexCount()) // pop quad from vbo stack
		{
			m_primitive_renderer->vertexCount() -= 4;
		}
		else // fill vbo with zeros at the correct pos
		{
			attemptMap();
			quad->gen_vertices_zero(m_primitive_renderer->modifyVertex(4, quad->getQuadIndex() * 4));
			m_empty_spaces.push_back(quad->getQuadIndex());
		}
	}

	void Renderer::forget(std::unique_ptr<Quad>&& quad)
	{
		m_forgotten_quads.push_back(std::move(quad));
	}

	void Renderer::sort()
	{
		/* auto debug_lambda = [this](){
			spdlog::debug("vertcount:{}", m_primitive_renderer->vertexCount());
			for(size_t i = 0; i < m_quads.size(); i++)
				spdlog::debug("quad:{}, idx:{}, z:{}, tex:{}, ?:{}", 
				i,
				m_quads[i]->getQuadIndex(),
				m_quads[i]->getPosition().z,
				(size_t)m_quads[i]->getSprite()->m_owner.get(),
				m_quads[i]->getRemoved());
		};

		for(size_t i = 0; i < m_quads.size(); i++)
			if(rand() % 100 >= 50)
				m_quads[i]->setRemoved(true);

		debug_lambda();
		__debugbreak(); */

		sort(sort_lz_t, true);
		
		/* debug_lambda();
		__debugbreak(); */
	}

	void Renderer::clear()
	{
		std::for_each(m_quads.begin(), m_quads.end(), [this](Quad* quad){
			quad->setRemoved(true);
		});

		m_quads.clear();
		m_forgotten_quads.clear();

		m_primitive_renderer->clear();
	}

	void Renderer::render_c() const
	{
		if(m_quads.size() == 0)
			return;

		std::vector<renderpass> renderpasses;
		ITexture* latest_texture_ptr = nullptr;
		renderpasses.reserve(m_quads.size());

		for(size_t i = 0; i < m_quads.size(); i++)
		{
			const auto& quad = m_quads.at(i);

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
		std::get<1>(renderpasses.back()) = m_quads.back()->getQuadIndex() - std::get<0>(renderpasses.back()) + 1;

		// spdlog::debug("Quads: {}, Vertices: {}, Renderpasses: {}", m_quads.size(), m_primitive_renderer->vertexCount(), renderpasses.size());

		std::for_each(renderpasses.begin(), renderpasses.end(), [this](const renderpass& pass){
			// spdlog::debug(" - rp {},{}", std::get<0>(pass), std::get<1>(pass));
			std::get<2>(pass)->bind();
			m_primitive_renderer->render(std::get<0>(pass), std::get<1>(pass));
		});
	}

	void Renderer::render()
	{
		if(m_mapped)
			m_primitive_renderer->end();
		m_mapped = false;

		render_c();
	}
}