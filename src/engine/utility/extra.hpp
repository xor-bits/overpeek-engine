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

	template<class T>
	T clamp(const T _val, const T _min, const T _max) {
		return std::max(std::min(_val, _max), _min);
	}

	template<class T>
	T map(const T value, const T low1, const T high1, const T low2, const T high2) {
		return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
	}

    // rgb/hsv algorthms by
    // https://stackoverflow.com/a/6930407/12147216

	extern glm::vec3 rgbToHSV(glm::vec3 in);
	extern glm::vec3 hsvToRGB(glm::vec3 in);
	extern std::string convertUTF(const std::wstring &s);
	extern std::wstring convertUTF(const std::string &s);

	// hex conversions
	extern glm::ivec3 hexToRGB(unsigned long hex);
	extern unsigned long RGBtoHex(glm::ivec3 rgb);
	extern std::string hexToString(unsigned long hex);
	template<typename chr_type> unsigned long stringToHex(const std::basic_string<chr_type>& str);

	// string checks
	template<typename chr_type> bool isNumber(const std::basic_string<chr_type>& input);
	template<typename chr_type> const chr_type* strchr(const chr_type* input, chr_type c);
	template<typename chr_type> bool checkChar(const std::basic_string<chr_type>& text, chr_type character, size_t position);
	
	// templates
	template<typename chr_type> unsigned long stringToHex(const std::basic_string<chr_type>& str)
	{
		if (!checkChar(str, (chr_type)'#', 0)) return -1; // not a hexcode

		
		std::basic_stringstream<chr_type> hex_str;
		hex_str << std::hex << str.substr(1, 6);
		std::basic_string<chr_type> hex_sstring = hex_str.str();
		long hex = std::stol(hex_sstring, nullptr, 16);
		if (!isNumber(hex_sstring) || hex < 0) return -1; // not a hexcode

		return hex;
	}
	template<typename chr_type> bool isNumber(const std::basic_string<chr_type>& input)
	{
		if (strchr(input.c_str(), (chr_type)'x')) return false; // no hex
		if (strchr(input.c_str(), (chr_type)'b')) return false; // or oct
		if (strchr(input.c_str(), (chr_type)'o')) return false; // or bin

		size_t p = 0;
		long hex = std::stol(input, &p, 16);
		return p == input.size();
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
