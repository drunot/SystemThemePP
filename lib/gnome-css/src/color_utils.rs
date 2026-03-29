
pub fn string_to_color(color_str: &str) -> Option<lightningcss::values::color::CssColor> {
    match color_str {
        "aliceblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 240,
                green: 248,
                blue: 255,
                alpha: 0,
            }))
        }
        "antiquewhite" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 250,
                green: 235,
                blue: 215,
                alpha: 0,
            }))
        }
        "aqua" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 255,
                blue: 255,
                alpha: 0,
            }))
        }
        "aquamarine" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 127,
                green: 255,
                blue: 212,
                alpha: 0,
            }))
        }
        "azure" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 240,
                green: 255,
                blue: 255,
                alpha: 0,
            }))
        }
        "beige" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 245,
                green: 245,
                blue: 220,
                alpha: 0,
            }))
        }
        "bisque" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 228,
                blue: 196,
                alpha: 0,
            }))
        }
        "black" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 0,
                blue: 0,
                alpha: 0,
            }))
        }
        "blanchedalmond" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 235,
                blue: 205,
                alpha: 0,
            }))
        }
        "blue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 0,
                blue: 255,
                alpha: 0,
            }))
        }
        "blueviolet" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 138,
                green: 43,
                blue: 226,
                alpha: 0,
            }))
        }
        "brown" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 165,
                green: 42,
                blue: 42,
                alpha: 0,
            }))
        }
        "burlywood" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 222,
                green: 184,
                blue: 135,
                alpha: 0,
            }))
        }
        "cadetblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 95,
                green: 158,
                blue: 160,
                alpha: 0,
            }))
        }
        "chartreuse" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 95,
                green: 158,
                blue: 160,
                alpha: 0,
            }))
        }
        "chocolate" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 210,
                green: 105,
                blue: 30,
                alpha: 0,
            }))
        }
        "coral" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 127,
                blue: 80,
                alpha: 0,
            }))
        }
        "cornflowerblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 100,
                green: 149,
                blue: 237,
                alpha: 0,
            }))
        }
        "cornsilk" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 248,
                blue: 220,
                alpha: 0,
            }))
        }
        "crimson" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 220,
                green: 20,
                blue: 60,
                alpha: 0,
            }))
        }
        "cyan" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 255,
                blue: 255,
                alpha: 0,
            }))
        }
        "darkblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 0,
                blue: 139,
                alpha: 0,
            }))
        }
        "darkcyan" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 139,
                blue: 139,
                alpha: 0,
            }))
        }
        "darkgoldenrod" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 184,
                green: 134,
                blue: 11,
                alpha: 0,
            }))
        }
        "darkgray" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 169,
                green: 169,
                blue: 169,
                alpha: 0,
            }))
        }
        "darkgreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 100,
                blue: 0,
                alpha: 0,
            }))
        }
        "darkkhaki" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 189,
                green: 183,
                blue: 107,
                alpha: 0,
            }))
        }
        "darkmagenta" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 139,
                green: 0,
                blue: 139,
                alpha: 0,
            }))
        }
        "darkolivegreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 85,
                green: 107,
                blue: 47,
                alpha: 0,
            }))
        }
        "darkorange" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 140,
                blue: 0,
                alpha: 0,
            }))
        }
        "darkorchid" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 153,
                green: 50,
                blue: 204,
                alpha: 0,
            }))
        }
        "darkred" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 139,
                green: 0,
                blue: 0,
                alpha: 0,
            }))
        }
        "darksalmon" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 233,
                green: 150,
                blue: 122,
                alpha: 0,
            }))
        }
        "darkseagreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 143,
                green: 188,
                blue: 143,
                alpha: 0,
            }))
        }
        "darkslateblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 72,
                green: 61,
                blue: 139,
                alpha: 0,
            }))
        }
        "darkslategray" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 47,
                green: 79,
                blue: 79,
                alpha: 0,
            }))
        }
        "darkturquoise" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 206,
                blue: 209,
                alpha: 0,
            }))
        }
        "darkviolet" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 148,
                green: 0,
                blue: 211,
                alpha: 0,
            }))
        }
        "deeppink" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 20,
                blue: 147,
                alpha: 0,
            }))
        }
        "deepskyblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 191,
                blue: 255,
                alpha: 0,
            }))
        }
        "dimgray" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 191,
                blue: 255,
                alpha: 0,
            }))
        }
        "dodgerblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 30,
                green: 144,
                blue: 255,
                alpha: 0,
            }))
        }
        "firebrick" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 178,
                green: 34,
                blue: 34,
                alpha: 0,
            }))
        }
        "floralwhite" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 250,
                blue: 240,
                alpha: 0,
            }))
        }
        "forestgreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 34,
                green: 139,
                blue: 34,
                alpha: 0,
            }))
        }
        "fuchsia" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 0,
                blue: 255,
                alpha: 0,
            }))
        }
        "gainsboro" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 220,
                green: 220,
                blue: 220,
                alpha: 0,
            }))
        }
        "ghostwhite" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 248,
                green: 248,
                blue: 255,
                alpha: 0,
            }))
        }
        "gold" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 215,
                blue: 0,
                alpha: 0,
            }))
        }
        "goldenrod" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 218,
                green: 165,
                blue: 32,
                alpha: 0,
            }))
        }
        "gray" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 127,
                green: 127,
                blue: 127,
                alpha: 0,
            }))
        }
        "green" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 128,
                blue: 0,
                alpha: 0,
            }))
        }
        "greenyellow" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 173,
                green: 255,
                blue: 47,
                alpha: 0,
            }))
        }
        "honeydew" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 240,
                green: 255,
                blue: 240,
                alpha: 0,
            }))
        }
        "hotpink" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 105,
                blue: 180,
                alpha: 0,
            }))
        }
        "indianred" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 205,
                green: 92,
                blue: 92,
                alpha: 0,
            }))
        }
        "indigo" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 75,
                green: 0,
                blue: 130,
                alpha: 0,
            }))
        }
        "ivory" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 255,
                blue: 240,
                alpha: 0,
            }))
        }
        "khaki" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 240,
                green: 230,
                blue: 140,
                alpha: 0,
            }))
        }
        "lavender" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 230,
                green: 230,
                blue: 250,
                alpha: 0,
            }))
        }
        "lavenderblush" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 240,
                blue: 245,
                alpha: 0,
            }))
        }
        "lawngreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 124,
                green: 252,
                blue: 0,
                alpha: 0,
            }))
        }
        "lemonchiffon" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 250,
                blue: 205,
                alpha: 0,
            }))
        }
        "lightblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 173,
                green: 216,
                blue: 230,
                alpha: 0,
            }))
        }
        "lightcoral" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 240,
                green: 128,
                blue: 128,
                alpha: 0,
            }))
        }
        "lightcyan" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 224,
                green: 255,
                blue: 255,
                alpha: 0,
            }))
        }
        "lightgoldenrodyellow" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 250,
                green: 250,
                blue: 210,
                alpha: 0,
            }))
        }
        "lightgreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 144,
                green: 238,
                blue: 144,
                alpha: 0,
            }))
        }
        "lightgrey" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 211,
                green: 211,
                blue: 211,
                alpha: 0,
            }))
        }
        "lightpink" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 182,
                blue: 193,
                alpha: 0,
            }))
        }
        "lightsalmon" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 160,
                blue: 122,
                alpha: 0,
            }))
        }
        "lightseagreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 32,
                green: 178,
                blue: 170,
                alpha: 0,
            }))
        }
        "lightskyblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 135,
                green: 206,
                blue: 250,
                alpha: 0,
            }))
        }
        "lightslategray" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 119,
                green: 136,
                blue: 153,
                alpha: 0,
            }))
        }
        "lightsteelblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 176,
                green: 196,
                blue: 222,
                alpha: 0,
            }))
        }
        "lightyellow" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 255,
                blue: 224,
                alpha: 0,
            }))
        }
        "lime" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 255,
                blue: 0,
                alpha: 0,
            }))
        }
        "limegreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 50,
                green: 205,
                blue: 50,
                alpha: 0,
            }))
        }
        "linen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 250,
                green: 240,
                blue: 230,
                alpha: 0,
            }))
        }
        "magenta" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 0,
                blue: 255,
                alpha: 0,
            }))
        }
        "maroon" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 128,
                green: 0,
                blue: 0,
                alpha: 0,
            }))
        }
        "mediumaquamarine" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 102,
                green: 205,
                blue: 170,
                alpha: 0,
            }))
        }
        "mediumblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 0,
                blue: 205,
                alpha: 0,
            }))
        }
        "mediumorchid" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 186,
                green: 85,
                blue: 211,
                alpha: 0,
            }))
        }
        "mediumpurple" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 147,
                green: 112,
                blue: 219,
                alpha: 0,
            }))
        }
        "mediumseagreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 60,
                green: 179,
                blue: 113,
                alpha: 0,
            }))
        }
        "mediumslateblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 123,
                green: 104,
                blue: 238,
                alpha: 0,
            }))
        }
        "mediumspringgreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 250,
                blue: 154,
                alpha: 0,
            }))
        }
        "mediumturquoise" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 72,
                green: 209,
                blue: 204,
                alpha: 0,
            }))
        }
        "mediumvioletred" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 199,
                green: 21,
                blue: 133,
                alpha: 0,
            }))
        }
        "midnightblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 25,
                green: 25,
                blue: 112,
                alpha: 0,
            }))
        }
        "mintcream" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 245,
                green: 255,
                blue: 250,
                alpha: 0,
            }))
        }
        "mistyrose" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 228,
                blue: 225,
                alpha: 0,
            }))
        }
        "moccasin" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 228,
                blue: 181,
                alpha: 0,
            }))
        }
        "navajowhite" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 222,
                blue: 173,
                alpha: 0,
            }))
        }
        "navy" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 0,
                blue: 128,
                alpha: 0,
            }))
        }
        "navyblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 159,
                green: 175,
                blue: 223,
                alpha: 0,
            }))
        }
        "oldlace" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 253,
                green: 245,
                blue: 230,
                alpha: 0,
            }))
        }
        "olive" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 128,
                green: 128,
                blue: 0,
                alpha: 0,
            }))
        }
        "olivedrab" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 107,
                green: 142,
                blue: 35,
                alpha: 0,
            }))
        }
        "orange" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 165,
                blue: 0,
                alpha: 0,
            }))
        }
        "orangered" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 69,
                blue: 0,
                alpha: 0,
            }))
        }
        "orchid" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 218,
                green: 112,
                blue: 214,
                alpha: 0,
            }))
        }
        "palegoldenrod" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 238,
                green: 232,
                blue: 170,
                alpha: 0,
            }))
        }
        "palegreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 152,
                green: 251,
                blue: 152,
                alpha: 0,
            }))
        }
        "paleturquoise" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 175,
                green: 238,
                blue: 238,
                alpha: 0,
            }))
        }
        "palevioletred" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 219,
                green: 112,
                blue: 147,
                alpha: 0,
            }))
        }
        "papayawhip" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 239,
                blue: 213,
                alpha: 0,
            }))
        }
        "peachpuff" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 218,
                blue: 185,
                alpha: 0,
            }))
        }
        "peru" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 205,
                green: 133,
                blue: 63,
                alpha: 0,
            }))
        }
        "pink" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 192,
                blue: 203,
                alpha: 0,
            }))
        }
        "plum" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 221,
                green: 160,
                blue: 221,
                alpha: 0,
            }))
        }
        "powderblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 176,
                green: 224,
                blue: 230,
                alpha: 0,
            }))
        }
        "purple" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 128,
                green: 0,
                blue: 128,
                alpha: 0,
            }))
        }
        "red" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 0,
                blue: 0,
                alpha: 0,
            }))
        }
        "rosybrown" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 188,
                green: 143,
                blue: 143,
                alpha: 0,
            }))
        }
        "royalblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 65,
                green: 105,
                blue: 225,
                alpha: 0,
            }))
        }
        "saddlebrown" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 139,
                green: 69,
                blue: 19,
                alpha: 0,
            }))
        }
        "salmon" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 250,
                green: 128,
                blue: 114,
                alpha: 0,
            }))
        }
        "sandybrown" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 244,
                green: 164,
                blue: 96,
                alpha: 0,
            }))
        }
        "seagreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 46,
                green: 139,
                blue: 87,
                alpha: 0,
            }))
        }
        "seashell" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 245,
                blue: 238,
                alpha: 0,
            }))
        }
        "sienna" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 160,
                green: 82,
                blue: 45,
                alpha: 0,
            }))
        }
        "silver" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 192,
                green: 192,
                blue: 192,
                alpha: 0,
            }))
        }
        "skyblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 135,
                green: 206,
                blue: 235,
                alpha: 0,
            }))
        }
        "slateblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 106,
                green: 90,
                blue: 205,
                alpha: 0,
            }))
        }
        "slategray" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 112,
                green: 128,
                blue: 144,
                alpha: 0,
            }))
        }
        "snow" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 250,
                blue: 250,
                alpha: 0,
            }))
        }
        "springgreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 255,
                blue: 127,
                alpha: 0,
            }))
        }
        "steelblue" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 70,
                green: 130,
                blue: 180,
                alpha: 0,
            }))
        }
        "tan" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 210,
                green: 180,
                blue: 140,
                alpha: 0,
            }))
        }
        "teal" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 0,
                green: 128,
                blue: 128,
                alpha: 0,
            }))
        }
        "thistle" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 216,
                green: 191,
                blue: 216,
                alpha: 0,
            }))
        }
        "tomato" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 99,
                blue: 71,
                alpha: 0,
            }))
        }
        "turquoise" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 64,
                green: 224,
                blue: 208,
                alpha: 0,
            }))
        }
        "violet" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 238,
                green: 130,
                blue: 238,
                alpha: 0,
            }))
        }
        "wheat" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 245,
                green: 222,
                blue: 179,
                alpha: 0,
            }))
        }
        "white" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 255,
                blue: 255,
                alpha: 0,
            }))
        }
        "whitesmoke" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 245,
                green: 245,
                blue: 245,
                alpha: 0,
            }))
        }
        "yellow" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 255,
                green: 255,
                blue: 0,
                alpha: 0,
            }))
        }
        "yellowgreen" => {
            return Some(From::from(lightningcss::values::color::RGBA {
                red: 139,
                green: 205,
                blue: 50,
                alpha: 0,
            }))
        }
        _ => return None,
    }
}
