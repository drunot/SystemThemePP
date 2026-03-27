#ifndef D5D88805_C503_4E5B_BDD7_E7B12E0D6C8D
#define D5D88805_C503_4E5B_BDD7_E7B12E0D6C8D

#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>
#include <cstdlib>

namespace system_theme_pp {
    class SYSTEM_THEME_PP_PRIVATE LinuxThemeInterface {
        public:
            

            virtual void getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const = 0;

            virtual bool isDarkMode() const = 0;

            virtual ThemeColors getForegroundColor() const = 0;
            virtual ThemeColors getBackgroundColor() const = 0;
            virtual ThemeColors getAccentColor() const = 0;

            virtual void setThemeChangeCallback(ThemeChangeCallback callback, void* data = nullptr) = 0;

            virtual void removeThemeChangeCallback() = 0;

            virtual void  getSystemDefaultFont(wchar_t* buffer, size_t bufferSize) const = 0;
            virtual float getSystemDefaultFontScale() const = 0;

            virtual ~LinuxThemeInterface() = default;

    };
}

#endif /* D5D88805_C503_4E5B_BDD7_E7B12E0D6C8D */
