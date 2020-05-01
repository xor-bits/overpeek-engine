#pragma once

#include "engine/internal_libs.hpp"
#include "engine/graphics/sprite.hpp"

#include "primitive_renderer.hpp"

#include <unordered_set>



namespace oe::graphics {

	class Window; class Renderer; struct SubRenderer;


	class Quad {
	private:
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_size = { 0.0f, 0.0f };
		glm::vec2 m_rotation_alignment = { 0.0f, 0.0f };
		glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const Sprite* m_sprite = nullptr;
		float m_rotation = 0.0f;
		size_t m_quad_index = 0;
		bool m_assigned = false;

		Renderer* m_renderer;
		SubRenderer* m_last_subrenderer = nullptr;
		SubRenderer* m_current_subrenderer = nullptr;
		
		Quad(Renderer* host) {
			m_renderer = host;
		}
		~Quad() {}

		size_t getQuadIndex() const {
			return m_quad_index;
		}

		friend class Renderer;

	public:
		void setPosition(const glm::vec3& position) { m_position = position; }
		void setPosition(const glm::vec2& position) { m_position = glm::vec3(position, 0.0f); }
		const glm::vec3& getPosition() const { return m_position; }

		void setSize(const glm::vec2& size) { m_size = size; }
		const glm::vec2& getSize() const { return m_size; }

		void setRotationAlignment(const glm::vec2& align) { m_rotation_alignment = align; }
		const glm::vec2& getRotationAlignment() const { return m_rotation_alignment; }

		void setColor(const glm::vec4& color) { m_color = color; }
		const glm::vec4& getColor() const { return m_color; }

		void setSprite(const Sprite* sprite) { m_sprite = sprite; }
		const Sprite* getSprite() const { return m_sprite; }

		void setRotation(float rotation) { m_rotation = rotation; }
		float getRotation() const { return m_rotation; }

		void update();

		std::array<VertexData, 4> gen_vertices() const;

	};

	struct SubRenderer {
		PrimitiveRenderer* m_primitive_renderer;
		size_t m_quad_index;
		bool m_buffer_bound;

		void attempt_map();
		void attempt_unmap();
		void render();
	};

	class Renderer {
	private:
		RendererInfo m_renderer_info;
		std::unordered_map<Texture*, SubRenderer*> m_renderers;
		std::unordered_set<Quad*> m_quads;

		SubRenderer* select_subrenderer(Quad* quad);
		void assign_new_quad(Quad* quad);
		void reassign_quad(Quad* quad);
		void modify_quad(Quad* quad);
	
	public:
		Renderer(const RendererInfo& renderer_info);
		~Renderer();

		void updateQuad(Quad* quad);

		Quad* createQuad();
		void destroyQuad(Quad* quad);
		void destroyQuads();
		void clear();

		void render();

	};

};
