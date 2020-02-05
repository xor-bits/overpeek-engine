#pragma once

#include "engine/utility/filereader.h"



namespace oe::graphics {

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
		
		/*
		Data format is always RGBA
		4 bytes
		*/
		
		void load2D(const oe::utils::image_data& data);
		void computeShaderBuffer(int width, int height);
		void generateMipMap();

		void empty2D(int width, int height);
		void load2D(const void* data, int width, int height);
		void data2D(const void* data, int offx, int offy, int width, int height);
		void empty3D(int width, int height, int depth);
		void load3D(const void* data, int width, int height, int depth);
		void data3D(const void* data, int offx, int offy, int offz, int width, int height, int depth);

		inline unsigned int getId() const { return p_id; }
		inline unsigned int getWidth() const { return p_width; }
		inline unsigned int getHeight() const { return p_height; }
		inline unsigned int getDepth() const { return p_depth; }
	};

}