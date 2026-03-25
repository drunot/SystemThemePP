#include "gnome_theme.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <fontconfig/fontconfig.h>
#include <fstream>
#include <gio/gio.h>
#include <iostream>
#include <regex>
#include <string>
#include <system_theme_pp/system_theme.hpp>
#include <vector>

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

void SystemTheme::getCurrentThemeName(wchar_t *buffer,
                                      size_t bufferSize) const {
  const wchar_t *themeName = L"Default Theme";
  std::wcsncpy(buffer, themeName, bufferSize - 1);
  buffer[bufferSize - 1] = L'\0'; // Ensure null-termination
}

bool SystemTheme::isDarkMode() const {
  if (schemaExists("org.gnome.desktop.interface")) {
    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    gchar *colorScheme = g_settings_get_string(settings, "color-scheme");
    bool dark = (colorScheme && strstr(colorScheme, "dark") != nullptr);
    g_free(colorScheme);
    g_object_unref(settings);
    return dark;
  }
  // Default to light mode if schema not found.
  return false;
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
    auto &gnomeTheme = GnomeTheme::getInstance();
    return gnomeTheme.gnomeGetColor("accent-color");
}

void SystemTheme::setThemeChangeCallback(ThemeChangeCallback callback,
                                         void *data) {}

void SystemTheme::removeThemeChangeCallback() {}

void SystemTheme::getSystemDefaultFont(wchar_t *buffer,
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

float SystemTheme::getSystemDefaultFontScale() const {
  if (schemaExists("org.gnome.desktop.interface")) {
    GSettings *settings = g_settings_new("org.gnome.desktop.interface");
    gdouble scale = g_settings_get_double(settings, "text-scaling-factor");
    g_object_unref(settings);
    return (float)scale;
  }
  return 1.0f;
}

SystemTheme::SystemTheme() = default;
SystemTheme::~SystemTheme() = default;
} // namespace system_theme_pp