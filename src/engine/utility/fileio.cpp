#include "fileio.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "engine/internal_libs.hpp"
#include "engine/utility/clock.hpp"
#include "engine/engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#define MINIMP3_IMPLEMENTATION
#include <minimp3/minimp3_ex.h>

#ifdef __EMSCRIPTEN__
#include <AL/al.h>
#elif defined(VCPKG_TOOLCHAIN)
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <al.h>
#endif

// #include <miniz/miniz_zip.h>
#include <zip.h>



constexpr int stb_i_format(oe::formats format) {
	switch (format)
	{
	case oe::formats::rgba:
		return STBI_rgb_alpha;
	case oe::formats::rgb:
		return STBI_rgb;
	case oe::formats::mono:
		return STBI_grey;
	}
}

constexpr int stb_i_channels(oe::formats format) {
	switch (format)
	{
	case oe::formats::rgba:
		return 4;
	case oe::formats::rgb:
		return 3;
	case oe::formats::mono:
		return 1;
	}
}

namespace oe::utils
{
	image_data::image_data(oe::formats _format, int _width, int _height)
		: format(_format)
		, width(_width), height(_height)
		, size(_width* _height* stb_i_channels(_format))
	{
		data = new uint8_t[size];
	}

	image_data::image_data(fs::path path, oe::formats _format)
	{
		int channels;
		data = stbi_load(path.string().c_str(), &width, &height, &channels, stb_i_format(_format));
		
		if (!data) {
			oe_error_terminate("Failed to load imagefile \"{}\"", std::string(path.string().c_str()));
		}

		format = _format;
		size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
	}

	image_data::image_data(const uint8_t* _data, size_t data_size, oe::formats _format)
	{
		int channels;
		data = stbi_load_from_memory(_data, data_size, &width, &height, &channels, stb_i_format(_format));
		
		if (!data) {
			oe_error_terminate("Failed to load imagedata {}:{}", (size_t)_data, data_size);
		}

		format = _format;
		size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
	}

	image_data::image_data(const uint8_t* _data, oe::formats _format, int _width, int _height)
		: format(_format)
		, width(_width), height(_height)
		, size(_width* _height* stb_i_channels(_format))
	{
		data = new uint8_t[size];
		std::memcpy(data, _data, size);
	}

	image_data::image_data(const image_data& _copied)
		: format(_copied.format)
		, width(_copied.width), height(_copied.height)
		, size(_copied.width* _copied.height* stb_i_channels(_copied.format))
	{
		data = new uint8_t[size];
		std::memcpy(data, _copied.data, size);
	}

	image_data::image_data(image_data&& move)
		: format(move.format)
		, width(move.width), height(move.height)
		, size(move.size)
		, data(move.data)
	{
		move.format = oe::formats::mono;
		move.width = 0; move.height = 0;
		move.size = 0;
		move.data = 0;
	}

	image_data::~image_data()
	{
		delete data;
	}

	byte_string image_data::save() const
	{
		int channels = stb_i_channels(format);
		int size;
		const uint8_t* dat = stbi_write_png_to_mem(data, width * channels, width, height, channels, &size);

		return { false, std::make_pair<const uint8_t*, size_t>(std::move(dat), static_cast<size_t>(size)) };
	}

	audio_data::audio_data(int _format, int _size, int _channels, int _sample_rate)
		: format(_format)
		, size(_size)
		, channels(_channels)
		, sample_rate(_sample_rate)
	{
		data = new int16_t[size];
	}

	audio_data::audio_data(fs::path path)
	{
		mp3dec_t mp3d;
		mp3dec_file_info_t info;
		if (mp3dec_load(&mp3d, path.string().c_str(), &info, NULL, NULL))
			oe_error_terminate("Failed to load audiofile \"{}\"", std::string(path.string().c_str()));

		size = info.samples * sizeof(int16_t);
		data = info.buffer;
		channels = info.channels;
		sample_rate = info.hz;

		// Format
		format = -1;
		if (info.channels == 1) {
			format = AL_FORMAT_MONO16;
		}
		else if (info.channels == 2) {
			format = AL_FORMAT_STEREO16;
		}
	}

	audio_data::audio_data(const uint8_t* _data, size_t data_size)
	{
		mp3dec_t mp3d;
		mp3dec_file_info_t info;
		mp3dec_load_buf(&mp3d, _data, data_size, &info, NULL, NULL);
		if (!info.buffer)
			oe_error_terminate("Failed to load audiodata {}:{}", (size_t)_data, data_size);
		

		size = info.samples * sizeof(int16_t);
		data = info.buffer;
		channels = info.channels;
		sample_rate = info.hz;

		// Format
		format = -1;
		if (info.channels == 1) {
			format = AL_FORMAT_MONO16;
		}
		else if (info.channels == 2) {
			format = AL_FORMAT_STEREO16;
		}
	}

	audio_data::audio_data(const int16_t* _data, int _format, int _size, int _channels, int _sample_rate)
		: format(_format)
		, size(_size)
		, channels(_channels)
		, sample_rate(_sample_rate)
	{
		data = new int16_t[size];
		std::memcpy(data, _data, size);
	}

	audio_data::audio_data(const audio_data& _copied)
		: format(_copied.format)
		, size(_copied.size)
		, channels(_copied.channels)
		, sample_rate(_copied.sample_rate)
	{
		data = new int16_t[size];
		std::memcpy(data, _copied.data, size);
	}

	audio_data::audio_data(audio_data&& move)
		: format(move.format)
		, size(move.size)
		, channels(move.channels)
		, sample_rate(move.sample_rate)
		, data(move.data)
	{
		move.format = 0;
		move.size = 0;
		move.channels = 0;
		move.sample_rate = 0;
		move.data = 0;
	}

	audio_data::~audio_data() {
		delete data;
	}

}

namespace oe::utils
{
	auto zip_open_error(int error)
	{
		zip_error_t ziperror;
		zip_error_init_with_code(&ziperror, error);
		return std::string(zip_error_strerror(&ziperror));
	}

	void write_in_zip(const fs::path& path_to_zip, const fs::path& path_in_zip, const byte_string& data)
	{
		auto generic_to_zip = path_to_zip.generic_string();
		auto generic_in_zip = path_in_zip.generic_string();

		int error;
		auto zipper = zip_open(generic_to_zip.c_str(), ZIP_CREATE, &error);
		if (!zipper)
			throw std::runtime_error(fmt::format("Failed to open {}, {}", generic_to_zip, zip_open_error(error)));

		try
		{
			// create folder structure
			for (fs::path::const_iterator iter = path_in_zip.begin(); ; iter++)
			{
				if (iter == path_in_zip.end() || std::next(iter) == path_in_zip.end())
					break;

				auto filename = iter->generic_string();
				zip_dir_add(zipper, filename.c_str(), ZIP_FL_ENC_UTF_8);
			}

			// add the file
			auto source = zip_source_buffer(zipper, data.m_pair.first, data.m_pair.second, 0);
			if (!source)
				throw std::runtime_error(fmt::format("Failed to create source file from data, {}", zip_strerror(zipper)));

			auto size = zip_file_add(zipper, generic_in_zip.c_str(), source, ZIP_FL_ENC_UTF_8 | ZIP_FL_OVERWRITE);
			if (size < 0)
			{
				zip_source_free(source);
				throw std::runtime_error(fmt::format("Failed to add file {} to zip, {}", generic_in_zip, zip_strerror(zipper)));
			}
		}
		catch (const std::exception& e)
		{
			zip_close(zipper);
			throw e;
		}

		zip_close(zipper);
	}

	void read_from_zip(const fs::path& path_to_zip, const fs::path& path_in_zip, byte_string& data)
	{
		auto generic_to_zip = path_to_zip.generic_string();
		auto generic_in_zip = path_in_zip.generic_string();

		int error;
		auto zipper = zip_open(generic_to_zip.c_str(), 0, &error);
		if (!zipper)
			throw std::runtime_error(fmt::format("Failed to open {}, {}", path_to_zip, zip_open_error(error)));

		try
		{
			zip_stat_t stats;
			zip_stat(zipper, generic_in_zip.c_str(), 0, &stats);

			auto file = zip_fopen(zipper, generic_in_zip.c_str(), 0);
			if (!file)
				throw std::runtime_error(fmt::format("Failed to open file {} from zip, {}", generic_in_zip.c_str(), zip_strerror(zipper)));

			uint8_t* data_ptr = new uint8_t[stats.size];
			data.m_pair.first = data_ptr;
			data.m_pair.second = stats.size;
			auto read_size = zip_fread(file, data_ptr, stats.size);
			if (read_size < 0)
			{
				zip_fclose(file);
				throw std::runtime_error(fmt::format("Failed to read file {} from zip, {}", generic_in_zip.c_str(), zip_strerror(zipper)));
			}
			zip_fclose(file);
		}
		catch (const std::exception& e)
		{
			zip_close(zipper);
			throw e;
		}

		zip_close(zipper);
	}

	void zip_entries(const fs::path& path_to_zip, const fs::path& path_in_zip, std::vector<fs::path>& vec)
	{

	}

	void zip_paths(const std::vector<fs::path::const_iterator>& iter, const fs::path& current_path, fs::path& path_to_zip, fs::path& path_in_zip)
	{
		for (auto left_iter = current_path.begin(); left_iter != std::next(iter[0]); left_iter++)
			path_to_zip.append(left_iter->generic_string());

		for (auto right_iter = std::next(iter[0]); right_iter != current_path.end(); right_iter++)
			path_in_zip.append(right_iter->generic_string());

		if (!fs::is_regular_file(path_to_zip) && fs::exists(path_to_zip))
			throw std::runtime_error(fmt::format("Path: '{}' already exists, but is not a file", path_to_zip.generic_string()));
	}

	std::vector<fs::path::const_iterator> first_zip_loc(const FileIO& fileio)
	{
		std::vector<fs::path::const_iterator> vec;

		for (fs::path::const_iterator iter = fileio.getPath().begin(); iter != fileio.getPath().end(); iter++)
		{
			if(iter->extension() == ".zip")
				vec.push_back(iter);
		}

		return vec;
	}

	FileIO::FileIO(const fs::path& path)
		: m_current_path(path)
	{}
	
	FileIO& FileIO::open(const std::string& file_or_directory_name)
	{
		m_current_path.append(file_or_directory_name);
		return *this;
	}

	FileIO& FileIO::close(size_t n)
	{
		for(size_t i = 0; i < n; i++)
			m_current_path = m_current_path.parent_path();
		return *this;
	}
	
	std::vector<fs::path> FileIO::items() const
	{
		std::vector<fs::path> items;
		
		auto iter = first_zip_loc(*this);
		if (!iter.empty())
		{
			fs::path path_to_zip, path_in_zip;
			zip_paths(iter, m_current_path, path_to_zip, path_in_zip);

			auto generic_to_zip = path_to_zip.generic_string();
			auto generic_in_zip = path_in_zip.generic_string();

			int error;
			auto zipper = zip_open(generic_to_zip.c_str(), 0, &error);
			if (!zipper)
				throw std::runtime_error(fmt::format("Failed to open {}, {}", path_to_zip, zip_open_error(error)));

			auto n = zip_get_num_entries(zipper, 0);
			std::string last_folder;
			for(decltype(n) ni = 0; ni < n; ni++)
			{
				zip_stat_t stats;
				zip_stat_index(zipper, ni, 0, &stats);
				
				fs::path parent_path = fs::path(stats.name).parent_path();
				if(path_in_zip == parent_path)
					items.push_back(fs::path(stats.name).filename());

				if(last_folder != parent_path && parent_path.parent_path() == path_in_zip)
				{
					last_folder = parent_path.generic_string();
					items.push_back(last_folder);
				}
			}
		}
		else
		{
			for(auto& iter : fs::directory_iterator(m_current_path))
				items.push_back(iter);
		}
		return items;
	}
	
	const fs::path& FileIO::getPath() const
	{
		return m_current_path;
	}

	bool FileIO::isDirectory() const
	{
		return fs::is_directory(m_current_path);
	}

	bool FileIO::isFile() const
	{
		return fs::is_regular_file(m_current_path);
	}
	
	bool FileIO::exists() const
	{
		return fs::exists(m_current_path);
	}

	void FileIO::remove() const
	{
		fs::remove_all(m_current_path);
	}

	byte_string FileIO::readBytes() const
	{
		byte_string data{ true, std::make_pair<const uint8_t*, size_t>(nullptr, 0) };

		auto iter = first_zip_loc(*this);
		if (!iter.empty())
		{
			fs::path path_to_zip, path_in_zip;
			zip_paths(iter, m_current_path, path_to_zip, path_in_zip);

			read_from_zip(path_to_zip, path_in_zip, data);
		}
		else
		{
			std::ifstream input_stream(m_current_path);
			if (!input_stream.is_open())
				throw std::runtime_error(fmt::format("Could not open file: '{}'", m_current_path.string()));

			input_stream.seekg(input_stream.end);
			size_t data_size = input_stream.tellg();
			uint8_t* data_ptr = new uint8_t[data_size];
			data.m_pair.first = data_ptr;
			data.m_pair.second = data_size;
			input_stream.read(reinterpret_cast<char*>(data_ptr), data_size);
			input_stream.close();
		}

		return std::move(data);
	}

	void FileIO::writeBytes(const byte_string& bytes) const
	{
		auto iter = first_zip_loc(*this);
		if (!iter.empty())
		{
			fs::path path_to_zip, path_in_zip;
			zip_paths(iter, m_current_path, path_to_zip, path_in_zip);

			write_in_zip(path_to_zip, path_in_zip, bytes);
		}
		else
		{
			if (!isFile() && exists())
				throw std::runtime_error(fmt::format("Path: '{}' already exists, but is not a file", m_current_path.string()));

			if (!exists())
				fs::create_directories(m_current_path.parent_path());

			std::ofstream output_stream(m_current_path);
			if (!output_stream.is_open())
				throw std::runtime_error(fmt::format("Could not open file: '{}'", m_current_path.string()));

			output_stream.write(reinterpret_cast<const char*>(bytes.m_pair.first), bytes.m_pair.second);
			output_stream.close();
		}
	}
}