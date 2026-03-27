#ifndef B3A8B356_FEAD_4ABB_B94A_DF02F58FFC73
#define B3A8B356_FEAD_4ABB_B94A_DF02F58FFC73

#include <gio/gio.h>
#include <string>
#include <sstream>
#include "gnome_shared.hpp"
#include <system_theme_pp/system_theme.hpp>

namespace system_theme_pp::gtk {
#define LOAD_SYM(name)                                                         \
  name = (name##_t)dlsym(handle, #name);                                       \
  if (!name)                                                                   \
    std::cerr << "[GTK3Theme] Missing symbol: " #name "\n";

std::string getCurrentGtkThemeName();

// Helper to force GTK to load a specific theme by setting GTK_THEME env var
// and resetting the style context
bool forceGtkThemeReload();

std::wstring getThemeName(std::wstring_view gtkVersion);
} // namespace system_theme_pp::gtk


#endif /* B3A8B356_FEAD_4ABB_B94A_DF02F58FFC73 */
