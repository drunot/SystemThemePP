#ifndef FD9366B4_C10B_43BB_B884_2476555045AD
#define FD9366B4_C10B_43BB_B884_2476555045AD

#include "../dbus_theme/dbus_theme.hpp"
#include "gtk3_theme.hpp"
#include "gtk4_theme.hpp"

#include <cstdint>
#include <cstdlib>
#include <memory>

#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>

namespace system_theme_pp {

    class SYSTEM_THEME_PP_PRIVATE GnomeTheme : public DBusTheme {
      public:

        GnomeTheme();
        virtual ThemeColors getBackgroundColor() const override;
        virtual ThemeColors getForegroundColor() const override;
        float               getSystemDefaultFontScale() const override;
        void                getSystemDefaultFont(wchar_t* buffer, size_t bufferSize) const override;
        virtual void        getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const override;

      protected:

        virtual void internalOnThemeChanged() override;

      private:

        std::unique_ptr<system_theme_pp::gtk::GTKThemeInterface> gtkTheme;
    };
}  // namespace system_theme_pp

#endif /* FD9366B4_C10B_43BB_B884_2476555045AD */
