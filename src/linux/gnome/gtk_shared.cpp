#include "gtk_shared.hpp"
namespace system_theme_pp::gtk {
std::string getCurrentGtkThemeName() {
  if (!schemaExists("org.gnome.desktop.interface"))
    return "";
  GSettings *settings = g_settings_new("org.gnome.desktop.interface");
  gchar *name = g_settings_get_string(settings, "gtk-theme");
  g_object_unref(settings);
  if (!name)
    return "";
  std::string result(name);
  g_free(name);
  return result;
}

// Helper to force GTK to load a specific theme by setting GTK_THEME env var
// and resetting the style context
bool forceGtkThemeReload() {
  std::string themeName = getCurrentGtkThemeName();
  if (!themeName.empty()) {
    setenv("GTK_THEME", themeName.c_str(), 1);
    return true;
  }
  return false;
}

std::wstring getThemeName(std::wstring_view gtkVersion) {
  if (schemaExists("org.gnome.desktop.interface")) {
    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    gchar *themeName = g_settings_get_string(settings, "gtk-theme");
    g_object_unref(settings);
    if (themeName) {
      std::wstringstream wss;
      wss << L"/usr/share/themes/" << themeName << L"/gtk-" << gtkVersion
          << L"/";
      auto &theme = system_theme_pp::SystemTheme::getInstance();
      if (theme.isDarkMode()) {
        wss << L"gtk-dark.css";
      } else {
        wss << L"gtk.css";
      }
      return wss.str();
    }
  }
  return L"Unknown Theme";
}
}