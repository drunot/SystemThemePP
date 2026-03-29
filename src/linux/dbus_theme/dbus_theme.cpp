#include "dbus_theme.hpp"

#include "dbus_handler.hpp"

#include <iostream>

#include <system_theme_pp/system_theme.hpp>

namespace system_theme_pp {

    bool DBusTheme::isDarkMode() const {
        auto& dbusHandler = DebusHandler::getInstance();
        return dbusHandler.isDarkMode();
    }

    ThemeColors DBusTheme::getAccentColor() const {
        auto& dbusHandler = DebusHandler::getInstance();
        return dbusHandler.getAccentColor();
    }

    void DBusTheme::setThemeChangeCallback(ThemeChangeCallback callback, void* data) {
        auto& dbusHandler = DebusHandler::getInstance();
        dbusHandler.setThemeChangeCallback([this]() { internalOnThemeChanged(); });
    }

    void DBusTheme::removeThemeChangeCallback() {
        auto& dbusHandler = DebusHandler::getInstance();
        dbusHandler.removeThemeChangeCallback();
    }

    DBusTheme::~DBusTheme() = default;
    void DBusTheme::internalOnThemeChanged() {}
}  // namespace system_theme_pp