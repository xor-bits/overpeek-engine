#include "textureManager.h"



namespace oe {

	unsigned char* TextureManager::m_textureBuffer;
	size_t TextureManager::m_textureBufferCurrent;
	size_t TextureManager::m_textureBufferSize;

	int TextureManager::m_r;
	oe::Texture TextureManager::m_texture;
	size_t TextureManager::m_current_layer;

	void TextureManager::init(int wh) {
		m_r = wh;
		m_textureBuffer = new unsigned char[0];
		m_textureBufferSize = 0;
		m_current_layer = 0;

		// first image is always empty image (for non textured rendering)
		auto data = new unsigned char[4 * wh * wh];
		std::memset(data, (unsigned char)255u, 4 * wh * wh);
		load(image_data(data, wh, wh, 4));
		delete[] data;
	}

	size_t TextureManager::coordsToIndex(size_t x, size_t y, size_t c, size_t width, size_t channels) {
		return channels * (x + y * width) + c;
	}

	void TextureManager::resize(size_t added_bytes) {
		size_t newSize = m_textureBufferSize + added_bytes;
		unsigned char* newArr = new unsigned char[newSize];

		memcpy(newArr, m_textureBuffer, m_textureBufferSize * sizeof(unsigned char));

		m_textureBufferSize = newSize;
		delete[] m_textureBuffer;
		m_textureBuffer = newArr;
	}

	TextureManager::multitexture TextureManager::load(oe::image_data image) {
		//Texture data
		size_t rows = image.height / m_r;
		size_t columns = image.width / m_r;
		multitexture returnData = std::make_pair(m_current_layer, size_t(rows * columns));
		m_current_layer += rows * columns;
		resize(image.width * image.height * 4UL);

		//Data processing
		// all subimages
		for (int j = 0; j < rows; j++) {
			for (int i = 0; i < columns; i++) {

				// all pixels in all subimages
				for (int y = 0; y < m_r; y++) {
					for (int x = 0; x < m_r; x++) {
						// for all color channels (r, g, b, a)
						for (int c = 0; c < 4; c++)
						{
							int image_x = i * m_r + x;
							int image_y = j * m_r + y;
							size_t position = coordsToIndex(image_x, image_y, c, image.width, 4);
							m_textureBuffer[m_textureBufferCurrent++] = image.data[position];
						}
					}
				}

			}

		}
		return returnData;
	}

	TextureManager::multitexture TextureManager::load(std::filesystem::path path) {
		oe::image_data image = oe::loadImage(path);
		TextureManager::multitexture returnData = load(image);
		oe::freeImage(image);
		return returnData;
	}

	void TextureManager::finish() {
		int layers = m_current_layer;

		m_texture = oe::Texture::Texture();
		m_texture.load3D(m_textureBuffer, m_r, m_r, layers);

		delete[] m_textureBuffer;
	}

	void TextureManager::bind() {
		m_texture.bind();
	}

}