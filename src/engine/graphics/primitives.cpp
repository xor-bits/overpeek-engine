#include "primitives.hpp"

#include "renderer.hpp"
#include "engine/utility/extra.hpp"
#include "engine/assets/texture_set/texture_set.hpp"



namespace oe::graphics
{
	class NullSpritePatcher
	{
	private:
		static NullSpritePatcher* singleton;
		NullSpritePatcher(const NullSpritePatcher& copy) = delete;
		NullSpritePatcher()
		{
			auto checker_img = oe::assets::TextureSet::generate_checkerboard();
			TextureInfo ti;
			ti.data = checker_img.data;
			ti.data_format = checker_img.format;
			ti.size_offset = { { checker_img.width, 0 }, { checker_img.height, 0 } };
			ti.wrap = oe::texture_wrap::repeat;

			m_sprite = {
				{ ti }, { 0.0f, 0.0f }, { 1.0f, 1.0f }
			};
		}

	public:
		static NullSpritePatcher& getSingleton() { if (!singleton) singleton = new NullSpritePatcher(); return *singleton; }

		oe::graphics::Sprite m_sprite;
	};
	NullSpritePatcher* NullSpritePatcher::singleton = nullptr;



	Quad::Quad(Renderer& renderer)
		: m_renderer(renderer)
	{}

	Quad::~Quad()
	{
		m_renderer.remove(this);
	}

	void gen_points(const glm::vec2& position, const glm::vec2& size, const glm::vec2& align, float angle, glm::vec2& pointA, glm::vec2& pointB, glm::vec2& pointC, glm::vec2& pointD)
	{
		// setup points
		const glm::vec2 alignment = alignmentOffset(size, align);
		pointA = size * glm::vec2(0.0f, 0.0f) - alignment;
		pointB = size * glm::vec2(0.0f, 1.0f) - alignment;
		pointC = size * glm::vec2(1.0f, 1.0f) - alignment;
		pointD = size * glm::vec2(1.0f, 0.0f) - alignment;
		
		// rotate if non 0 angle
		if(angle != 0.0f)
		{
			oe::utils::rotatePoint(angle, pointA);
			oe::utils::rotatePoint(angle, pointB);
			oe::utils::rotatePoint(angle, pointC);
			oe::utils::rotatePoint(angle, pointD);
		}

		// move to wanted offset pos
		pointA += position;
		pointB += position;
		pointC += position;
		pointD += position;
	}

	void Quad::gen_vertices(VertexData* ref) const
	{
		glm::vec2 pointA, pointB, pointC, pointD;
		gen_points(m_position, m_size * (m_toggled ? 1.0f : 0.0f), m_rotation_alignment, m_rotation, pointA, pointB, pointC, pointD);

		const glm::vec2 sprite_pos = m_sprite.position;
		const glm::vec2 sprite_size = m_sprite.size;

		ref[0] = VertexData(glm::vec3(pointA, m_position.z), sprite_pos + sprite_size * glm::vec2(0.0f, 0.0f), m_color);
		ref[1] = VertexData(glm::vec3(pointB, m_position.z), sprite_pos + sprite_size * glm::vec2(0.0f, 1.0f), m_color);
		ref[2] = VertexData(glm::vec3(pointC, m_position.z), sprite_pos + sprite_size * glm::vec2(1.0f, 1.0f), m_color);
		ref[3] = VertexData(glm::vec3(pointD, m_position.z), sprite_pos + sprite_size * glm::vec2(1.0f, 0.0f), m_color);
	}

	void Quad::gen_vertices_zero(VertexData* ref)
	{
		std::fill(reinterpret_cast<float*>(ref), reinterpret_cast<float*>(ref + 4), 0.0f);
	}

	void Quad::setOpacityMode()
	{
		m_opacitymode = m_color.a < 0.9999f || m_sprite.opacity;
	}

	/* bool Quad::update()
	{
		if(m_index_in_vbo == -1)
			return false;
		m_renderer.attemptMap();
		return update(m_renderer.getPrimitiveRenderer()->modifyVertex(4, m_index_in_vbo * 4));
	} */
	
	bool Quad::update(VertexData* vbo)
	{
		bool updated = m_updated;
		m_updated = false;
		if(!updated)
			return m_updated;

		if(!m_sprite.m_owner)
			setSprite(NullSpritePatcher::getSingleton().m_sprite);
		gen_vertices(vbo + m_index_in_vbo * 4);
		
		return updated;
	}

	/* bool Quad::updateForce() { m_updated = true; return update(); }
	bool Quad::updateForce(VertexData* vbo)  { m_updated = true; return update(vbo); } */
	
	bool Quad::updated() const
	{
		return m_updated;
	}
}