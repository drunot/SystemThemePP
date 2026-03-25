#ifndef E406110C_0236_43AF_AF02_300231E54175
#define E406110C_0236_43AF_AF02_300231E54175

#include <system_theme_pp/core.hpp>
#include <system_theme_pp/types.hpp>

namespace system_theme_pp {

    // Add public member functions and variables here
    SYSTEM_THEME_PP_API void getCurrentThemeName(wchar_t* buffer, size_t bufferSize);

    SYSTEM_THEME_PP_API bool isDarkMode();

    SYSTEM_THEME_PP_API ThemeColors getForegroundColor();
    SYSTEM_THEME_PP_API ThemeColors getBackgroundColor();
    SYSTEM_THEME_PP_API ThemeColors getAccentColor();
};  // namespace system_theme_pp

#endif /* E406110C_0236_43AF_AF02_300231E54175 */
