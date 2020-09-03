#pragma once

#include "engine/internal_libs.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/interfacegen_renderer.hpp"

#include <unordered_set>



namespace oe::graphics {

	class IWindow; class Renderer; class SubRenderer;


	class Quad : std::enable_shared_from_this<Quad>
	{
	private:
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
		bool m_toggled = true;

		std::shared_ptr<SubRenderer> m_current_subrenderer;
		Renderer& m_renderer;
		size_t m_id = 0;
		
	public:
		// construct
		Quad(Renderer& host);
		~Quad();
		std::shared_ptr<Quad> shared();

		// subrenderer quad render index
		size_t getQuadIndex() const;

		// visible
		void toggle(bool enabled) { if (m_toggled != enabled) { m_updated = true; } m_toggled = enabled; };
		
		// positional setters/getters
		void setPosition(const glm::vec3& position) { if (m_position != position) { m_updated = true; } m_position = position; }
		void setPosition(const glm::vec2& position) { if (m_position.x != position.x || m_position.y != position.y) { m_updated = true; } m_position.x = position.x; m_position.y = position.y; }
		void setX(float x) { if (m_position.x != x) { m_updated = true; } m_position.x = x; }
		void setY(float y) { if (m_position.y != y) { m_updated = true; } m_position.y = y; }
		void setZ(float z) { if (m_position.z != z) { m_updated = true; } m_position.z = z; }
		const glm::vec3& getPosition() const { return m_position; }

		// scale setters/getters
		void setSize(const glm::vec2& size) { if (m_size != size) { m_updated = true; } m_size = size; }
		const glm::vec2& getSize() const { return m_size; }

		// alignment setters/getters
		void setRotationAlignment(const glm::vec2& align) { if (m_rotation_alignment != align) { m_updated = true; } m_rotation_alignment = align; }
		const glm::vec2& getRotationAlignment() const { return m_rotation_alignment; }

		// color setters/getters
		void setColor(const glm::vec4& color) { if (m_color != color) { m_updated = true; } m_color = color; }
		const glm::vec4& getColor() const { return m_color; }

		// sprite setters/getters
		void expandSprite() { m_updated = true; m_sprite.position = { 0.0f, 0.0f }; m_sprite.size = { 1.0f, 1.0f }; }
		void setSprite(const Sprite& sprite) { if (m_sprite.m_owner != sprite.m_owner) { m_sprite_updated = true; } else if(m_sprite.position != sprite.position || m_sprite.size != sprite.size) { m_updated = true; } m_sprite = sprite; }
		void setSprite(const Sprite* sprite) { setSprite(*sprite); }
		const Sprite* getSprite() const { return &m_sprite; }

		// rotation setters/getters
		void setRotation(float rotation) { if (m_rotation != rotation) { m_updated = true; } m_rotation = rotation; }
		float getRotation() const { return m_rotation; }

		// update after modifications
		void update();
		void update(const std::shared_ptr<Quad>& ptr);

		// generate vertices, pointer must have room for 4 VertexData obj:s
		void gen_vertices(VertexData* ref) const;

		friend struct SubRenderer;
		friend struct Renderer;
		friend struct fmt::formatter<oe::graphics::Quad>;
	};

	class SubRenderer : public std::enable_shared_from_this<SubRenderer>
	{
	private:
		PrimitiveRenderer m_primitive_renderer;
		size_t m_quad_index = 0;
		std::unordered_set<size_t> m_quad_index_free_spots;
		bool m_buffer_bound = false;
		Texture m_texture;

		Renderer& m_renderer;
		size_t m_id = 0;

	public:
		SubRenderer(Renderer& host);
		~SubRenderer();
		std::shared_ptr<SubRenderer> shared();

		void attempt_map();
		void attempt_unmap();
		void render();

		void assign_new_quad(const std::shared_ptr<Quad>& quad);
		void reassign_quad(const std::shared_ptr<Quad>& quad);
		void modify_quad(const std::shared_ptr<Quad>& quad);
		void remove_quad(const std::shared_ptr<Quad>& quad);

		friend struct Quad;
		friend struct Renderer;
	};

	class Renderer
	{
	private:
		RendererInfo m_renderer_info;
		std::unordered_map<size_t, std::weak_ptr<SubRenderer>> m_renderers;   // size_t being pointer to texture
		std::unordered_map<size_t, std::weak_ptr<Quad>> m_quads;              // ^^
		std::unordered_map<size_t, std::shared_ptr<Quad>> m_quads_forgotten;  // ^^
	
	public:
		Renderer(const RendererInfo& renderer_info);
		virtual ~Renderer();

		std::shared_ptr<Quad> create();
		void forget(std::shared_ptr<Quad>&& quad); // can only be removed with clear
		void clear();
		void render();
	
	private:
		std::shared_ptr<SubRenderer> select_subrenderer(const std::shared_ptr<Quad>& quad) { return select_subrenderer(quad->m_sprite); }
		std::shared_ptr<SubRenderer> select_subrenderer(const Sprite& sprite) { return select_subrenderer(sprite.m_owner); }
		std::shared_ptr<SubRenderer> select_subrenderer(const Texture& texture);
		void update(const std::shared_ptr<Quad>& quad);

	friend struct Quad;
	friend struct SubRenderer;
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