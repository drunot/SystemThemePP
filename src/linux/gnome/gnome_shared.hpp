#ifndef DEFE0CC7_B71D_4BCE_90A1_4A856D26ADA3
#define DEFE0CC7_B71D_4BCE_90A1_4A856D26ADA3

#include <string>

#include <gio/gio.h>

namespace system_theme_pp {

    // Helper: check if a GSettings schema exists
    bool schemaExists(const char* schemaId);


    bool forceGtkThemeReload();

    std::wstring getThemeName(std::wstring_view gtkVersion);

    std::string getCurrentThemeName();

}  // namespace system_theme_pp
#endif /* DEFE0CC7_B71D_4BCE_90A1_4A856D26ADA3 */
