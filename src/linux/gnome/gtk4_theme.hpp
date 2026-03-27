#ifndef DDDB195C_AF99_40AD_AB1F_81DEDB0F46A0
#define DDDB195C_AF99_40AD_AB1F_81DEDB0F46A0

#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>
#include "gtk_theme_interface.hpp"

namespace system_theme_pp::gtk {
class SYSTEM_THEME_PP_PRIVATE GTK4Theme : public GTKThemeInterface {
public:
  GTK4Theme();
  GTK4Theme(void*_handler);
  ~GTK4Theme() override;

  static void* GTK4CheckLoaded();
  static void* loadGTK4();

  ThemeColors getBackgroundColor() const override;
  ThemeColors getForegroundColor() const override;

  void getCurrentThemeName(wchar_t *buffer,
                                      size_t bufferSize) const override;
private:

  void getFunctions();
  void ResetGTKStyleContext() const;

  using gdk_display_get_default_t = void *(*)();
  gdk_display_get_default_t gdk_display_get_default = nullptr;

  using gtk_init_t = void (*)();
  gtk_init_t gtk_init = nullptr;

  using gtk_init_check_t = bool (*)();
  gtk_init_check_t gtk_init_check = nullptr;

  using gtk_window_new_t = void *(*)();
  gtk_window_new_t gtk_window_new = nullptr;

  using gtk_widget_get_style_context_t = void *(*)(void *widget);
  gtk_widget_get_style_context_t gtk_widget_get_style_context = nullptr;

  using gtk_style_context_lookup_color_t = bool (*)(void *, const char *,
                                                    Gdk4RGBACompat *);
  gtk_style_context_lookup_color_t gtk_style_context_lookup_color = nullptr;

  using gtk_window_destroy_t = void (*)(void *widget);
  gtk_window_destroy_t gtk_window_destroy = nullptr;

  using g_object_unref_t = void (*)(void *);
  g_object_unref_t g_object_unref = nullptr;

  using gtk_style_context_reset_widgets_t = void (*)();
  gtk_style_context_reset_widgets_t gtk_style_context_reset_widgets = nullptr;

  using g_main_context_iteration_t = bool (*)(void *, bool);
  g_main_context_iteration_t g_main_context_iteration = nullptr;

  void *handle = nullptr;

};



}

#endif /* DDDB195C_AF99_40AD_AB1F_81DEDB0F46A0 */
