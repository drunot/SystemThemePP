
// clang-format off
#include <windows.h>
#include <uxtheme.h>
#include <vssym32.h>
#include <dwmapi.h>
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

    static struct winrt::event_token themeChangeRevoker;
    static bool                      themeChangeHooked = false;
    void                             SystemTheme::setThemeChangeCallback(ThemeChangeCallback callback, void* data) {
        themeChangeCallback      = callback;
        themeChangeCallback_data = data;
        // You would need to set up a Windows event hook here to listen for theme changes
        // and call the callback when a change is detected.
        themeChangeRevoker       = settings.ColorValuesChanged(
            winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::UI::ViewManagement::UISettings,
                                                                                            winrt::Windows::Foundation::IInspectable>(
                [this](winrt::Windows::UI::ViewManagement::UISettings const&,
                       winrt::Windows::Foundation::IInspectable const&) {
                    if(themeChangeCallback) {
                        SystemThemeInfo info;
                        getCurrentThemeName(info.themeName, sizeof(info.themeName) / sizeof(wchar_t));
                        info.isDarkMode      = isDarkMode();
                        info.foregroundColor = getForegroundColor();
                        info.backgroundColor = getBackgroundColor();
                        info.accentColor     = getAccentColor();
                        themeChangeCallback(info, themeChangeCallback_data);
                    }
                }));
        themeChangeHooked = true;
    };

    void SystemTheme::removeThemeChangeCallback() {
        if(themeChangeHooked) {
            settings.ColorValuesChanged(themeChangeRevoker);
            themeChangeHooked = false;
        }
        themeChangeCallback      = nullptr;
        themeChangeCallback_data = nullptr;
    }

    SystemTheme::~SystemTheme() {
        removeThemeChangeCallback();
    }
}  // namespace system_theme_pp