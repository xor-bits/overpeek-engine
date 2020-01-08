#pragma once

#include "engine/graphics/texture.h"
#include "engine/internal_libs.h"



namespace oe::graphics {

	class FrameBuffer {
	private:
		unsigned int p_rbo;
		unsigned int p_id;

		Texture *p_texture;
		int p_width;
		int p_height;

	public:
		FrameBuffer(int width, int height);
		~FrameBuffer();

		void bind();
		static void unbind();

		void clear();
		void clearColor(glm::vec4 c);

		inline const Texture* getTexture() const { return p_texture; }
		inline int getWidth() const { return p_width; }
		inline int getHeight() const { return p_height; }
		inline float getAspect() const { return (float)p_width / (float)p_height; }
	};

}