#ifndef FD9366B4_C10B_43BB_B884_2476555045AD
#define FD9366B4_C10B_43BB_B884_2476555045AD

#include <cstdint>
#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>

namespace system_theme_pp {

struct SYSTEM_THEME_PP_PRIVATE Gdk3RGBACompat {
  double red, green, blue, alpha;
};
struct SYSTEM_THEME_PP_PRIVATE Gdk4RGBACompat {
  float red, green, blue, alpha;
};
class SYSTEM_THEME_PP_PRIVATE GnomeTheme {
public:
  ThemeColors gnomeGetColor(const char *colorName) const;
  virtual ThemeColors getBackgroundColor() const = 0;
  virtual ThemeColors getForegroundColor() const = 0;
  static const GnomeTheme &getInstance();

};

class SYSTEM_THEME_PP_PRIVATE GTK3Theme : public GnomeTheme {
public:
  GTK3Theme();

  bool isInitialized() const;
  bool initCheck() const;
  bool lookupNamedColor(const char *colorName, Gdk3RGBACompat *color) const;
  ThemeColors getBackgroundColor() const override;
  ThemeColors getForegroundColor() const override;

private:
  using gdk_display_get_default_t = void *(*)();
  gdk_display_get_default_t gdk_display_get_default = nullptr;

  using gtk_init_check_t = bool (*)(int *, char ***);
  gtk_init_check_t gtk_init_check = nullptr;

  using gtk_style_context_new_t = void *(*)();
  gtk_style_context_new_t gtk_style_context_new = nullptr;

  using gtk_style_context_lookup_color_t = bool (*)(void *, const char *,
                                                    Gdk3RGBACompat *);
  gtk_style_context_lookup_color_t gtk_style_context_lookup_color = nullptr;

  using g_object_unref_t = void (*)(void *);
  g_object_unref_t g_object_unref = nullptr;

  void *handle = nullptr;
};

class SYSTEM_THEME_PP_PRIVATE AdwaitaTheme : public GnomeTheme {
public:
  AdwaitaTheme();
  
  ThemeColors getBackgroundColor() const override;
  ThemeColors getForegroundColor() const override;
private:
  using adw_style_manager_get_default_t = void *(*)();
  adw_style_manager_get_default_t adw_style_manager_get_default = nullptr;

  using adw_style_manager_get_accent_color_t = int (*)(void *);
  adw_style_manager_get_accent_color_t adw_style_manager_get_accent_color = nullptr;

  using adw_accent_color_to_rgba_t = void (*)(int, Gdk4RGBACompat *);
  adw_accent_color_to_rgba_t adw_accent_color_to_rgba = nullptr;

  void *handle = nullptr;
};
}

#endif /* FD9366B4_C10B_43BB_B884_2476555045AD */
