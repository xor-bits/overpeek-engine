#pragma once

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

glm::vec3 oe::utils::rgbToHSV(glm::vec3 in)
{
    glm::vec3   out;
    float      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min < in.b ? min : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max > in.b ? max : in.b;

    out.z = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.r = 0; // undefined, maybe nan?
        return out;
    }
    if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
        out.y = (delta / max);                  // s
    }
    else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.x = NAN;                            // its now undefined
        return out;
    }
    if (in.r >= max)
        out.x = (in.g - in.b) / delta;        // between yellow & magenta
    else
        if (in.g >= max)
            out.x = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
        else
            out.x = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

    out.x *= 60.0;                              // degrees

    if (out.x < 0.0)
        out.x += 360.0;

    return out;
}

glm::vec3 oe::utils::hsvToRGB(glm::vec3 in)
{
    float      hh, p, q, t, ff;
    long        i;
    glm::vec3   out;

    if (in.s <= 0.0) {
        out.r = in.y;
        out.g = in.y;
        out.b = in.y;
        return out;
    }
    hh = in.x;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.z * (1.0 - in.y);
    q = in.z * (1.0 - (in.y * ff));
    t = in.z * (1.0 - (in.y * (1.0 - ff)));

    switch (i) {
    case 0:
        out.r = in.z;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.z;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.z;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.z;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.z;
        break;
    case 5:
    default:
        out.r = in.z;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
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
    return rgb.x << 16 + rgb.y << 8 + rgb.z << 0;
}

std::string oe::utils::hexToString(unsigned long hex)
{
    return fmt::format("{0:x}", hex);
}

std::string oe::utils::convertUTF(const std::wstring &s)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
    return conv.to_bytes(s);
}

std::wstring oe::utils::convertUTF(const std::string &s)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
    return conv.from_bytes(s);
}