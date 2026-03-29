#ifndef C37DCF1F_545C_46CF_A8C4_1D3D5322BD41
#define C37DCF1F_545C_46CF_A8C4_1D3D5322BD41

#include <system_theme_pp/core.hpp>

namespace system_theme_pp {
    struct ThemeColors {
        int r;
        int g;
        int b;
    };

    struct SYSTEM_THEME_PP_API SystemThemeInfo {
        wchar_t     themeName[256];
        bool        isDarkMode;
        ThemeColors foregroundColor;
        ThemeColors backgroundColor;
        ThemeColors accentColor;
        wchar_t     systemDefaultFont[512];
        float       systemDefaultFontScale;
    };

    using ThemeChangeCallback = void (*)(const SystemThemeInfo&, void*);

}  // namespace system_theme_pp
#endif /* C37DCF1F_545C_46CF_A8C4_1D3D5322BD41 */
