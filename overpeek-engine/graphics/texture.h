#pragma once

namespace oe {

	class Texture {
	private:
		unsigned int p_id;
		unsigned int p_width;
		unsigned int p_height;
		unsigned int p_depth;
		int p_target;

	public:
		Texture();
		~Texture();

		void bind() const;
		void unbind() const;

		void load2D(void *data, int width, int height);
		void load3D(void *data, int width, int height, int depth);
		void computeShaderBuffer(int width, int height);

		inline unsigned int getId() const { return p_id; }
		inline unsigned int getWidth() const { return p_width; }
		inline unsigned int getHeight() const { return p_height; }
		inline unsigned int getDepth() const { return p_depth; }
	};

}