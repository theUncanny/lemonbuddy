#include <iomanip>

#include "utils/color.hpp"
#include "utils/string.hpp"
#include "x11/color.hpp"

LEMONBUDDY_NS

map<string, color> g_colorstore;
color g_colorempty{"#00000000"};
color g_colorblack{"#ff000000"};
color g_colorwhite{"#ffffffff"};

color::color(string hex) : m_source(hex) {
  if (hex.empty()) {
    throw application_error("Cannot create color from empty hex");
  }

  auto value = static_cast<uint32_t>(std::strtoul(&hex[1], nullptr, 16));

  // Premultiply alpha
  auto a = color_util::alpha_channel(value);
  auto r = color_util::red_channel(value) * a / 255;
  auto g = color_util::green_channel(value) * a / 255;
  auto b = color_util::blue_channel(value) * a / 255;

  m_color = color_util::make_32bit(((a << 24) | (r << 16) | (g << 8) | b));
}

string color::source() const {
  return m_source;
}

color::operator XRenderColor() const {
  XRenderColor x;
  x.red = color_util::red_channel<uint8_t>(m_color) << 8;
  x.green = color_util::green_channel<uint8_t>(m_color) << 8;
  x.blue = color_util::blue_channel<uint8_t>(m_color) << 8;
  x.alpha = color_util::alpha_channel<uint8_t>(m_color) << 8;
  return x;
}

color::operator string() const {
  return color_util::hex(m_color);
}

color::operator uint32_t() const {
  return m_color.colorspace.value;
}

color color::parse(string input, color fallback) {
  if (input.empty()) {
    throw application_error("Cannot parse empty color");
  }

  auto it = g_colorstore.find(input);
  if (it != g_colorstore.end()) {
    return it->second;
  }

  string hex{input};

  if (hex.substr(0, 1) != "#")
    hex = "#" + hex;
  if (hex.length() == 4)
    hex = {'#', hex[1], hex[1], hex[2], hex[2], hex[3], hex[3]};
  if (hex.length() == 7)
    hex = "#FF" + hex.substr(1);
  if (hex.length() != 9)
    return fallback;
  color result{hex};
  g_colorstore.emplace(input, result);
  return result;
}

color color::parse(string input) {
  return parse(input, g_colorempty);
}

LEMONBUDDY_NS_END
