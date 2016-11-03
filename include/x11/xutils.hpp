#pragma once

#include <X11/Xlib-xcb.h>
#include <xcb/xcb_util.h>

#include "common.hpp"

LEMONBUDDY_NS

namespace xutils {
  xcb_connection_t* get_connection();

  void pack_values(uint32_t mask, const uint32_t* src, uint32_t* dest);
  void pack_values(uint32_t mask, const xcb_params_cw_t* src, uint32_t* dest);
  void pack_values(uint32_t mask, const xcb_params_gc_t* src, uint32_t* dest);
}

LEMONBUDDY_NS_END
