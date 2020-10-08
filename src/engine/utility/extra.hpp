#pragma once

#include "engine/internal_libs.hpp"
#include <string>
#include <locale>
#include <codecvt>



namespace oe::utils {

	extern void rotatePoint(glm::vec2 center, float angle, glm::vec2& p);
	extern void rotatePoint(float angle, glm::vec2& p);

	extern std::string boolToString(bool b);
	extern bool isInRange(int input, int min, int max);
	extern int sign(float n);
	extern bool bounding_box_test(const glm::vec2& point, const glm::vec2& top_left, const glm::vec2& size);

	template<typename T>
	T clamp(const T _val, const T _min, const T _max) {
		return std::max(std::min(_val, _max), _min);
	}

	template<typename T>
	T map(const T value, const T low1, const T high1, const T low2, const T high2) {
		return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
	}

	template <typename T>
	bool weak_ptr_test(const std::weak_ptr<T>& ptr)
	{
		return !ptr.owner_before(std::weak_ptr<T>{}) && !std::weak_ptr<T>{}.owner_before(ptr);
	}

	// { 0 - 1, 0 - 1, 0 - 1 } -> { 0 - 1, 0 - 1, 0 - 1 }
	// https://stackoverflow.com/a/17897228 modified for glm
	extern glm::vec3 rgbToHSV(glm::vec3 in);
	extern glm::vec3 hsvToRGB(glm::vec3 in);

	// https://github.com/coder0xff/Plange/blob/master/source/utilities/utf.cpp
	template<typename to, typename from>
	to convertUTF(const from&);

	// hex conversions
	extern glm::ivec3 hexToRGB(unsigned long hex);
	extern unsigned long RGBtoHex(glm::ivec3 rgb);
	extern std::string hexToString(unsigned long hex);
	template<typename chr_type> unsigned long stringToHex(const std::basic_string<chr_type>& str);

	// string checks
	template<typename chr_type> bool isNumber(const std::basic_string<chr_type>& input);
	template<typename chr_type> const chr_type* strchr(const chr_type* input, chr_type c);
	template<typename chr_type> bool checkChar(const std::basic_string<chr_type>& text, chr_type character, size_t position);

	// string to T where T is any integer or float type
	template<class number>
	static number stoT(const std::string& str);
	
	template<> static int stoT(const std::string& str) { return std::stoi(str); }
	template<> static long stoT(const std::string& str) { return std::stol(str); }
	template<> static long long stoT(const std::string& str) { return std::stoll(str); }
	template<> static unsigned int stoT(const std::string& str) { return std::stoul(str); }
	template<> static unsigned long stoT(const std::string& str) { return std::stoul(str); }
	template<> static unsigned long long stoT(const std::string& str) { return std::stoull(str); }
	template<> static float stoT(const std::string& str) { return std::stof(str); }
	template<> static double stoT(const std::string& str) { return std::stod(str); }
	template<> static long double stoT(const std::string& str) { return std::stold(str); }

	// vector conversions
	template<size_t dim, typename T>
	bool containerToVec(const std::vector<T>& from, glm::vec<dim, T>& to)
	{
		if (from.size() < dim)
			return false;

		glm::vec<dim, T> vec;
		std::memcpy(glm::value_ptr(to), from.data(), dim * sizeof(T));
		return true;
	}
	template<size_t dim, typename T>
	bool vecToContainer(const glm::vec<dim, T>& from, std::vector<T>& to)
	{
		to.resize(dim);
		std::memcpy(to.data(), glm::value_ptr(from), dim * sizeof(T));
		return true;
	}
	template<size_t dim, typename T>
	bool containerToVec(const std::array<T, dim>& from, glm::vec<dim, T>& to)
	{
		std::memcpy(glm::value_ptr(to), from.data(), dim * sizeof(T));
		return true;
	}
	template<size_t dim, typename T>
	bool vecToContainer(const glm::vec<dim, T>& from, std::array<T, dim>& to)
	{
		std::memcpy(to.data(), glm::value_ptr(from), dim * sizeof(T));
		return true;
	}
	
	// templates
	template<typename chr_type> unsigned long stringToHex(const std::basic_string<chr_type>& str)
	{
		if (!checkChar(str, (chr_type)'#', 0)) return -1; // not a hexcode

		long hex = std::stol(oe::utils::convertUTF<std::basic_string<chr_type>, std::string>(str.substr(1, 6)), nullptr, 16);
		if (hex < 0) return -1; // not a hexcode

		return hex;
	}
	template<typename chr_type> bool isNumber(const std::basic_string<chr_type>& input)
	{
		auto iter = input.begin();
		while (iter != input.end() && std::isdigit(*iter)) iter++;
		return !input.empty() && iter == input.end();
	}
	template<typename chr_type> const chr_type* strchr(const chr_type* input, chr_type c)
	{
		for (; *input != '\0'; ++input)
		{
			if (*input == c)
				return input;
		}
		return nullptr;
	}
	template<typename chr_type> bool checkChar(const std::basic_string<chr_type>& text, chr_type character, size_t position)
	{
		if (position < text.size() && text[position] == character)
		{
			return true;
		}

		return false;
	}

}
