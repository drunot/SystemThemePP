#include <iostream>

#include <system_theme_pp/system_theme.hpp>
namespace system_theme_pp {
    SystemTheme& SystemTheme::getInstance() {
        static SystemTheme instance;
        return instance;
    }

    SystemThemeInfo SystemTheme::getCurrentThemeInfo() const {
        SystemThemeInfo info;
        getCurrentThemeName(info.themeName, sizeof(info.themeName) / sizeof(wchar_t));
        info.isDarkMode      = isDarkMode();
        info.foregroundColor = getForegroundColor();
        info.backgroundColor = getBackgroundColor();
        info.accentColor     = getAccentColor();
        getSystemDefaultFont(info.systemDefaultFont, sizeof(info.systemDefaultFont) / sizeof(wchar_t));
        info.systemDefaultFontScale = getSystemDefaultFontScale();
        return info;
    }

    bool SystemTheme::callCallback() const {

        if(themeChangeCallback) {
            SystemThemeInfo info = getCurrentThemeInfo();
            themeChangeCallback(info, themeChangeCallback_data);
            return true;
        }
        return false;
    }
}  // namespace system_theme_pp