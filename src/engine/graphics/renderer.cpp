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

	void Renderer::update_quads()
	{
		attemptMap();
		auto* vertices = m_primitive_renderer->modifyVertex(m_primitive_renderer->vertexCount(), 0);

		int32_t i = 0;
		for(; i < m_quads.size(); i++)
		{
			auto& quad = m_quads.at(i);
			if(quad->getRemoved())
			{
				if(quad->getQuadIndex() == m_primitive_renderer->vertexCount() - 4)


				--i;
				break;
			}
			
			bool got_moved = quad->getQuadIndex() != i * 4;
			quad->setQuadIndex(i * 4);

			if(got_moved)
			{
				quad->update(vertices);
			}
		}
		m_primitive_renderer->vertexCount() = i * 4;
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
			return quad_creator(insert_at);
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
		if(quad->getQuadIndex() == m_primitive_renderer->vertexCount() - 4) // pop quad from vbo stack
			m_primitive_renderer->vertexCount() -= 4;
		else // fill vbo with zeros at the correct pos
		{
			attemptMap();
			quad->gen_vertices_zero(m_primitive_renderer->modifyVertex(4, quad->getQuadIndex()));
			m_empty_spaces.push_back(quad->getQuadIndex());
		}
	}

	void Renderer::forget(std::unique_ptr<Quad>&& quad)
	{
		m_forgotten_quads.push_back(std::move(quad));
	}

	void Renderer::sort()
	{
		sort(sort_by_largest_z_and_texture);
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
		std::vector<renderpass> renderpasses;
		ITexture* latest_texture_ptr = nullptr;
		for(size_t i = 0; i < m_quads.size(); i++)
		{
			if(!m_quads.at(i))
			{
				std::get<1>(renderpasses.back())++;
				continue;
			}

			auto this_texture_ptr = m_quads.at(i)->getSprite()->m_owner.get();
			if(latest_texture_ptr == this_texture_ptr)
			{
				std::get<1>(renderpasses.back())++;
				continue;
			}

			renderpasses.push_back({ i, 1 });
			latest_texture_ptr = this_texture_ptr;
		}

		// spdlog::debug("Quads: {}, Vertices: {}, Renderpasses: {}", m_quads.size(), m_primitive_renderer->vertexCount(), renderpasses.size());

		std::for_each(renderpasses.begin(), renderpasses.end(), [this](const renderpass& pass){
			auto& quad = m_quads.at(std::get<0>(pass));
			quad->getSprite()->m_owner->bind();
			m_primitive_renderer->render(quad->getQuadIndex() / 4, std::get<1>(pass));
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