#include "renderer.hpp"

#include "engine/utility/extra.hpp"
#include "engine/graphics/opengl/gl_primitive_renderer.hpp"
#include "engine/graphics/interface/window.hpp"


#include "engine/graphics/opengl/gl_texture.hpp"



namespace oe::graphics {
	
	void gen_points(const glm::vec2& position, const glm::vec2& size, const glm::vec2& align, float angle, glm::vec2& pointA, glm::vec2& pointB, glm::vec2& pointC, glm::vec2& pointD) {
		glm::vec2 alignment = alignmentOffset(size, align);
		pointA = size * glm::vec2(0.0f, 0.0f) - alignment;
		pointB = size * glm::vec2(0.0f, 1.0f) - alignment;
		pointC = size * glm::vec2(1.0f, 1.0f) - alignment;
		pointD = size * glm::vec2(1.0f, 0.0f) - alignment;

		if (angle != 0.0f) { // with rotation
			oe::utils::rotatePoint(angle, pointA);
			oe::utils::rotatePoint(angle, pointB);
			oe::utils::rotatePoint(angle, pointC);
			oe::utils::rotatePoint(angle, pointD);
		}

		pointA += position;
		pointB += position;
		pointC += position;
		pointD += position;
	}

	std::array<VertexData, 4> Quad::gen_vertices() const {
		glm::vec2 pointA, pointB, pointC, pointD;
		gen_points(m_position, m_size, m_rotation_alignment, m_rotation, pointA, pointB, pointC, pointD);

		glm::vec2 sprite_pos = m_sprite.position;
		glm::vec2 sprite_size = m_sprite.size;

		std::array<VertexData, 4> vertex_array = {
			VertexData(glm::vec3(pointA, m_position.z), sprite_pos + sprite_size * glm::vec2(0.0f, 0.0f), m_color),
			VertexData(glm::vec3(pointB, m_position.z), sprite_pos + sprite_size * glm::vec2(0.0f, 1.0f), m_color),
			VertexData(glm::vec3(pointC, m_position.z), sprite_pos + sprite_size * glm::vec2(1.0f, 1.0f), m_color),
			VertexData(glm::vec3(pointD, m_position.z), sprite_pos + sprite_size * glm::vec2(1.0f, 0.0f), m_color)
		};

		return vertex_array;
	}

	void Quad::update()
	{
		m_renderer->updateQuad(this);
	}



	Renderer::Renderer(const RendererInfo& renderer_info)
		: m_renderer_info(renderer_info)
	{
	}

	Renderer::~Renderer()
	{
		clear();
	}
	
	void SubRenderer::attempt_map()
	{
		if(m_buffer_bound) return;

		m_primitive_renderer->begin();
		m_buffer_bound = true;
	}
	
	void SubRenderer::attempt_unmap()
	{
		if(!m_buffer_bound) return;

		m_primitive_renderer->end();
		m_buffer_bound = false;
	}
	
	void SubRenderer::render()
	{
		attempt_unmap();
		m_primitive_renderer->render();
	}
	
	SubRenderer* Renderer::select_subrenderer(Quad* quad)
	{
		auto find_iter = m_renderers.find(quad->m_sprite.m_owner);
		if (find_iter == m_renderers.end())
		{
			SubRenderer* subrenderer = new SubRenderer();
			switch (oe::Engine::getSingleton().engine_info.api)
			{
			case oe::graphics_api::OpenGL:
				subrenderer->m_primitive_renderer = new GLPrimitiveRenderer(m_renderer_info);
				break;
			case oe::graphics_api::Vulkan:
				// TODO:
				subrenderer->m_primitive_renderer = new GLPrimitiveRenderer(m_renderer_info);
				break;
			}
			subrenderer->m_quad_index = 0;
			subrenderer->m_buffer_bound = false;
			subrenderer->m_texture = quad->m_sprite.m_owner;

			// create new subrenderer
			m_renderers.insert(std::make_pair(quad->m_sprite.m_owner, subrenderer));
			return subrenderer;
		}
		else
		{
			return find_iter->second;
		}
	}

	void SubRenderer::assign_new_quad(Quad* quad)
	{
		auto vertex_array = quad->gen_vertices();

		quad->m_assigned = true;
		quad->m_current_subrenderer = this;
		quad->m_quad_index = m_quad_index;
		
		// submit the quad to the selected subrenderer
		attempt_map();
		m_primitive_renderer->submitVertex(vertex_array.data(), vertex_array.size(), m_quad_index * 4);
		m_primitive_renderer->vertexCount() += 4;
		m_quad_index++;
	}

	void SubRenderer::reassign_quad(Quad* quad)
	{
		quad->m_current_subrenderer->remove_quad(quad);
		assign_new_quad(quad);
	}
	
	void SubRenderer::modify_quad(Quad* quad)
	{
		auto vertex_array = quad->gen_vertices();

		attempt_map();
		m_primitive_renderer->submitVertex(vertex_array.data(), vertex_array.size(), quad->m_quad_index * 4);
	}
	
	void SubRenderer::remove_quad(Quad* quad)
	{
		quad->m_current_subrenderer = nullptr;
		quad->m_assigned = false;
		
		// if last quad
		if (quad->m_quad_index + 1 == m_quad_index)
		{
			// reduce drawn vertices
			m_primitive_renderer->vertexCount() -= 4;
			m_quad_index--;
		}
		else
		{
			// else fill with empty data
			const static std::array<VertexData, 4> empty_filler = {
				VertexData({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }),
				VertexData({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }),
				VertexData({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }),
				VertexData({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f })
			}; // this could just be array of 0 bytes, but this looks nicer

			attempt_map();
			m_primitive_renderer->submitVertex(empty_filler.data(), empty_filler.size(), quad->m_quad_index * 4);
		}
	}



	void Renderer::destroyQuads()
	{
		clear();
	}

	void Renderer::clear()
	{
		for(auto quad : m_quads) 
		{
			delete quad;
		}
		
		for (auto subrenderer : m_renderers)
		{
			delete subrenderer.second->m_primitive_renderer;
			delete subrenderer.second;
		}
		
		m_quads.clear();
		m_renderers.clear();
	}
	
	void Renderer::updateQuad(Quad* quad)
	{
		if (!quad->m_current_subrenderer)
		{
			// new quad
			quad->m_updated = false;
			quad->m_sprite_updated = false;
			auto new_subrenderer = select_subrenderer(quad);
			if (new_subrenderer->m_quad_index >= m_renderer_info.max_primitive_count)
			{
				spdlog::error("subrenderer primitive overflow!");
				return;
			}
			new_subrenderer->assign_new_quad(quad);
		}
		else if (quad->m_sprite_updated)
		{
			// move quad to another subrenderer
			quad->m_sprite_updated = false;
			auto new_subrenderer = select_subrenderer(quad);
			auto old_subrenderer = quad->m_current_subrenderer;

			if (old_subrenderer == new_subrenderer)
			{
				// modified quad vertices
				quad->m_updated = false;
				quad->m_current_subrenderer->modify_quad(quad);
			}

			quad->m_current_subrenderer->remove_quad(quad);
			if (old_subrenderer->m_quad_index == 0)
			{
				removeSubrendererWithTexture(old_subrenderer->m_texture);
			}
			
			if (new_subrenderer->m_quad_index >= m_renderer_info.max_primitive_count)
			{
				spdlog::error("subrenderer primitive overflow!");
				return;
			}
			new_subrenderer->assign_new_quad(quad);
		}
		else if (quad->m_updated)
		{
			// modified quad vertices
			quad->m_updated = false;
			quad->m_current_subrenderer->modify_quad(quad);
		}
		else
		{
			// nothing has changed
		}
	}

	void Renderer::removeSubrendererWithTexture(const Texture* texture)
	{
		auto iter = m_renderers.find(texture);
		if (iter != m_renderers.end())
		{
			delete iter->second->m_primitive_renderer;
			delete iter->second;
			m_renderers.erase(iter);
		}
	}

	Quad* Renderer::createQuad()
	{
		auto quad = new Quad(this);
		m_quads.insert(quad);

		return quad;
	}

	void Renderer::destroyQuad(Quad* quad)
	{
		auto old_subrenderer = quad->m_current_subrenderer;
		if (old_subrenderer)
		{
			quad->m_current_subrenderer->remove_quad(quad);
			if (old_subrenderer->m_quad_index == 0)
			{
				removeSubrendererWithTexture(old_subrenderer->m_texture);
			}
		}

		m_quads.erase(quad);
		delete quad;
	}
	
	void Renderer::render()
	{
		// spdlog::debug("subrenderer count: {}", m_renderers.size());
		for (auto subrenderer : m_renderers)
		{
			// spdlog::debug("primitive count: {}", subrenderer.second->m_quad_index);
			subrenderer.first->bind();
			subrenderer.second->render();
		}
	}

}
