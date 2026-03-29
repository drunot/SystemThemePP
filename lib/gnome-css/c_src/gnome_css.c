#include "gnome_css/gnome_css.h"

#include <stdio.h>

extern struct GnomeCSSRGBA get_color_from_css_c(
    const char* path, const char* prefer_color_scheme, const char* color_name);