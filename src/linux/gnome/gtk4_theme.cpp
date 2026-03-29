#include "gtk4_theme.hpp"

#include "gtk_shared.hpp"

#include <dlfcn.h>

namespace system_theme_pp::gtk {

    void* GTK4Theme::GTKCheckLoaded() {
        const char* gtk4libs[] = {"libgtk-4.so.0", "libgtk-4.so", nullptr};
        for(const char** lib = gtk4libs; *lib != nullptr; lib++) {
            auto handle = dlopen(*lib, RTLD_LAZY | RTLD_NOLOAD);
            if(handle) {
                return handle;
            }
        }
        return nullptr;
    }

    void* GTK4Theme::loadGTK() {
        const char* gtk4libs[] = {"libgtk-4.so.0", "libgtk-4.so", nullptr};
        for(const char** lib = gtk4libs; *lib != nullptr; lib++) {
            auto handle = dlopen(*lib, RTLD_LAZY | RTLD_LOCAL);
            if(handle) {
                return handle;
            }
        }
        return nullptr;
    }

    GTK4Theme::GTK4Theme() {

        const char* gtk4libs[] = {"libgtk-4.so.0", "libgtk-4.so", nullptr};
        if(!handle) {
            handle = GTKCheckLoaded();
        }
        if(!handle) {
            handle = loadGTK();
        }
        getFunctions();
    }

    GTK4Theme::GTK4Theme(void* _handler) {
        handle = _handler;
        getFunctions();
    }

    void GTK4Theme::getFunctions() {
        LOAD_SYM(gdk_display_get_default);
        LOAD_SYM(gtk_init);
        LOAD_SYM(gtk_init_check);
        LOAD_SYM(gtk_window_new);
        LOAD_SYM(gtk_widget_get_style_context);
        LOAD_SYM(gtk_style_context_lookup_color);
        LOAD_SYM(gtk_window_destroy);
        LOAD_SYM(g_object_unref);
        LOAD_SYM(gtk_style_context_reset_widgets);
        LOAD_SYM(g_main_context_iteration);
        LOAD_SYM(gtk_widget_realize);
        LOAD_SYM(gtk_widget_show);
        LOAD_SYM(gtk_widget_queue_draw);
    }

    ThemeColors GTK4Theme::getBackgroundColor() const {
        if(!handle || !gtk_window_new || !gtk_style_context_lookup_color) return {30, 30, 30};

        if(gdk_display_get_default && !gdk_display_get_default()) {
            gtk_init_check();
        }

        // GTK4: need a real widget to get a valid style context
        void* win = gtk_window_new();
        if(!win) {
            return {30, 30, 30};
        }

        ResetGTKStyleContext();
        // Realize and show the window to ensure style is applied
        gtk_widget_realize(win);
        gtk_widget_show(win);
        gtk_widget_queue_draw(win);

        void* ctx   = gtk_widget_get_style_context(win);
        auto  color = Gdk4RGBACompat{};

        const char* names[] = {"window_bg_color", "theme_bg_color", nullptr};
        for(const char** name = names; *name != nullptr; name++) {
            if(ctx && gtk_style_context_lookup_color(ctx, *name, &color)) {
                gtk_window_destroy(win);
                return {(uint8_t) (color.red * 255), (uint8_t) (color.green * 255), (uint8_t) (color.blue * 255)};
            }
        }

        gtk_window_destroy(win);
        return {30, 30, 30};
    }

    ThemeColors GTK4Theme::getForegroundColor() const {
        if(!handle || !gtk_window_new || !gtk_style_context_lookup_color) return {30, 30, 30};

        if(gdk_display_get_default && !gdk_display_get_default()) {
            gtk_init_check();
        }

        ResetGTKStyleContext();
        // GTK4: need a real widget to get a valid style context
        void* win = gtk_window_new();
        if(!win) {
            return {30, 30, 30};
        }
        // Realize and show the window to ensure style is applied
        gtk_widget_realize(win);
        gtk_widget_show(win);
        gtk_widget_queue_draw(win);

        void* ctx   = gtk_widget_get_style_context(win);
        auto  color = Gdk4RGBACompat{};

        const char* names[] = {"window_fg_color", "theme_fg_color", nullptr};
        for(const char** name = names; *name != nullptr; name++) {
            if(ctx && gtk_style_context_lookup_color(ctx, *name, &color)) {
                gtk_window_destroy(win);
                return {(uint8_t) (color.red * 255), (uint8_t) (color.green * 255), (uint8_t) (color.blue * 255)};
            }
        }

        gtk_window_destroy(win);
        return ThemeColors{255, 255, 255};
    }

    void GTK4Theme::getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const {
        std::wstring themePath = getThemeName(L"4.0");
        std::wcsncpy(buffer, themePath.c_str(), bufferSize - 1);
        buffer[bufferSize - 1] = L'\0';
    }

    void GTK4Theme::ResetGTKStyleContext() const {
        forceGtkThemeReload();
        while(g_main_context_iteration(nullptr, false));

        if(gtk_style_context_reset_widgets) {
            gtk_style_context_reset_widgets();
        }
    }

    GTK4Theme::~GTK4Theme(){
        
    }

}  // namespace system_theme_pp::gtk