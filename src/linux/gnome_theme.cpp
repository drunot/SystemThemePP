#include "gnome_theme.hpp"
#include <cstring>
#include <dbus/dbus.h>
#include <dlfcn.h>
#include <iostream>

namespace system_theme_pp {

#define LOAD_SYM(name)                                                         \
  name = (name##_t)dlsym(handle, #name);                                       \
  if (!name)                                                                   \
    std::cerr << "[GTK3Theme] Missing symbol: " #name "\n";

const GnomeTheme &GnomeTheme::getInstance() {
  static GTK3Theme instance;
  return instance;
}

ThemeColors GTK3Theme::getBackgroundColor() const {
  if (!isInitialized()) {
    initCheck(); // ignore failure, let lookupNamedColor handle it
  }

  auto color = Gdk3RGBACompat{};
  // Try modern names first, fall back to legacy
  const char *names[] = {"window_bg_color", "theme_base_color",
                         "theme_bg_color", nullptr};
  for (const char **name = names; *name != nullptr; name++) {
    if (lookupNamedColor(*name, &color)) {
      return {(uint8_t)(color.red * 255), (uint8_t)(color.green * 255),
              (uint8_t)(color.blue * 255)};
    }
  }
  // Fall back to portal color-scheme to at least get light/dark right
  return ThemeColors{0, 0, 0};
}

ThemeColors GTK3Theme::getForegroundColor() const {
  if (!isInitialized()) {
    initCheck(); // ignore failure, let lookupNamedColor handle it
  }

  auto color = Gdk3RGBACompat{};
  const char *names[] = {"window_fg_color", "theme_text_color",
                         "theme_fg_color", nullptr};
  for (const char **name = names; *name != nullptr; name++) {
    if (lookupNamedColor(*name, &color)) {
      return {(uint8_t)(color.red * 255), (uint8_t)(color.green * 255),
              (uint8_t)(color.blue * 255)};
    }
  }
  return ThemeColors{255, 255, 255};
}
// ThemeColors GTK3Theme::getAccentColor() const {

//   // Try to initialize if not already — best effort,
//   // lookupNamedColor will fail gracefully if there's no display
//   if (!isInitialized()) {
//     initCheck(); // ignore failure, let lookupNamedColor handle it
//   }

//   Gdk3RGBACompat color{};
//   const char *colorNames[] = {"accent_bg_color", "theme_selected_bg_color",
//                               nullptr};
//   for (const char **name = colorNames; *name != nullptr; name++) {
//     if (lookupNamedColor(*name, &color)) {
//       return {(uint8_t)(color.red * 255), (uint8_t)(color.green * 255),
//               (uint8_t)(color.blue * 255)};
//     }
//   }

//   return {53, 132, 228};
// }

bool GTK3Theme::isInitialized() const {
  return gdk_display_get_default() != nullptr;
}

bool GTK3Theme::initCheck() const { return gtk_init_check(nullptr, nullptr); }

bool GTK3Theme::lookupNamedColor(const char *colorName,
                                 Gdk3RGBACompat *color) const {
  void *display = gdk_display_get_default();
  if (!display) {
    return false;
  }
  void *ctx = nullptr;
  // GTK3: standalone style context works fine
  if (!gtk_style_context_new) {
    return false;
  }
  ctx = gtk_style_context_new();
  if (!ctx) {
    return false;
  }

  bool found = gtk_style_context_lookup_color(ctx, colorName, color);
  g_object_unref(ctx);
  return found;
}

GTK3Theme::GTK3Theme() {
  const char *gtk3libs[] = {"libgtk-3.so.0", "libgtk-3.so", nullptr};
  if (!handle) {
    for (const char **lib = gtk3libs; *lib != nullptr; lib++) {
      handle = dlopen(*lib, RTLD_LAZY | RTLD_NOLOAD);
      if (handle)
        break;
    }
  }
  if (!handle) {
    for (const char **lib = gtk3libs; *lib != nullptr; lib++) {
      handle = dlopen(*lib, RTLD_LAZY | RTLD_LOCAL);
      if (handle)
        break;
    }
  }

  LOAD_SYM(gdk_display_get_default);
  LOAD_SYM(gtk_init_check);
  LOAD_SYM(gtk_style_context_new);
  LOAD_SYM(gtk_style_context_lookup_color);
  LOAD_SYM(g_object_unref);
}

// ThemeColors AdwaitaTheme::getAccentColor() const {
//     auto *manager = adw_style_manager_get_default();
//       if (!manager) {
//         std::cerr << "[AdwaitaTheme] Failed to get Adwaita style manager\n";
//     return {53, 132, 228};
//   }

//     auto accent = adw_style_manager_get_accent_color(manager);

//     auto rgba = Gdk4RGBACompat{};
//     adw_accent_color_to_rgba(accent, &rgba);

//     return {(uint8_t)(rgba.red * 255), (uint8_t)(rgba.green * 255),
//             (uint8_t)(rgba.blue * 255)};
// }

AdwaitaTheme::AdwaitaTheme() {
  const char *adwaitalibs[] = {"libadwaita-1.so.0", "libadwaita-1.so", nullptr};
  for (const char **lib = adwaitalibs; *lib != nullptr; lib++) {
    handle = dlopen(*lib, RTLD_LAZY | RTLD_NOLOAD);
    if (handle)
      break;
  }
  if (!handle) {
    for (const char **lib = adwaitalibs; *lib != nullptr; lib++) {
      handle = dlopen(*lib, RTLD_LAZY | RTLD_LOCAL);
      if (handle)
        break;
    }
  }

  LOAD_SYM(adw_style_manager_get_default);
  LOAD_SYM(adw_style_manager_get_accent_color);
  LOAD_SYM(adw_accent_color_to_rgba);
}

ThemeColors AdwaitaTheme::getBackgroundColor() const {
  return ThemeColors{30, 30, 30};
}

ThemeColors AdwaitaTheme::getForegroundColor() const {
  return ThemeColors{255, 255, 255};
}

ThemeColors GnomeTheme::getAccentColor() const {
  // Use xdg-desktop-portal D-Bus Settings interface
  // org.freedesktop.portal.Settings.Read("org.freedesktop.appearance",
  // "accent-color") Returns (r, g, b) as doubles in a variant

  DBusError err;
  dbus_error_init(&err);

  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err) || !conn) {
    dbus_error_free(&err);
    return {53, 132, 228};
  }

  DBusMessage *msg = dbus_message_new_method_call(
      "org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop",
      "org.freedesktop.portal.Settings", "Read");
  if (!msg)
    return {53, 132, 228};

  const char *ns = "org.freedesktop.appearance";
  const char *key = "accent-color";
  dbus_message_append_args(msg, DBUS_TYPE_STRING, &ns, DBUS_TYPE_STRING, &key,
                           DBUS_TYPE_INVALID);

  DBusMessage *reply =
      dbus_connection_send_with_reply_and_block(conn, msg, 1000, &err);
  dbus_message_unref(msg);

  if (dbus_error_is_set(&err) || !reply) {
    dbus_error_free(&err);
    return {53, 132, 228};
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

#undef LOAD_SYM

} // namespace system_theme_pp