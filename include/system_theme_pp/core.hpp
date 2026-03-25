#ifndef D013FC57_EA1F_4C08_9F0C_1FD5B5B72A34
#define D013FC57_EA1F_4C08_9F0C_1FD5B5B72A34

/* clang-format off */
#ifdef SYSTEM_THEME_PP_EXPORTS
    #ifdef _MSC_VER
        #define SYSTEM_THEME_PP_API __declspec(dllexport)
        #define SYSTEM_THEME_PP_PRIVATE
    #else
        #define SYSTEM_THEME_PP_API
        #define SYSTEM_THEME_PP_PRIVATE __attribute__((visibility("hidden")))
    #endif
#else
    #ifdef _MSC_VER
        #define SYSTEM_THEME_PP_API __declspec(dllimport)
        #define SYSTEM_THEME_PP_PRIVATE
    #else
        #define SYSTEM_THEME_PP_API
        #define SYSTEM_THEME_PP_PRIVATE __attribute__((visibility("hidden")))
    #endif
#endif
/* clang-format on */

#endif /* D013FC57_EA1F_4C08_9F0C_1FD5B5B72A34 */
