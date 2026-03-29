#include "gnome_theme.hpp"

#include "gnome_shared.hpp"

#include <chrono>
#include <cstring>
#include <filesystem>
#include <sstream>
#include <thread>

#include <fontconfig/fontconfig.h>
#include <gio/gio.h>
#include <gnome_css/gnome_css.h>
#include <system_theme_pp/system_theme.hpp>
namespace system_theme_pp {
    void GnomeTheme::on_theme_changed(GSettings* settings, gchar* key, gpointer user_data) {
        system_theme_pp::forceGtkThemeReload();  
    }
    void GnomeTheme::setup_theme_change_listener(){
        GSettings* settings = g_settings_new("org.gnome.desktop.interface");
        themeChangedSignalId = g_signal_connect(settings, "changed", G_CALLBACK(on_theme_changed), nullptr);
        // Keep 'settings' alive as long as you want to listen
    }
    void GnomeTheme::teardown_theme_change_listener() {
        GSettings* settings = g_settings_new("org.gnome.desktop.interface");
        if(themeChangedSignalId) {
            g_signal_handler_disconnect(settings, themeChangedSignalId);
            themeChangedSignalId = 0;
        }
    }

    // Callback function for the signal
    static void on_theme_changed(GSettings* settings, gchar* key, gpointer user_data) {
        system_theme_pp::forceGtkThemeReload();  
    }

    // Call this during initialization
    void setup_theme_change_listener() {
        GSettings* settings = g_settings_new("org.gnome.desktop.interface");
        g_signal_connect(settings, "changed", G_CALLBACK(on_theme_changed), nullptr);
        // Keep 'settings' alive as long as you want to listen
    }

    GnomeTheme::GnomeTheme() {
        // Try GTK4 first since it's newer, but fall back to GTK3 if not available
        // void* gtkHandle = gtk::GTK4Theme::GTKCheckLoaded();
        // if(gtkHandle) {
        //     gtkTheme = std::make_unique<gtk::GTK4Theme>(gtkHandle);
        // } else {
        //     gtkHandle = gtk::GTK3Theme::GTKCheckLoaded();
        //     if(gtkHandle) {
        //         gtkTheme = std::make_unique<gtk::GTK3Theme>(gtkHandle);
        //     } else {
        //         gtkHandle = gtk::GTK4Theme::loadGTK();
        //         if(gtkHandle) {
        //             gtkTheme = std::make_unique<gtk::GTK4Theme>(gtkHandle);
        //         } else {
        //             gtkHandle = gtk::GTK3Theme::loadGTK();
        //             if(gtkHandle) {
        //                 gtkTheme = std::make_unique<gtk::GTK3Theme>(gtkHandle);
        //             } else {
        //                 throw std::runtime_error("Failed to load GTK3 or GTK4 libraries");
        //             }
        //         }
        //     }
        // }
        setup_theme_change_listener();
    }

    GnomeTheme::~GnomeTheme() {
        teardown_theme_change_listener();
    }


    float GnomeTheme::getSystemDefaultFontScale() const {
        if(schemaExists("org.gnome.desktop.interface")) {
            GSettings* settings = g_settings_new("org.gnome.desktop.interface");
            gdouble    scale    = g_settings_get_double(settings, "text-scaling-factor");
            g_object_unref(settings);
            return (float) scale;
        }
        return 1.0f;
    }

    void GnomeTheme::getSystemDefaultFont(wchar_t* buffer, size_t bufferSize) const {
        // Try GSettings first (GNOME)
        if(schemaExists("org.gnome.desktop.interface")) {
            GSettings* settings = g_settings_new("org.gnome.desktop.interface");
            gchar*     fontName = g_settings_get_string(settings, "font-name");
            g_object_unref(settings);

            if(fontName) {
                // font-name is like "Cantarell 11", strip the size
                gchar* lastSpace = strrchr(fontName, ' ');
                if(lastSpace) {
                    *lastSpace = '\0';
                }

                // Now use fontconfig to get the actual file path
                FcPattern* pattern = FcNameParse((FcChar8*) fontName);
                g_free(fontName);

                FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
                FcDefaultSubstitute(pattern);
                FcResult   result;
                FcPattern* match = FcFontMatch(nullptr, pattern, &result);
                FcPatternDestroy(pattern);

                FcChar8* file = nullptr;
                if(match && FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch) {
                    mbstowcs(buffer, (char*) file, bufferSize - 1);
                    buffer[bufferSize - 1] = L'\0';
                    FcPatternDestroy(match);
                    return;
                }
                if(match) {
                    FcPatternDestroy(match);
                }
            }
        }

        // Fallback: use fontconfig default sans font
        FcPattern* pattern = FcNameParse((FcChar8*) "sans");
        FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
        FcDefaultSubstitute(pattern);
        FcResult   result;
        FcPattern* match = FcFontMatch(nullptr, pattern, &result);
        FcPatternDestroy(pattern);

        FcChar8* file = nullptr;
        if(match && FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch) {
            mbstowcs(buffer, (char*) file, bufferSize - 1);
            buffer[bufferSize - 1] = L'\0';
        }
        if(match) {
            FcPatternDestroy(match);
        }
    }

    ThemeColors GnomeTheme::getBackgroundColor() const {
        wchar_t themePath[512];
        getCurrentThemeName(themePath, sizeof(themePath) / sizeof(wchar_t));
        // Convert wchar_t themePath to std::string (UTF-8)
        std::wstring ws(themePath);
        std::string  themePathStr(ws.begin(), ws.end());

        auto color = get_color_from_css_c(themePathStr.c_str(), isDarkMode() ? "dark" : "light", "window_bg_color");
        if(color.valid) {
            return {color.r, color.g, color.b};
        }
        return isDarkMode() ? ThemeColors{0, 0, 0} : ThemeColors{255, 255, 255};
    }

    ThemeColors GnomeTheme::getForegroundColor() const {
        wchar_t themePath[512];
        getCurrentThemeName(themePath, sizeof(themePath) / sizeof(wchar_t));
        // Convert wchar_t themePath to std::string (UTF-8)
        std::wstring ws(themePath);
        std::string  themePathStr(ws.begin(), ws.end());

        auto color = get_color_from_css_c(themePathStr.c_str(), isDarkMode() ? "dark" : "light", "window_fg_color");
        if(color.valid) {
            return {color.r, color.g, color.b};
        }
        return isDarkMode() ? ThemeColors{255, 255, 255} : ThemeColors{30, 30, 30};
    }

    void GnomeTheme::getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const {
        std::wstring themePath = getThemeName(L"4.0");
        if(!std::filesystem::exists(themePath)) {
            themePath = getThemeName(L"3.0");
        }
        std::wcsncpy(buffer, themePath.c_str(), bufferSize - 1);
        buffer[bufferSize - 1] = L'\0';
    }

}  // namespace system_theme_pp