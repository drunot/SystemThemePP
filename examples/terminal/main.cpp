#include <iostream>

#include <system_theme_pp/system_theme.hpp>

void themeChangeCallback(const system_theme_pp::SystemThemeInfo& info, void* data) {
    std::wcout << L"Theme changed: " << info.themeName << L", Dark mode: " << (info.isDarkMode ? "Yes" : "No") << L"\n";
    std::wcout << L"Foreground: RGB(" << info.foregroundColor.r << ", " << info.foregroundColor.g << ", "
               << info.foregroundColor.b << L")\n";
    std::wcout << L"Background: RGB(" << info.backgroundColor.r << ", " << info.backgroundColor.g << ", "
               << info.backgroundColor.b << L")\n";
    std::wcout << L"Accent:     RGB(" << info.accentColor.r << ", " << info.accentColor.g << ", " << info.accentColor.b
               << L")\n";
}

int main() {

    std::cout << "Press enter to exit...\n";

    auto theme = system_theme_pp::SystemTheme::getInstance();

    system_theme_pp::SystemThemeInfo info;
    theme.getCurrentThemeName(info.themeName, sizeof(info.themeName) / sizeof(wchar_t));
    info.isDarkMode      = theme.isDarkMode();
    info.foregroundColor = theme.getForegroundColor();
    info.backgroundColor = theme.getBackgroundColor();
    info.accentColor     = theme.getAccentColor();
    theme.setThemeChangeCallback(themeChangeCallback);

    themeChangeCallback(info, nullptr);

    for(;;) {
        if(std::cin.get() == '\n') {
            break;
        }
    }

    return 0;
}