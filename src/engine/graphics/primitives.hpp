#pragma once

#include "sprite.hpp"
#include "vertexData.hpp"



namespace oe::graphics
{
	class Renderer;

	class Quad
	{
	private:
		Renderer& m_renderer;
		int32_t m_index_in_vbo;
		bool m_updated                 = false;
		bool m_toggled                 = true;
		bool m_opacitymode             = false;
		
		glm::vec3 m_position           = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_size               = { 0.0f, 0.0f };
		glm::vec2 m_rotation_alignment = { 0.0f, 0.0f };
		glm::vec4 m_color              = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_rotation               = 0.0f;
		Sprite m_sprite;

	public:
		Quad(Renderer& renderer);
		~Quad();

		// subrenderer quad render index
		int32_t getQuadIndex() const { return m_index_in_vbo; }
		void setQuadIndex(int32_t index) { if(m_index_in_vbo != index) { m_updated = true; } m_index_in_vbo = index; };

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
		void setColor(const glm::vec4& color) { if (m_color != color) { m_updated = true; } m_color = color; setOpacityMode(/*auto*/); }
		const glm::vec4& getColor() const { return m_color; }

		// sprite setters/getters
		void expandSprite() { m_updated = true; m_sprite.position = { 0.0f, 0.0f }; m_sprite.size = { 1.0f, 1.0f }; }
		void setSprite(const Sprite& sprite) { if (m_sprite.m_owner != sprite.m_owner) { m_updated = true; } else if(m_sprite.position != sprite.position || m_sprite.size != sprite.size) { m_updated = true; } m_sprite = sprite; setOpacityMode(/*auto*/); }
		void setSprite(const Sprite* sprite) { if(!sprite) return; setSprite(*sprite); }
		const Sprite* getSprite() const { return &m_sprite; }

		// rotation setters/getters
		void setRotation(float rotation) { if (m_rotation != rotation) { m_updated = true; } m_rotation = rotation; }
		float getRotation() const { return m_rotation; }

		void setOpacityMode(/*auto*/);
		void setOpacityMode(bool opacitymode) { m_opacitymode = opacitymode; }
		bool getOpacityMode() const { return m_opacitymode; }

		// update after modifications
		// returns true if it was modified
		/* bool update(); */
		bool update(VertexData* vbo);

		// update after modifications
		// returns true if it was modified
		/* bool updateForce();
		bool updateForce(VertexData* vbo); */

		// returns true if it was modified
		bool updated() const;
		
		// generate vertices, pointer must have room for 4 VertexData obj:s
		void gen_vertices(VertexData* ref) const;
		static void gen_vertices_zero(VertexData* ref);
	};
}