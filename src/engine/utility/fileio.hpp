#pragma once

#include <string>
#include <fstream>

#include "engine/enum.hpp"
#include "engine/engine.hpp"



namespace oe::utils
{
	struct byte_string
	{
		bool m_destruct = true;
		std::pair<const uint8_t*, size_t> m_pair;

		byte_string(const bool& destruct, std::pair<const uint8_t*, size_t>&& pair) : m_destruct(destruct), m_pair(std::move(pair)) {};
		byte_string(const byte_string&) = delete;
		byte_string(byte_string&& move) : m_destruct(move.m_destruct), m_pair(std::move(move.m_pair)) { move.m_destruct = false; move.m_pair.first = nullptr; move.m_pair.second = 0; };
		~byte_string() { if (m_destruct) delete m_pair.first; }
	};

	struct image_data
	{
		uint8_t* data;
		oe::formats format;
		int width, height;
		size_t size;

		image_data(oe::formats format, int width, int height); // allocates space for uint8_t*data
		image_data(fs::path path, oe::formats format = oe::formats::rgba); // load from file
		image_data(const uint8_t* data, size_t data_size, oe::formats format = oe::formats::rgba); // load from memory
		image_data(const uint8_t* data, oe::formats format, int width, int height);
		image_data(const image_data& copied);
		image_data(image_data&& move);
		~image_data();

		byte_string save() const;
	};

	struct audio_data
	{
		int16_t* data;
		int format, size, channels, sample_rate;
		const bool stolen = false;

		audio_data(int format, int size, int channels, int sample_rate); // allocates space for uint16_t*data
		audio_data(fs::path path); // load from file
		audio_data(const uint8_t* data, size_t data_size); // load from memory
		audio_data(const int16_t* data, int format, int size, int channels, int sample_rate);
		audio_data(const audio_data& copied);
		audio_data(audio_data&& move);
		~audio_data();
	};

	// std fstream and libzip wrapper
	// currently supports zip nesting depth just up to max 1
	class FileIO
	{
	private:
		fs::path m_current_path;

	public:
		FileIO(const fs::path& path = std::filesystem::current_path());

		FileIO& open(const std::string& file_or_directory_name); // open directory or directories
		FileIO& close(size_t n = 1); // go back or close directories
		
		const fs::path& getPath() const;
		bool isDirectory() const;
		bool isFile() const;
		bool exists() const;

		void remove() const;

		// read/write any
		template<typename T> T read() const;
		template<typename T> void write(const T&) const;

		image_data read(oe::formats format) const
		{
			auto dat = readBytes();
			return image_data(dat.m_pair.first, dat.m_pair.second, format);
		}

		template<>
		image_data FileIO::read() const
		{
			return read(oe::formats::rgba);
		}

		template<>
		void FileIO::write(const image_data& image) const
		{
			return writeBytes(image.save());
		}

		template<>
		audio_data FileIO::read() const
		{
			return audio_data(m_current_path);
		}

		template<> // no def
		void FileIO::write(const audio_data& audio) const;

		template<>
		std::string FileIO::read() const
		{
			auto dat = readBytes();
			return { dat.m_pair.first, dat.m_pair.first + dat.m_pair.second };
		}

		template<>
		void FileIO::write(const std::string& string) const
		{
			byte_string data{ false, std::make_pair(reinterpret_cast<const uint8_t*>(string.data()), string.size()) };
			return writeBytes(data);
		}

	private:
		byte_string readBytes() const;
		void writeBytes(const byte_string&) const;
	};
}
