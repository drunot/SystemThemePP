#include <iostream>

#include <system_theme_pp/system_theme.hpp>

int main() {
    wchar_t buffer[256] = {0};
    system_theme_pp::getCurrentThemeName(buffer, sizeof(buffer) / sizeof(wchar_t));
    std::wcout << L"Current Theme: " << buffer << std::endl;

    bool darkMode = system_theme_pp::isDarkMode();
    std::cout << "Is Dark Mode: " << (darkMode ? "Yes" : "No") << std::endl;

    auto foregroundColor = system_theme_pp::getForegroundColor();
    std::cout << "Foreground Color: RGB(" << foregroundColor.r << ", " << foregroundColor.g << ", " << foregroundColor.b
              << ")" << std::endl;

    auto backgroundColor = system_theme_pp::getBackgroundColor();
    std::cout << "Background Color: RGB(" << backgroundColor.r << ", " << backgroundColor.g << ", " << backgroundColor.b
              << ")" << std::endl;

    auto accentColor = system_theme_pp::getAccentColor();
    std::cout << "Accent Color: RGB(" << accentColor.r << ", " << accentColor.g << ", " << accentColor.b << ")"
              << std::endl;

    return 0;
}