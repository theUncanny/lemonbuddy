#include "common/test.hpp"
#include "utils/color.hpp"

int main() {
  using namespace lemonbuddy;

  "rgb"_test = []{
    auto color = color_util::make_24bit(0x123456);
    expect(color_util::alpha_channel(color) == 0);
    expect(color_util::red_channel<uint8_t>(color) == 0x12);
    expect(color_util::green_channel<uint8_t>(color) == 0x34);
    expect(color_util::green_channel<uint16_t>(color) == 0x3434);
    expect(color_util::blue_channel<uint8_t>(color) == 0x56);
  };

  "rgba"_test = []{
    auto color = color_util::make_32bit(0xCC123456);
    expect(color_util::alpha_channel(color) == 0xCCCC);
    expect(color_util::red_channel<uint16_t>(color) == 0x1212);
    expect(color_util::red_channel<uint8_t>(color) == 0x12);
    expect(color_util::green_channel<uint16_t>(color) == 0x3434);
    expect(color_util::blue_channel<uint16_t>(color) == 0x5656);
  };

  "hex"_test = [] {
    auto colorA = color_util::make_24bit(0x123456);
    expect(color_util::hex(colorA).compare("#123456") == 0);
    auto colorB = color_util::make_32bit(0xCC123456);
    expect(color_util::hex(colorB).compare("#CC123456") == 0);
  };
}
