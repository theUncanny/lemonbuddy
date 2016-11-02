#pragma once

#include <iomanip>

#include "common.hpp"
#include "utils/string.hpp"
#include "x11/xlib.hpp"

LEMONBUDDY_NS

namespace color_util {
  template <typename ChannelType = uint8_t>
  struct color {
    using type = color<ChannelType>;

    union {
      struct {
        ChannelType red;
        ChannelType green;
        ChannelType blue;
        ChannelType alpha;
      } bits;

      uint32_t value = 0U;
    } colorspace;

    explicit color() = default;
    explicit color(uint32_t v) {
      colorspace.value = v;
    }

    color<ChannelType> operator=(const uint32_t v) {
      colorspace.value = v;
      return *this;
    }

    operator uint32_t() {
      return colorspace.value;
    }
  };

  template <typename T = uint8_t>
  T alpha_channel(const uint32_t value) {
    uint8_t a = value >> 24;
    if (std::is_same<T, uint8_t>::value)
      return a << 8 / 0xFF;
    else if (std::is_same<T, uint16_t>::value)
      return a << 8 | a << 8 / 0xFF;
  }

  template <typename T = uint8_t>
  T alpha_channel(const color<T>& c) {
    return color_util::alpha_channel<T>(c.colorspace.value);
  }

  template <typename T = uint8_t>
  T red_channel(const uint32_t value) {
    uint8_t r = value >> 16;
    if (std::is_same<T, uint8_t>::value)
      return r << 8 / 0xFF;
    else if (std::is_same<T, uint16_t>::value)
      return r << 8 | r << 8 / 0xFF;
  }

  template <typename T = uint8_t>
  T red_channel(const color<T>& c) {
    return color_util::red_channel<T>(c.colorspace.value);
  }

  template <typename T = uint8_t>
  T green_channel(const uint32_t value) {
    uint8_t g = value >> 8;
    if (std::is_same<T, uint8_t>::value)
      return g << 8 / 0xFF;
    else if (std::is_same<T, uint16_t>::value)
      return g << 8 | g << 8 / 0xFF;
  }

  template <typename T = uint8_t>
  T green_channel(const color<T>& c) {
    return color_util::green_channel<T>(c.colorspace.value);
  }

  template <typename T = uint8_t>
  T blue_channel(const uint32_t value) {
    uint8_t b = value;
    if (std::is_same<T, uint8_t>::value)
      return b << 8 / 0xFF;
    else if (std::is_same<T, uint16_t>::value)
      return b << 8 | b << 8 / 0xFF;
  }

  template <typename T = uint8_t>
  T blue_channel(const color<T>& c) {
    return color_util::blue_channel<T>(c.colorspace.value);
  }

  template <typename T = uint8_t>
  string hex(const color<T>& color) {
    auto value = color.colorspace.value;
    auto width = 8;

    if (std::is_same<T, uint8_t>::value) {
      value = value & 0x00FFFFFF;
      width = 6;
    }

    // clang-format off
    return string_util::from_stream(stringstream()
        << "#"
        << std::setw(width)
        << std::setfill('0')
        << std::hex
        << std::uppercase
        << value);
    // clang-format on
  }

  color<uint8_t> make_24bit(uint32_t value);
  color<uint16_t> make_32bit(uint32_t value);
}

LEMONBUDDY_NS_END
