
// clang-format off
#include <windows.h>
#include <uxtheme.h>
#include <shlobj.h> 
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>
// clang-format on
#include <iostream>

#include <system_theme_pp/system_theme.hpp>

namespace system_theme_pp {

    static struct winrt::Windows::UI::ViewManagement::UISettings settings;

    SystemTheme::SystemTheme() {
        // Initialize any necessary resources here
        settings = winrt::Windows::UI::ViewManagement::UISettings();
    }

    void SystemTheme::getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const {
        // Get the current theme color scheme
        GetCurrentThemeName(buffer, static_cast<int>(bufferSize), nullptr, 0, nullptr, 0);
    }

    bool SystemTheme::isDarkMode() const {
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

    ThemeColors SystemTheme::getForegroundColor() const {
        ThemeColors colors;
        auto        color = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
        colors.r          = color.R;
        colors.g          = color.G;
        colors.b          = color.B;
        return colors;
    }

    ThemeColors SystemTheme::getBackgroundColor() const {
        ThemeColors colors;

        auto color = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Background);
        colors.r   = color.R;
        colors.g   = color.G;
        colors.b   = color.B;

        return colors;
    }

    ThemeColors SystemTheme::getAccentColor() const {
        ThemeColors colors;

        auto color = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
        colors.r   = color.R;
        colors.g   = color.G;
        colors.b   = color.B;

        return colors;
    }

    static struct winrt::event_token colorChangeRevoker;
    static bool                      colorChangeHooked = false;
    static struct winrt::event_token textScaleChangeRevoker;
    static bool                      textScaleChangeHooked = false;
    void                             SystemTheme::setThemeChangeCallback(ThemeChangeCallback callback, void* data) {
        themeChangeCallback      = callback;
        themeChangeCallback_data = data;
        // You would need to set up a Windows event hook here to listen for theme changes
        // and call the callback when a change is detected.
        colorChangeRevoker       = settings.ColorValuesChanged(
            winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::UI::ViewManagement::UISettings,
                                                                                            winrt::Windows::Foundation::IInspectable>(
                [this](winrt::Windows::UI::ViewManagement::UISettings const&,
                       winrt::Windows::Foundation::IInspectable const&) {
                    if(themeChangeCallback) {
                        SystemThemeInfo info = getCurrentThemeInfo();
                        themeChangeCallback(info, themeChangeCallback_data);
                    }
                }));
        colorChangeHooked = true;

        textScaleChangeRevoker = settings.TextScaleFactorChanged(
            winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::UI::ViewManagement::UISettings,
                                                                                      winrt::Windows::Foundation::IInspectable>(
                [this](winrt::Windows::UI::ViewManagement::UISettings const&,
                       winrt::Windows::Foundation::IInspectable const&) {
                    if(themeChangeCallback) {
                        SystemThemeInfo info = getCurrentThemeInfo();
                        themeChangeCallback(info, themeChangeCallback_data);
                    }
                }));

        textScaleChangeHooked = true;
    };

    void SystemTheme::removeThemeChangeCallback() {
        if(colorChangeHooked) {
            settings.ColorValuesChanged(colorChangeRevoker);
            colorChangeHooked = false;
        }
        if(textScaleChangeHooked) {
            settings.TextScaleFactorChanged(textScaleChangeRevoker);
            textScaleChangeHooked = false;
        }

        themeChangeCallback      = nullptr;
        themeChangeCallback_data = nullptr;
    }

    void SystemTheme::getSystemDefaultFont(wchar_t* buffer, size_t bufferSize) const {
        NONCLIENTMETRICSW metrics = {};
        metrics.cbSize            = sizeof(metrics);

        wchar_t faceName[LF_FACESIZE] = {};
        if(SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0)) {
            wcsncpy_s(faceName, metrics.lfMessageFont.lfFaceName, _TRUNCATE);
        } else {
            wcsncpy_s(faceName, L"Segoe UI", _TRUNCATE);
        }

        // First check registry for font file name
        wchar_t regValue[MAX_PATH] = {};
        DWORD   regSize            = sizeof(regValue);
        wchar_t regKey[MAX_PATH]   = {};
        swprintf_s(regKey, L"%s (TrueType)", faceName);

        LONG result = RegGetValueW(HKEY_LOCAL_MACHINE,
                                   L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
                                   regKey,
                                   RRF_RT_REG_SZ,
                                   nullptr,
                                   regValue,
                                   &regSize);

        if(result != ERROR_SUCCESS) {
            // fallback to Segoe UI
            wcsncpy_s(regValue, L"segoeui.ttf", _TRUNCATE);
        }

        // Build full path to font file
        wchar_t fontsDir[MAX_PATH] = {};
        SHGetFolderPathW(nullptr, CSIDL_FONTS, nullptr, 0, fontsDir);
        swprintf_s(buffer, bufferSize, L"%s\\%s", fontsDir, regValue);
    }

    float SystemTheme::getSystemDefaultFontScale() const {
        return settings.TextScaleFactor();
    }

    SystemTheme::~SystemTheme() {
        removeThemeChangeCallback();
    }
}  // namespace system_theme_pp