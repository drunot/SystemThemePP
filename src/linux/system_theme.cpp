#include "linux_theme_interface/linux_theme_interface.hpp"
#include "gnome/gnome_theme.hpp"
#include <system_theme_pp/system_theme.hpp>
namespace system_theme_pp {

static LinuxThemeInterface* theme = nullptr;

void SystemTheme::getCurrentThemeName(wchar_t *buffer,
                                      size_t bufferSize) const {
  theme->getCurrentThemeName(buffer, bufferSize);
}

bool SystemTheme::isDarkMode() const {
  return theme->isDarkMode();
}

ThemeColors SystemTheme::getForegroundColor() const {
  return theme->getForegroundColor();
}

ThemeColors SystemTheme::getBackgroundColor() const {
  return theme->getBackgroundColor();
}

ThemeColors SystemTheme::getAccentColor() const {
  return theme->getAccentColor();
}

void SystemTheme::setThemeChangeCallback(ThemeChangeCallback callback,
                                         void *data) {
  themeChangeCallback = callback;
  themeChangeCallback_data = data;
  theme->setThemeChangeCallback(callback, data);
}

void SystemTheme::removeThemeChangeCallback() {
  themeChangeCallback = nullptr;
  themeChangeCallback_data = nullptr;
  theme->removeThemeChangeCallback();
}

void SystemTheme::getSystemDefaultFont(wchar_t *buffer,
                                       size_t bufferSize) const {
  theme->getSystemDefaultFont(buffer, bufferSize);
}

float SystemTheme::getSystemDefaultFontScale() const {
  return theme->getSystemDefaultFontScale();
}

SystemTheme::SystemTheme(){
  theme = new GnomeTheme();
}
SystemTheme::~SystemTheme() = default;
} // namespace system_theme_pp