#pragma once

#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>

#include "common.hpp"
#include "components/logger.hpp"
#include "components/signals.hpp"
#include "components/types.hpp"
#include "utils/memory.hpp"
#include "utils/process.hpp"
#include "x11/connection.hpp"
#include "x11/xembed.hpp"

#define _NET_SYSTEM_TRAY_ORIENTATION_HORZ 0
#define _NET_SYSTEM_TRAY_ORIENTATION_VERT 1

#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2

#define TRAY_WM_NAME "Lemonbuddy tray window"
#define TRAY_WM_CLASS "tray\0Lemonbuddy"

LEMONBUDDY_NS

// class definition : trayclient {{{

class trayclient {
 public:
  explicit trayclient(connection& conn, xcb_window_t win);

  ~trayclient();

  bool match(const xcb_window_t& win) const;
  bool mapped() const;
  void mapped(bool state);

  xcb_window_t window() const;
  xembed_data* xembed() const;

  void configure_notify(int16_t x, int16_t y, uint16_t w, uint16_t h);

 protected:
  connection& m_connection;
  xcb_window_t m_window{0};
  shared_ptr<xembed_data> m_xembed;
  stateflag m_mapped{false};
};

// }}}
// class definition : traymanager {{{

class traymanager
    : public xpp::event::sink<evt::expose, evt::visibility_notify, evt::client_message,
          evt::configure_request, evt::resize_request, evt::selection_clear, evt::property_notify,
          evt::reparent_notify, evt::destroy_notify, evt::map_notify, evt::unmap_notify> {
 public:
  explicit traymanager(connection& conn, const logger& logger);

  ~traymanager();

  void bootstrap(tray_settings settings);
  void activate();
  void deactivate();
  void reconfigure();

 protected:
  void bar_visibility_change(bool state);

  int16_t calculate_x(uint32_t width) const;
  int16_t calculate_y() const;
  int calculate_client_xpos(const xcb_window_t& win);
  int calculate_client_ypos();

  shared_ptr<trayclient> find_client(const xcb_window_t& win);

  void query_atom();
  void create_window();
  void set_wmhints();
  void set_traycolors();

  void acquire_selection();
  void notify_clients();

  void track_selection_owner(xcb_window_t owner);
  void process_docking_request(xcb_window_t win);

  void handle(const evt::expose& evt);
  void handle(const evt::visibility_notify& evt);
  void handle(const evt::client_message& evt);
  void handle(const evt::configure_request& evt);
  void handle(const evt::resize_request& evt);
  void handle(const evt::selection_clear& evt);
  void handle(const evt::property_notify& evt);
  void handle(const evt::reparent_notify& evt);
  void handle(const evt::destroy_notify& evt);
  void handle(const evt::map_notify& evt);
  void handle(const evt::unmap_notify& evt);

 private:
  connection& m_connection;
  const logger& m_log;
  vector<shared_ptr<trayclient>> m_clients;

  tray_settings m_settings;

  xcb_atom_t m_atom{0};
  xcb_window_t m_tray{0};
  xcb_window_t m_othermanager{0};
  uint32_t m_lastwidth;

  stateflag m_activated{false};
  stateflag m_mapped{false};
  stateflag m_hidden{false};
  stateflag m_sinkattached{false};

  thread m_delayed_activation;

  bool m_restacked = false;
};

// }}}

namespace {
  /**
   * Configure injection module
   */
  template <class T = unique_ptr<traymanager>>
  di::injector<T> configure_traymanager() {
    return di::make_injector(configure_logger(), configure_connection());
  }
}

LEMONBUDDY_NS_END
