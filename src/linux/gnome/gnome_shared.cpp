#include "gnome_shared.hpp"
#include <sstream>
#include <system_theme_pp/system_theme.hpp>

namespace system_theme_pp {

    // Helper: check if a GSettings schema exists
    bool schemaExists(const char* schemaId) {
        GSettingsSchemaSource* source = g_settings_schema_source_get_default();
        GSettingsSchema*       schema = g_settings_schema_source_lookup(source, schemaId, TRUE);
        if(schema) {
            g_settings_schema_unref(schema);
            return true;
        }
        return false;
    }

    bool forceGtkThemeReload() {
        std::string themeName = getCurrentThemeName();
        if(!themeName.empty()) {
            setenv("GTK_THEME", themeName.c_str(), 1);
            return true;
        }
        return false;
    }

    std::wstring getThemeName(std::wstring_view gtkVersion) {
        if(schemaExists("org.gnome.desktop.interface")) {
            GSettings* settings  = g_settings_new("org.gnome.desktop.interface");
            gchar*     themeName = g_settings_get_string(settings, "gtk-theme");
            g_object_unref(settings);
            if(themeName) {
                std::wstringstream wss;
                wss << L"/usr/share/themes/" << themeName << L"/gtk-" << gtkVersion << L"/";
                auto& theme = system_theme_pp::SystemTheme::getInstance();
                if(theme.isDarkMode()) {
                    wss << L"gtk-dark.css";
                } else {
                    wss << L"gtk.css";
                }
                return wss.str();
            }
        }
        return L"Unknown Theme";
    }

    std::string getCurrentThemeName() {
        if(!schemaExists("org.gnome.desktop.interface")) return "";
        GSettings* settings = g_settings_new("org.gnome.desktop.interface");
        gchar*     name     = g_settings_get_string(settings, "gtk-theme");
        g_object_unref(settings);
        if(!name) return "";
        std::string result(name);
        g_free(name);
        return result;
    }
}  // namespace system_theme_pp