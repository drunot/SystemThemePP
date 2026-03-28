#ifndef AAA28150_7EE7_4A73_9EAF_AA960305B4D6
#define AAA28150_7EE7_4A73_9EAF_AA960305B4D6

#include "gtk_types.hpp"

#include <cstdint>
#include <cstdlib>
#include <memory>

#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>

namespace system_theme_pp::gtk {
    class SYSTEM_THEME_PP_PRIVATE GTKThemeInterface {
      public:

        virtual ThemeColors getBackgroundColor() const                                    = 0;
        virtual ThemeColors getForegroundColor() const                                    = 0;
        virtual void        getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const = 0;
        virtual void        ResetGTKStyleContext() const                                  = 0;

        virtual ~GTKThemeInterface() = default;
    };
}  // namespace system_theme_pp::gtk

#endif /* AAA28150_7EE7_4A73_9EAF_AA960305B4D6 */
