#ifndef A96FE5B1_E1A4_4CFF_AD1B_15486C606BE5
#define A96FE5B1_E1A4_4CFF_AD1B_15486C606BE5

#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>
#include "gtk_theme_interface.hpp"

namespace system_theme_pp::gtk {

class SYSTEM_THEME_PP_PRIVATE GTK3Theme : public GTKThemeInterface {
public:
  GTK3Theme();
  GTK3Theme(void*_handler);
  ~GTK3Theme() override;

  static void* GTK3CheckLoaded();
  static void* loadGTK3();

  bool isInitialized() const;
  bool initCheck() const;
  bool lookupNamedColor(const char *colorName, Gdk3RGBACompat *color) const;
  ThemeColors getBackgroundColor() const override;
  ThemeColors getForegroundColor() const override;
  void getCurrentThemeName(wchar_t *buffer,
                                      size_t bufferSize) const override;

private:
  
  void getFunctions();
  void ResetGTKStyleContext() const;

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

  using gtk_style_context_reset_widgets_t = void (*)(void *);
  gtk_style_context_reset_widgets_t gtk_style_context_reset_widgets = nullptr;

  using g_main_context_iteration_t = bool (*)(void *, bool);
  g_main_context_iteration_t g_main_context_iteration = nullptr;

  void *handle = nullptr;
};


}

#endif /* A96FE5B1_E1A4_4CFF_AD1B_15486C606BE5 */
