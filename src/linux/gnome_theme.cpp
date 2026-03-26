#include "gnome_theme.hpp"
#include <cstring>
#include <gio/gio.h>
#include <fontconfig/fontconfig.h>
#include <dlfcn.h>
#include <iostream>
#include <sstream>
#include <system_theme_pp/system_theme.hpp>

namespace system_theme_pp {

  // Helper: check if a GSettings schema exists
  static bool schemaExists(const char *schemaId) {
    GSettingsSchemaSource *source = g_settings_schema_source_get_default();
    GSettingsSchema *schema =
        g_settings_schema_source_lookup(source, schemaId, TRUE);
    if (schema) {
      g_settings_schema_unref(schema);
      return true;
    }
    return false;
  }

  static std::string getCurrentGtkThemeName() {
    if (!schemaExists("org.gnome.desktop.interface"))
      return "";
    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    gchar *name = g_settings_get_string(settings, "gtk-theme");
    g_object_unref(settings);
    if (!name) return "";
    std::string result(name);
    g_free(name);
    return result;
  }

  // Helper to force GTK to load a specific theme by setting GTK_THEME env var
  // and resetting the style context
  static bool forceGtkThemeReload() {
    std::string themeName = getCurrentGtkThemeName();
    if (!themeName.empty()) {
    setenv("GTK_THEME", themeName.c_str(), 1);
    return true;
    }
    return false;
  }


#define LOAD_SYM(name)                                                         \
  name = (name##_t)dlsym(handle, #name);                                       \
  if (!name)                                                                   \
    std::cerr << "[GTK3Theme] Missing symbol: " #name "\n";

const GnomeTheme &GnomeTheme::getInstance() {
  static GnomeTheme* instance = nullptr;
  if (!instance) {
    // Try GTK4 first since it's newer, but fall back to GTK3 if not available
    void* gtkHandle = GTK4Theme::GTK4CheckLoaded();
    if (gtkHandle) {
      instance = new GTK4Theme(gtkHandle);
    } else {
      gtkHandle = GTK3Theme::GTK3CheckLoaded();
      if (gtkHandle) {
        instance = new GTK3Theme(gtkHandle);
      } else {
        gtkHandle = GTK3Theme::loadGTK3();
        if (gtkHandle) {
          instance = new GTK3Theme(gtkHandle);
        } else {
          gtkHandle = GTK4Theme::loadGTK4();
          if (gtkHandle) {
            instance = new GTK4Theme(gtkHandle);
          } else {
            throw std::runtime_error("Failed to load GTK3 or GTK4 libraries");
          }
        }
      }
    }
  }



  return *instance;
}

float GnomeTheme::getSystemDefaultFontScale() const {
  if (schemaExists("org.gnome.desktop.interface")) {
    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    gdouble scale = g_settings_get_double(settings, "text-scaling-factor");
    g_object_unref(settings);
    return (float)scale;
  }
  return 1.0f;
}

void GnomeTheme::getSystemDefaultFont(wchar_t *buffer,
                                       size_t bufferSize) const {
  // Try GSettings first (GNOME)
  if (schemaExists("org.gnome.desktop.interface")) {
    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    gchar *fontName = g_settings_get_string(settings, "font-name");
    g_object_unref(settings);

    if (fontName) {
      // font-name is like "Cantarell 11", strip the size
      gchar *lastSpace = strrchr(fontName, ' ');
      if (lastSpace) {
        *lastSpace = '\0';
      }

      // Now use fontconfig to get the actual file path
      FcPattern *pattern = FcNameParse((FcChar8 *)fontName);
      g_free(fontName);

      FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
      FcDefaultSubstitute(pattern);
      FcResult result;
      FcPattern *match = FcFontMatch(nullptr, pattern, &result);
      FcPatternDestroy(pattern);

      FcChar8 *file = nullptr;
      if (match &&
          FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch) {
        mbstowcs(buffer, (char *)file, bufferSize - 1);
        buffer[bufferSize - 1] = L'\0';
        FcPatternDestroy(match);
        return;
      }
      if (match) {
        FcPatternDestroy(match);
      }
    }
  }

  // Fallback: use fontconfig default sans font
  FcPattern *pattern = FcNameParse((FcChar8 *)"sans");
  FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
  FcDefaultSubstitute(pattern);
  FcResult result;
  FcPattern *match = FcFontMatch(nullptr, pattern, &result);
  FcPatternDestroy(pattern);

  FcChar8 *file = nullptr;
  if (match && FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch) {
    mbstowcs(buffer, (char *)file, bufferSize - 1);
    buffer[bufferSize - 1] = L'\0';
  }
  if (match) {
    FcPatternDestroy(match);
  }
}

void *GTK3Theme::GTK3CheckLoaded() {
  const char *gtk3libs[] = {"libgtk-3.so.0", "libgtk-3.so", nullptr};
  for (const char **lib = gtk3libs; *lib != nullptr; lib++) {
    auto handle = dlopen(*lib, RTLD_LAZY | RTLD_NOLOAD);
    if (handle) {
      return handle;
    }
  }
  return nullptr;
}

void *GTK3Theme::loadGTK3() {
  const char *gtk3libs[] = {"libgtk-3.so.0", "libgtk-3.so", nullptr};
  for (const char **lib = gtk3libs; *lib != nullptr; lib++) {
    auto handle = dlopen(*lib, RTLD_LAZY | RTLD_LOCAL);
    if (handle) {
      return handle;
    }
  }
  return nullptr;
}

GTK3Theme::GTK3Theme() {
  const char *gtk3libs[] = {"libgtk-3.so.0", "libgtk-3.so", nullptr};
  if (!handle) {
    handle = GTK3CheckLoaded();
  }
  if (!handle) {
    handle = loadGTK3();
  }

  getFunctions();
}

GTK3Theme::GTK3Theme(void *_handler) {
  handle = _handler;
  getFunctions();
}

void GTK3Theme::getFunctions() {
  LOAD_SYM(gdk_display_get_default);
  LOAD_SYM(gtk_init_check);
  LOAD_SYM(gtk_style_context_new);
  LOAD_SYM(gtk_style_context_lookup_color);
  LOAD_SYM(g_object_unref);
  LOAD_SYM(gtk_style_context_reset_widgets);
  LOAD_SYM(g_main_context_iteration);
}

ThemeColors GTK3Theme::getBackgroundColor() const {
  if (!isInitialized()) {
    initCheck(); // ignore failure, let lookupNamedColor handle it
  }

  forceGtkThemeReload();
  ResetGTKStyleContext();
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

  forceGtkThemeReload();
  ResetGTKStyleContext();
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


std::wstring getThemeName(std::wstring_view gtkVersion) {
      if(schemaExists("org.gnome.desktop.interface")) {
        GSettings *settings = g_settings_new("org.gnome.desktop.interface");
        gchar *themeName = g_settings_get_string(settings, "gtk-theme");
        g_object_unref(settings);
        if(themeName) {
            std::wstringstream wss;
            wss << L"/usr/share/themes/" << themeName << L"/gtk-" << gtkVersion << L"/";
            auto& theme = SystemTheme::getInstance();
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

void GTK3Theme::getCurrentThemeName(wchar_t *buffer,
                                      size_t bufferSize) const {
  std::wstring themePath = getThemeName(L"3.0");
  std::wcsncpy(buffer, themePath.c_str(), bufferSize - 1);
  buffer[bufferSize - 1] = L'\0';
}


void GTK3Theme::ResetGTKStyleContext() const {
  while (g_main_context_iteration(nullptr, FALSE));
  if (gtk_style_context_reset_widgets) {
    gtk_style_context_reset_widgets(nullptr);
  }
}

void *GTK4Theme::GTK4CheckLoaded() {
  const char *gtk4libs[] = {"libgtk-4.so.0", "libgtk-4.so", nullptr};
  for (const char **lib = gtk4libs; *lib != nullptr; lib++) {
    auto handle = dlopen(*lib, RTLD_LAZY | RTLD_NOLOAD);
    if (handle) {
      return handle;
    }
  }
  return nullptr;
}

void *GTK4Theme::loadGTK4() {
  const char *gtk4libs[] = {"libgtk-4.so.0", "libgtk-4.so", nullptr};
  for (const char **lib = gtk4libs; *lib != nullptr; lib++) {
    auto handle = dlopen(*lib, RTLD_LAZY | RTLD_LOCAL);
    if (handle) {
      return handle;
    }
  }
  return nullptr;
}

GTK4Theme::GTK4Theme() {

  const char *gtk4libs[] = {"libgtk-4.so.0", "libgtk-4.so", nullptr};
  if (!handle) {
    handle = GTK4CheckLoaded();
  }
  if (!handle) {
    handle = loadGTK4();
  }
  getFunctions();
}

GTK4Theme::GTK4Theme(void *_handler) {
  handle = _handler;
  getFunctions();
}

void GTK4Theme::getFunctions() {
  LOAD_SYM(gdk_display_get_default);
  LOAD_SYM(gtk_init);
  LOAD_SYM(gtk_init_check);
  LOAD_SYM(gtk_window_new);
  LOAD_SYM(gtk_widget_get_style_context);
  LOAD_SYM(gtk_style_context_lookup_color);
  LOAD_SYM(gtk_window_destroy);
  LOAD_SYM(g_object_unref);
  LOAD_SYM(gtk_style_context_reset_widgets);
  LOAD_SYM(g_main_context_iteration);
}

ThemeColors GTK4Theme::getBackgroundColor() const {
  if (!handle || !gtk_window_new || !gtk_style_context_lookup_color)
    return {30, 30, 30};

  if (gdk_display_get_default && !gdk_display_get_default()) {
    gtk_init_check();
  }

  forceGtkThemeReload();
  ResetGTKStyleContext();
  // GTK4: need a real widget to get a valid style context
  void *win = gtk_window_new();
  if (!win) {
    return {30, 30, 30};
  }

  void *ctx = gtk_widget_get_style_context(win);
  auto color = Gdk4RGBACompat{};

  const char *names[] = {"window_bg_color", "theme_bg_color", nullptr};
  for (const char **name = names; *name != nullptr; name++) {
    if (ctx && gtk_style_context_lookup_color(ctx, *name, &color)) {
      gtk_window_destroy(win);
      return {(uint8_t)(color.red * 255), (uint8_t)(color.green * 255),
              (uint8_t)(color.blue * 255)};
    }
  }

  gtk_window_destroy(win);
  return {30, 30, 30};
}

ThemeColors GTK4Theme::getForegroundColor() const {
  if (!handle || !gtk_window_new || !gtk_style_context_lookup_color)
    return {30, 30, 30};

  if (gdk_display_get_default && !gdk_display_get_default()) {
    gtk_init_check();
  }

  forceGtkThemeReload();
  ResetGTKStyleContext();
  // GTK4: need a real widget to get a valid style context
  void *win = gtk_window_new();
  if (!win) {
    return {30, 30, 30};
  }

  void *ctx = gtk_widget_get_style_context(win);
  auto color = Gdk4RGBACompat{};

  const char *names[] = {"window_fg_color", "theme_fg_color", nullptr};
  for (const char **name = names; *name != nullptr; name++) {
    if (ctx && gtk_style_context_lookup_color(ctx, *name, &color)) {
      gtk_window_destroy(win);
      return {(uint8_t)(color.red * 255), (uint8_t)(color.green * 255),
              (uint8_t)(color.blue * 255)};
    }
  }

  gtk_window_destroy(win);
  return ThemeColors{255, 255, 255};
}


void GTK4Theme::getCurrentThemeName(wchar_t *buffer,
                                      size_t bufferSize) const {
  std::wstring themePath = getThemeName(L"4.0");
  std::wcsncpy(buffer, themePath.c_str(), bufferSize - 1);
  buffer[bufferSize - 1] = L'\0';
}

void GTK4Theme::ResetGTKStyleContext() const {
  while (g_main_context_iteration(nullptr, FALSE));
  if (gtk_style_context_reset_widgets) {
    gtk_style_context_reset_widgets();
  }
}

#undef LOAD_SYM

} // namespace system_theme_pp