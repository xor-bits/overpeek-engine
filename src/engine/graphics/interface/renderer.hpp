#pragma once

#include "engine/internal_libs.hpp"
#include "engine/graphics/sprite.hpp"

#include "primitive_renderer.hpp"

#include <unordered_set>



namespace oe::graphics {

	class IWindow; class Renderer; struct SubRenderer;


	struct Quad {
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_size = { 0.0f, 0.0f };
		glm::vec2 m_rotation_alignment = { 0.0f, 0.0f };
		glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Sprite m_sprite;
		bool m_sprite_updated = false;
		bool m_updated = false;
		float m_rotation = 0.0f;
		size_t m_quad_index = 0;
		bool m_assigned = false;

		Renderer* m_renderer;
		SubRenderer* m_current_subrenderer = nullptr;
		
		Quad(Renderer* host) {
			m_renderer = host;
		}
		~Quad() {}

		size_t getQuadIndex() const {
			return m_quad_index;
		}
		

		void reset() { m_position = { 0.0f, 0.0f, 0.0f }; m_size = { 0.0f, 0.0f }; m_color = { 0.0f, 0.0f, 0.0f, 0.0f }; }
		void setPosition(const glm::vec3& position) { if (m_position != position) { m_updated = true; } m_position = position; }
		void setPosition(const glm::vec2& position) { if (m_position.x != position.x || m_position.y != position.y) { m_updated = true; } m_position.x = position.x; m_position.y = position.y; }
		void setX(float x) { if (m_position.x != x) { m_updated = true; } m_position.x = x; }
		void setY(float y) { if (m_position.y != y) { m_updated = true; } m_position.y = y; }
		void setZ(float z) { if (m_position.z != z) { m_updated = true; } m_position.z = z; }
		const glm::vec3& getPosition() const { return m_position; }

		void setSize(const glm::vec2& size) { if (m_size != size) { m_updated = true; } m_size = size; }
		const glm::vec2& getSize() const { return m_size; }

		void setRotationAlignment(const glm::vec2& align) { if (m_rotation_alignment != align) { m_updated = true; } m_rotation_alignment = align; }
		const glm::vec2& getRotationAlignment() const { return m_rotation_alignment; }

		void setColor(const glm::vec4& color) { if (m_color != color) { m_updated = true; } m_color = color; }
		const glm::vec4& getColor() const { return m_color; }

		void expandSprite() { m_updated = true; m_sprite.position = { 0.0f, 0.0f }; m_sprite.size = { 1.0f, 1.0f }; }
		void setSprite(const Sprite& sprite) { if (m_sprite.m_owner != sprite.m_owner) { m_sprite_updated = true; } else if(m_sprite.position != sprite.position || m_sprite.size != sprite.size) { m_updated = true; } m_sprite = sprite; }
		void setSprite(const Sprite* sprite) { setSprite(*sprite); }
		const Sprite* getSprite() const { return &m_sprite; }

		void setRotation(float rotation) { if (m_rotation != rotation) { m_updated = true; } m_rotation = rotation; }
		float getRotation() const { return m_rotation; }

		void update();

		std::array<VertexData, 4> gen_vertices() const;
	};

	struct SubRenderer {
		PrimitiveRenderer m_primitive_renderer;
		size_t m_quad_index = 0;
		std::unordered_set<size_t> m_quad_index_free_spots;
		bool m_buffer_bound = false;
		Texture m_texture;

		void attempt_map();
		void attempt_unmap();
		void render();

		void assign_new_quad(Quad* quad);
		void reassign_quad(Quad* quad);
		void modify_quad(Quad* quad);
		void remove_quad(Quad* quad);
	};

	class Renderer {
	private:
		RendererInfo m_renderer_info;
		std::unordered_map<size_t, SubRenderer*> m_renderers; // size_t being pointer to texture
		std::unordered_set<Quad*> m_quads;

		SubRenderer* select_subrenderer(Quad* quad);
	
	public:
		Renderer(const RendererInfo& renderer_info);
		virtual ~Renderer();

		void removeSubrendererWithTexture(size_t texture);
		void updateQuad(Quad* quad);

		Quad* createQuad();
		void destroyQuad(Quad* quad);
		void destroyQuads();
		void clear();

		void render();

	};

};

template <>
struct fmt::formatter<oe::graphics::Quad> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const oe::graphics::Quad& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ xyz: {}, wh: {}, c: {}, ud: {}, uds: {}, as?: {} ]", d.m_position, d.m_size, d.m_color, d.m_updated, d.m_sprite_updated, d.m_assigned);
	}
};