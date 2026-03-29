#ifndef FD9366B4_C10B_43BB_B884_2476555045AD
#define FD9366B4_C10B_43BB_B884_2476555045AD

#include "../dbus_theme/dbus_theme.hpp"
// #include "gtk3_theme.hpp"
// #include "gtk4_theme.hpp"

#include <cstdint>
#include <cstdlib>
#include <memory>

#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>
#include <gio/gio.h>

namespace system_theme_pp {

    class SYSTEM_THEME_PP_PRIVATE GnomeTheme : public DBusTheme {
      public:

        GnomeTheme();
        ~GnomeTheme() override;
        virtual ThemeColors getBackgroundColor() const override;
        virtual ThemeColors getForegroundColor() const override;
        float               getSystemDefaultFontScale() const override;
        void                getSystemDefaultFont(wchar_t* buffer, size_t bufferSize) const override;
        virtual void        getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const override;

      private:

        static void on_theme_changed(GSettings* settings, gchar* key, gpointer user_data);
        void setup_theme_change_listener();
        void teardown_theme_change_listener();
        // std::unique_ptr<system_theme_pp::gtk::GTKThemeInterface> gtkTheme;

        gulong themeChangedSignalId = 0;
    };
}  // namespace system_theme_pp

#endif /* FD9366B4_C10B_43BB_B884_2476555045AD */
