#include "utils/color.hpp"

LEMONBUDDY_NS

namespace color_util {
  color<uint8_t> make_24bit(uint32_t value) {
    return color<uint8_t>(value);
  }

  color<uint16_t> make_32bit(uint32_t value) {
    return color<uint16_t>(value);
  }
}
LEMONBUDDY_NS_END
