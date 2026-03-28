#include "gnome_theme.hpp"

#include "gnome_shared.hpp"
#include "gtk3_theme.hpp"
#include "gtk4_theme.hpp"

#include <cstring>
#include <sstream>

#include <fontconfig/fontconfig.h>
#include <gio/gio.h>
#include <system_theme_pp/system_theme.hpp>

namespace system_theme_pp {

    GnomeTheme::GnomeTheme() {
        // Try GTK4 first since it's newer, but fall back to GTK3 if not available
        void* gtkHandle = gtk::GTK4Theme::GTK4CheckLoaded();
        if(gtkHandle) {
            gtkTheme = std::make_unique<gtk::GTK4Theme>(gtkHandle);
        } else {
            gtkHandle = gtk::GTK3Theme::GTK3CheckLoaded();
            if(gtkHandle) {
                gtkTheme = std::make_unique<gtk::GTK3Theme>(gtkHandle);
            } else {
                gtkHandle = gtk::GTK3Theme::loadGTK3();
                if(gtkHandle) {
                    gtkTheme = std::make_unique<gtk::GTK3Theme>(gtkHandle);
                } else {
                    gtkHandle = gtk::GTK4Theme::loadGTK4();
                    if(gtkHandle) {
                        gtkTheme = std::make_unique<gtk::GTK4Theme>(gtkHandle);
                    } else {
                        throw std::runtime_error("Failed to load GTK3 or GTK4 libraries");
                    }
                }
            }
        }
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
        if(!gtkTheme) return {30, 30, 30};

        return gtkTheme->getBackgroundColor();
    }

    ThemeColors GnomeTheme::getForegroundColor() const {
        if(!gtkTheme) return {30, 30, 30};

        return gtkTheme->getForegroundColor();
    }

    void GnomeTheme::getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const {
        if(!gtkTheme) {
            std::wcsncpy(buffer, L"Unknown", bufferSize - 1);
            buffer[bufferSize - 1] = L'\0';
            return;
        }

        gtkTheme->getCurrentThemeName(buffer, bufferSize);
    }

    void GnomeTheme::internalOnThemeChanged() {
        auto& systemTheme     = SystemTheme::getInstance();
        auto  systemThemeInfo = systemTheme.getCurrentThemeInfo();
        gtkTheme->ResetGTKStyleContext();
    }

}  // namespace system_theme_pp