#ifndef A8C945D3_C7C0_4779_BD16_18EB0B157179
#define A8C945D3_C7C0_4779_BD16_18EB0B157179

#include "../linux_theme_interface/linux_theme_interface.hpp"

namespace system_theme_pp {
    class SYSTEM_THEME_PP_PRIVATE DBusTheme : public LinuxThemeInterface {
      public:

        virtual void getCurrentThemeName(wchar_t* buffer, size_t bufferSize) const = 0;

        virtual bool isDarkMode() const override;

        virtual ThemeColors getForegroundColor() const = 0;
        virtual ThemeColors getBackgroundColor() const = 0;
        virtual ThemeColors getAccentColor() const;

        virtual void setThemeChangeCallback(ThemeChangeCallback callback, void* data = nullptr) override;
        virtual void removeThemeChangeCallback() override;

        virtual void  getSystemDefaultFont(wchar_t* buffer, size_t bufferSize) const = 0;
        virtual float getSystemDefaultFontScale() const                              = 0;

        virtual ~DBusTheme();

      protected:

        virtual void internalOnThemeChanged();

    };
}  // namespace system_theme_pp

#endif /* A8C945D3_C7C0_4779_BD16_18EB0B157179 */
