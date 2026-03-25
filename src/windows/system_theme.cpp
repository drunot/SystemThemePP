
// clang-format off
#include <windows.h>
#include <uxtheme.h>
#include <vssym32.h>
#include <dwmapi.h>
// clang-format on
#include <iostream>

#include <system_theme_pp/system_theme.hpp>

namespace system_theme_pp {
    void getCurrentThemeName(wchar_t* buffer, size_t bufferSize) {
        // Get the current theme color scheme
        GetCurrentThemeName(buffer, static_cast<int>(bufferSize), nullptr, 0, nullptr, 0);
    }

    bool isDarkMode() {
        DWORD value  = 0;
        DWORD size   = sizeof(value);
        LONG  result = RegGetValueW(HKEY_CURRENT_USER,
                                   L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                                   L"AppsUseLightTheme",
                                   RRF_RT_REG_DWORD,
                                   nullptr,
                                   &value,
                                   &size);

        if(result != ERROR_SUCCESS) {
            return false;  // assume light mode if registry key not found
        }

        return value == 0;  // 0 = dark mode, 1 = light mode
    }

    ThemeColors getForegroundColor() {
        ThemeColors colors;

        HTHEME hTheme = OpenThemeData(nullptr, L"TEXTSTYLE");
        if(hTheme) {
            COLORREF color = 0;
            if(SUCCEEDED(GetThemeColor(hTheme, TEXT_BODYTEXT, 1, TMT_TEXTCOLOR, &color))) {
                std::cout << "Got foreground color from theme: " << std::hex << color << std::dec << std::endl;
                colors.r = GetRValue(color);
                colors.g = GetGValue(color);
                colors.b = GetBValue(color);
                CloseThemeData(hTheme);
                return colors;
            }
            CloseThemeData(hTheme);
        }

        // fallback
        colors.r = isDarkMode() ? 255 : 0;
        colors.g = isDarkMode() ? 255 : 0;
        colors.b = isDarkMode() ? 255 : 0;
        return colors;
    }

    ThemeColors getBackgroundColor() {
        ThemeColors colors;

        HTHEME hTheme = OpenThemeData(nullptr, L"WINDOW");
        if(hTheme) {
            COLORREF color = 0;
            if(SUCCEEDED(GetThemeColor(hTheme, 1, 1, TMT_FILLCOLOR, &color))) {
                std::cout << "Got background color from theme: " << std::hex << color << std::dec << std::endl;
                colors.r = GetRValue(color);
                colors.g = GetGValue(color);
                colors.b = GetBValue(color);
                CloseThemeData(hTheme);
                return colors;
            }
            CloseThemeData(hTheme);
        }

        // fallback
        colors.r = isDarkMode() ? 32 : 255;
        colors.g = isDarkMode() ? 32 : 255;
        colors.b = isDarkMode() ? 32 : 255;
        return colors;
    }

    ThemeColors getAccentColor() {
        ThemeColors colors;

        DWORD   color  = 0;
        BOOL    opaque = FALSE;
        HRESULT result = DwmGetColorizationColor(&color, &opaque);

        if(FAILED(result)) {
            colors.r = 0;
            colors.g = 0;
            colors.b = 0;
            return colors;
        }

        // color is in 0xAARRGGBB format
        colors.r = (color >> 16) & 0xFF;
        colors.g = (color >> 8) & 0xFF;
        colors.b = (color) & 0xFF;

        return colors;
    }
}  // namespace system_theme_pp