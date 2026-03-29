#include "gtk3_theme.hpp"

#include "gtk_shared.hpp"

#include <dlfcn.h>

namespace system_theme_pp::gtk {

    void* GTK3Theme::GTKCheckLoaded() {
        const char* gtk3libs[] = {"libgtk-3.so.0", "libgtk-3.so", nullptr};
        for(const char** lib = gtk3libs; *lib != nullptr; lib++) {
            auto handle = dlopen(*lib, RTLD_LAZY | RTLD_NOLOAD);
            if(handle) {
                return handle;
            }
        }
        return nullptr;
    }

    void* GTK3Theme::loadGTK() {
        const char* gtk3libs[] = {"libgtk-3.so.0", "libgtk-3.so", nullptr};
        for(const char** lib = gtk3libs; *lib != nullptr; lib++) {
            auto handle = dlopen(*lib, RTLD_LAZY | RTLD_LOCAL);
            if(handle) {
                return handle;
            }
        }
        return nullptr;
    }

    GTK3Theme::GTK3Theme() {
        const char* gtk3libs[] = {"libgtk-3.so.0", "libgtk-3.so", nullptr};
        if(!handle) {
            handle = GTKCheckLoaded();
        }
        if(!handle) {
            handle = loadGTK();
        }

        getFunctions();
    }

    GTK3Theme::GTK3Theme(void* _handler) {
        handle = _handler;
        getFunctions();
    }

    void GTK3Theme::getFunctions() {
        LOAD_SYM(gdk_display_get_default);
        LOAD_SYM(gtk_init_check);
        LOAD_SYM(gtk_style_context_new);
        LOAD_SYM(gtk_style_context_lookup_color);
        LOAD_SYM(g_object_unref);
        LOAD_SYM(gtk_style_context_reset_widgets);
        LOAD_SYM(g_main_context_iteration);
    }

    ThemeColors GTK3Theme::getBackgroundColor() const {
        if(!isInitialized()) {
            initCheck();  // ignore failure, let lookupNamedColor handle it
        }

        auto color          = Gdk3RGBACompat{};
        // Try modern names first, fall back to legacy
        const char* names[] = {"window_bg_color", "theme_base_color", "theme_bg_color", nullptr};
        for(const char** name = names; *name != nullptr; name++) {
            if(lookupNamedColor(*name, &color)) {
                return {(uint8_t) (color.red * 255), (uint8_t) (color.green * 255), (uint8_t) (color.blue * 255)};
            }
        }
        // Fall back to portal color-scheme to at least get light/dark right
        return ThemeColors{0, 0, 0};
    }

    ThemeColors GTK3Theme::getForegroundColor() const {
        if(!isInitialized()) {
            initCheck();  // ignore failure, let lookupNamedColor handle it
        }

        auto        color   = Gdk3RGBACompat{};
        const char* names[] = {"window_fg_color", "theme_text_color", "theme_fg_color", nullptr};
        for(const char** name = names; *name != nullptr; name++) {
            if(lookupNamedColor(*name, &color)) {
                return {(uint8_t) (color.red * 255), (uint8_t) (color.green * 255), (uint8_t) (color.blue * 255)};
            }
        }
        return ThemeColors{255, 255, 255};
    }

    bool GTK3Theme::isInitialized() const {
        return gdk_display_get_default() != nullptr;
    }

    bool GTK3Theme::initCheck() const {
        return gtk_init_check(nullptr, nullptr);
    }

    bool GTK3Theme::lookupNamedColor(const char* colorName, Gdk3RGBACompat* color) const {
        void* display = gdk_display_get_default();
        if(!display) {
            return false;
        }
        void* ctx = nullptr;
        // GTK3: standalone style context works fine
        if(!gtk_style_context_new) {
            return false;
        }
        ctx = gtk_style_context_new();
        if(!ctx) {
            return false;
        }

        bool found = gtk_style_context_lookup_color(ctx, colorName, color);
        g_object_unref(ctx);
        return found;
    }

    void GTK3Theme::getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const {
        std::wstring themePath = getThemeName(L"3.0");
        std::wcsncpy(buffer, themePath.c_str(), bufferSize - 1);
        buffer[bufferSize - 1] = L'\0';
    }

    void GTK3Theme::ResetGTKStyleContext() const {
        forceGtkThemeReload();
        for(int i = 0; i < 5; i++) {
            if(!g_main_context_iteration(nullptr, false)) {
                break;
            }
        }
        if(gtk_style_context_reset_widgets) {
            gtk_style_context_reset_widgets(nullptr);
        }
    }

    GTK3Theme::~GTK3Theme() = default;

}  // namespace system_theme_pp::gtk