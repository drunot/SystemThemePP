#ifndef F70A7A1F_FF0C_4310_A017_7CF81D1C054B
#define F70A7A1F_FF0C_4310_A017_7CF81D1C054B
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct GnomeCSSRGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    uint8_t valid;
};

struct GnomeCSSRGBA get_color_from_css_c(
    const char* path, const char* prefer_color_scheme, const char* color_name);

#ifdef __cplusplus
}
#endif

#endif /* F70A7A1F_FF0C_4310_A017_7CF81D1C054B */
