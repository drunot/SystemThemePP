#include "gnome_theme.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <dbus/dbus.h>
#include <fstream>
#include <thread>
#include <atomic>
#include <iostream>
#include <regex>
#include <string>
#include <system_theme_pp/system_theme.hpp>
#include <vector>

namespace system_theme_pp {

DBusMessage *getApperance(const char *key) {
  DBusError err;
  dbus_error_init(&err);

  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err) || !conn) {
    dbus_error_free(&err);
    return nullptr;
  }

  DBusMessage *msg = dbus_message_new_method_call(
      "org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop",
      "org.freedesktop.portal.Settings", "Read");
  if (!msg)
    return nullptr;

  const char *ns = "org.freedesktop.appearance";
  dbus_message_append_args(msg, DBUS_TYPE_STRING, &ns, DBUS_TYPE_STRING, &key,
                           DBUS_TYPE_INVALID);

  DBusMessage *reply =
      dbus_connection_send_with_reply_and_block(conn, msg, 1000, &err);
  dbus_message_unref(msg);

  if (dbus_error_is_set(&err) || !reply) {
    dbus_error_free(&err);
    return nullptr;
  }

  return reply;
}

void SystemTheme::getCurrentThemeName(wchar_t *buffer,
                                      size_t bufferSize) const {
  auto &gnomeTheme = GnomeTheme::getInstance();
  gnomeTheme.getCurrentThemeName(buffer, bufferSize);
}

bool SystemTheme::isDarkMode() const {
  auto reply = getApperance("color-scheme");
  if (!reply) {
    return false;
  }

  DBusMessageIter iter, variant1, variant2;
  dbus_message_iter_init(reply, &iter);

  // Unwrap outer variant
  if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_VARIANT) {
    dbus_message_unref(reply);
    return false;
  }
  dbus_message_iter_recurse(&iter, &variant1);

  // Unwrap inner variant
  if (dbus_message_iter_get_arg_type(&variant1) != DBUS_TYPE_VARIANT) {
    dbus_message_unref(reply);
    return false;
  }
  dbus_message_iter_recurse(&variant1, &variant2);

  if (dbus_message_iter_get_arg_type(&variant2) != DBUS_TYPE_UINT32) {
    dbus_message_unref(reply);
    return false;
  }

  uint32_t colorScheme;
  dbus_message_iter_get_basic(&variant2, &colorScheme);
  dbus_message_unref(reply);

  // Per portal spec: 0=no preference, 1=dark, 2=light
  return colorScheme == 1;
}

ThemeColors SystemTheme::getForegroundColor() const {
  auto &gnomeTheme = GnomeTheme::getInstance();
  return gnomeTheme.getForegroundColor();
}

ThemeColors SystemTheme::getBackgroundColor() const {
  auto &gnomeTheme = GnomeTheme::getInstance();
  return gnomeTheme.getBackgroundColor();
}

ThemeColors SystemTheme::getAccentColor() const {

  auto reply = getApperance("accent-color");
  if (!reply) {
    return {53, 132, 228}; // default blue accent
  }

  // Reply is v(v(r,g,b)) — outer variant wraps inner variant of struct (ddd)
  DBusMessageIter iter, variant1, variant2, struct_iter;
  dbus_message_iter_init(reply, &iter);
  if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_VARIANT) {
    dbus_message_unref(reply);
    return {53, 132, 228};
  }
  dbus_message_iter_recurse(&iter, &variant1);
  if (dbus_message_iter_get_arg_type(&variant1) != DBUS_TYPE_VARIANT) {
    dbus_message_unref(reply);
    return {53, 132, 228};
  }
  dbus_message_iter_recurse(&variant1, &variant2);
  if (dbus_message_iter_get_arg_type(&variant2) != DBUS_TYPE_STRUCT) {
    dbus_message_unref(reply);
    return {53, 132, 228};
  }
  dbus_message_iter_recurse(&variant2, &struct_iter);

  double r = 0, g = 0, b = 0;
  dbus_message_iter_get_basic(&struct_iter, &r);
  dbus_message_iter_next(&struct_iter);
  dbus_message_iter_get_basic(&struct_iter, &g);
  dbus_message_iter_next(&struct_iter);
  dbus_message_iter_get_basic(&struct_iter, &b);

  dbus_message_unref(reply);

  return {(uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)};
}

static DBusConnection *signalConn = nullptr;
static std::thread signalThread;
static std::atomic<bool> signalThreadRunning{false};

static void signalThreadFunc() {
  while (signalThreadRunning) {
    dbus_connection_read_write(signalConn, 100);
    DBusMessage *msg = dbus_connection_pop_message(signalConn);
    if (!msg) continue;

    if (dbus_message_is_signal(msg, "org.freedesktop.portal.Settings",
                               "SettingChanged")) {
        auto& systemTheme = SystemTheme::getInstance();
        systemTheme.callCallback();
    }
    dbus_message_unref(msg);
  }
}

void SystemTheme::setThemeChangeCallback(ThemeChangeCallback callback,
                                         void *data) {
  themeChangeCallback = callback;
  themeChangeCallback_data = data;

  if (signalThreadRunning) return;

  DBusError err;
  dbus_error_init(&err);
  signalConn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (!signalConn || dbus_error_is_set(&err)) {
    dbus_error_free(&err);
    return;
  }

  dbus_bus_add_match(signalConn,
    "type='signal',"
    "interface='org.freedesktop.portal.Settings',"
    "member='SettingChanged',"
    "path='/org/freedesktop/portal/desktop'",
    &err);
  dbus_connection_flush(signalConn);

  if (dbus_error_is_set(&err)) {
    dbus_error_free(&err);
    return;
  }



  signalThreadRunning = true;
  signalThread = std::thread(signalThreadFunc);
}

void SystemTheme::removeThemeChangeCallback() {
  themeChangeCallback = nullptr;
  themeChangeCallback_data = nullptr;
  signalThreadRunning = false;
  if (signalThread.joinable())
    signalThread.join();
}

void SystemTheme::getSystemDefaultFont(wchar_t *buffer,
                                       size_t bufferSize) const {
  auto &gnomeTheme = GnomeTheme::getInstance();
  gnomeTheme.getSystemDefaultFont(buffer, bufferSize);
}

float SystemTheme::getSystemDefaultFontScale() const {
  auto &gnomeTheme = GnomeTheme::getInstance();
  return gnomeTheme.getSystemDefaultFontScale();
}

SystemTheme::SystemTheme() = default;
SystemTheme::~SystemTheme(){
  if(signalThreadRunning) {
    signalThreadRunning = false;
    if(signalThread.joinable()) {
        signalThread.join();
    }
  }
};
} // namespace system_theme_pp