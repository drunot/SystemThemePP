#ifndef E406110C_0236_43AF_AF02_300231E54175
#define E406110C_0236_43AF_AF02_300231E54175

#include <cstdlib>

#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>


namespace system_theme_pp {

    // Add public member functions and variables here
    class SYSTEM_THEME_PP_API SystemTheme {
      public:

        ~SystemTheme();

        static SystemTheme& getInstance();

        void getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const;

        bool isDarkMode() const;

        ThemeColors getForegroundColor() const;
        ThemeColors getBackgroundColor() const;
        ThemeColors getAccentColor() const;

        void setThemeChangeCallback(ThemeChangeCallback callback, void* data = nullptr);

        void removeThemeChangeCallback();

        void  getSystemDefaultFont(wchar_t* buffer, size_t bufferSize) const;
        float getSystemDefaultFontScale() const;

        SystemThemeInfo getCurrentThemeInfo() const;

        bool callCallback() const;

      private:

        void*               themeChangeCallback_data = nullptr;
        ThemeChangeCallback themeChangeCallback      = nullptr;
        SystemTheme();
        SystemTheme(const SystemTheme&) = delete;
        SystemTheme& operator=(const SystemTheme&) = delete;
    };
};  // namespace system_theme_pp

#endif /* E406110C_0236_43AF_AF02_300231E54175 */
