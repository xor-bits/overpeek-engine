#include "extra.hpp"

#include <string>
#include <sstream>



void oe::utils::rotatePoint(glm::vec2 center, float angle, glm::vec2& p)
{
    float s = sin(angle);
    float c = cos(angle);

    // translate point back to origin:
    p.x -= center.x;
    p.y -= center.y;

    // rotate point
    float xnew = p.x * c - p.y * s;
    float ynew = p.x * s + p.y * c;

    // translate point back:
    p.x = xnew + center.x;
    p.y = ynew + center.y;
}

void oe::utils::rotatePoint(float angle, glm::vec2& p)
{
    float s = sin(angle);
    float c = cos(angle);

    // rotate point
    p = { (p.x * c - p.y * s), (p.x * s + p.y * c) };
}

std::string oe::utils::boolToString(bool b)
{
    return b ? "true" : "false";
}

bool oe::utils::isInRange(int input, int min, int max)
{
    return (input >= min) && (input <= max);
}

int oe::utils::sign(float n)
{
    return n < 0 ? -1 : 1;
}

bool oe::utils::bounding_box_test(const glm::vec2& point, const glm::vec2& top_left, const glm::vec2& size)
{
    return (point.x >= top_left.x &&
        point.x < top_left.x + size.x &&
        point.y >= top_left.y &&
        point.y < top_left.y + size.y);
}

// { 0 - 1, 0 - 1, 0 - 1 } -> { 0 - 1, 0 - 1, 0 - 1 }
// https://stackoverflow.com/a/17897228 modified for glm
glm::vec3 oe::utils::rgbToHSV(glm::vec3 in)
{
    glm::vec4 K = glm::vec4(0.0f, -1.0f / 3.0f, 2.0f / 3.0f, -1.0f);
    glm::vec4 p = glm::mix(glm::vec4(glm::vec2(in.b, in.g), glm::vec2(K.w, K.z)), glm::vec4(glm::vec2(in.g, in.b), glm::vec2(K.x, K.y)), glm::step(in.b, in.g));
    glm::vec4 q = glm::mix(glm::vec4(glm::vec3(p.x, p.y, p.w), in.r), glm::vec4(in.r, glm::vec3(p.y, p.z, p.x)), glm::step(p.x, in.r));

    float d = q.x - glm::min(q.w, q.y);
    float e = 1.0e-10f;
    return glm::vec3(glm::abs(q.z + (q.w - q.y) / (6.0f * d + e)), d / (q.x + e), q.x);
}

// { 0 - 1, 0 - 1, 0 - 1 } -> { 0 - 1, 0 - 1, 0 - 1 }
// https://stackoverflow.com/a/17897228 modified for glm
glm::vec3 oe::utils::hsvToRGB(glm::vec3 in)
{
    glm::vec4 K = glm::vec4(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);
    glm::vec3 p = glm::abs(glm::fract(glm::vec3(in.x) + glm::vec3(K)) * 6.0f - glm::vec3(K.w));
    return in.z * glm::mix(glm::vec3(K.x), glm::clamp(p - glm::vec3(K.x), 0.0f, 1.0f), in.y);
}


glm::ivec3 oe::utils::hexToRGB(unsigned long hex)
{
    return glm::vec3(
        (hex & 0xff0000) >> 16,
        (hex & 0x00ff00) >> 8,
        (hex & 0x0000ff) >> 0
    );
}

unsigned long oe::utils::RGBtoHex(glm::ivec3 rgb)
{
    return (rgb.x << 16) + (rgb.y << 8) + (rgb.z << 0);
}

std::string oe::utils::hexToString(unsigned long hex)
{
    return fmt::format("{0:x}", hex);
}

template<> std::string oe::utils::convertUTF(const std::u32string& from)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.to_bytes(from);
}

template<> std::string oe::utils::convertUTF(const std::u16string& from)
{
	std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
	return conv.to_bytes(from);
}

template<> std::string oe::utils::convertUTF(const std::wstring& from)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
	return conv.to_bytes(from);
}

template<> std::u32string oe::utils::convertUTF(const std::string& from)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.from_bytes(from);
}

template<> std::u16string oe::utils::convertUTF(const std::string& from)
{
	std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
	return conv.from_bytes(from);
}

template<> std::wstring oe::utils::convertUTF(const std::string& from)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
	return conv.from_bytes(from);
}

template<> std::string oe::utils::convertUTF(const std::string& from)
{
	return from;
}

template<> int oe::utils::stoT(const std::string& str) { return std::stoi(str); }
template<> long oe::utils::stoT(const std::string& str) { return std::stol(str); }
template<> long long oe::utils::stoT(const std::string& str) { return std::stoll(str); }
template<> unsigned int oe::utils::stoT(const std::string& str) { return std::stoul(str); }
template<> unsigned long oe::utils::stoT(const std::string& str) { return std::stoul(str); }
template<> unsigned long long oe::utils::stoT(const std::string& str) { return std::stoull(str); }
template<> float oe::utils::stoT(const std::string& str) { return std::stof(str); }
template<> double oe::utils::stoT(const std::string& str) { return std::stod(str); }
template<> long double oe::utils::stoT(const std::string& str) { return std::stold(str); }