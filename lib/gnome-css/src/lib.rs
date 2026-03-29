use lightningcss::stylesheet::{ParserOptions, StyleSheet};
use lightningcss::traits::ToCss;

use std::path::Path;

use std::fs;

// fn get_color_from_function(
//     stylesheet: &StyleSheet,
//     function_token: &lightningcss::properties::custom::Function,
// ) -> Option<lightningcss::values::color::CssColor> {
//     let mut args = Vec::<lightningcss::properties::custom::TokenOrValue>::new();
//     for arg in &function_token.arguments.0 {
//         if let lightningcss::properties::custom::TokenOrValue::Token(token) = arg {
//             match token{
//                 lightningcss::properties::custom::Token::AtKeyword(keyword) => {
//                     if let Some(color) = get_define_color_by_name(stylesheet, &keyword.to_string()) {
//                         args.push(lightningcss::properties::custom::TokenOrValue::Color(color));
//                     }
//                 },
//                 lightningcss::properties::custom::Token::WhiteSpace(_) => {},
//                 _ => args.push(arg.clone()),
//             }
//         } else {
//             args.push(arg.clone());
//         }
//     }
//     let _func = lightningcss::properties::custom::Function {
//         name: function_token.name.clone(),
//         arguments: lightningcss::properties::custom::TokenList(args),
//     };
//     match
//     None
// }

fn get_variable_value_by_name(
    stylesheet: &StyleSheet,
    prefer_color_scheme: &str,
    style_rule: &lightningcss::rules::unknown::UnknownAtRule,
) -> Option<lightningcss::values::color::CssColor> {
    // If the value is a atToken is should be recursively resolved, if it's a color we can return it, if it's a function we should try to resolve it.
    if let lightningcss::properties::custom::TokenOrValue::Token(token) = &style_rule.prelude.0[2] {
        if let lightningcss::properties::custom::Token::AtKeyword(keyword) = token {
            return get_define_color_by_name(stylesheet, prefer_color_scheme, &keyword.to_string())
                .0;
        }
    }
    // If the token is already a color we can return it directly.
    if let lightningcss::properties::custom::TokenOrValue::Color(value) = &style_rule.prelude.0[2] {
        return Some(value.clone());
    }

    // TODO handle functions like color-mix and color-contrast, OKLAB, min() and max()
    // if let lightningcss::properties::custom::TokenOrValue::Function(_func) = &style_rule.prelude.0[2]
    // {
    //     return get_color_from_function(stylesheet, func);
    // }
    None
}

fn get_define_color_by_name_from_rules(
    stylesheet: &StyleSheet,
    prefer_color_scheme: &str,
    stylesheet_rules: &lightningcss::rules::CssRuleList,
    name: &str,
) -> (Option<lightningcss::values::color::CssColor>, u32) {
    let mut to_return = None;
    let mut location = 0;
    for rule in &stylesheet_rules.0 {
        if let lightningcss::rules::CssRule::Unknown(unkn_rule) = rule {
            if unkn_rule.name == "define-color" {
                if let lightningcss::properties::custom::TokenOrValue::Token(token) =
                    &unkn_rule.prelude.0[0]
                {
                    let rule_name = ToCss::to_css_string(
                        &token,
                        lightningcss::printer::PrinterOptions::default(),
                    )
                    .unwrap();
                    if rule_name == name {
                        if unkn_rule.loc.line >= location {
                            location = unkn_rule.loc.line;
                            to_return = get_variable_value_by_name(
                                stylesheet,
                                prefer_color_scheme,
                                &unkn_rule,
                            );
                        }
                    }
                }
            }
        }
    }
    (to_return, location)
}

fn get_define_color_by_name(
    stylesheet: &StyleSheet,
    prefer_color_scheme: &str,
    name: &str,
) -> (Option<lightningcss::values::color::CssColor>, u32) {
    let (color, _location) = get_define_color_by_name_from_rules(
        &stylesheet,
        prefer_color_scheme,
        &stylesheet.rules,
        name,
    );
    let (prefer_color_scheme_color, prefer_color_scheme_location) =
        get_color_from_prefer_color_scheme(&stylesheet, prefer_color_scheme, name);
    if let Some(prefer_color_scheme_color) = prefer_color_scheme_color {
        if prefer_color_scheme_location >= _location {
            return (
                Some(prefer_color_scheme_color),
                prefer_color_scheme_location,
            );
        }
    }
    (color, _location)
}

fn get_color_from_prefer_color_scheme(
    stylesheet: &StyleSheet,
    prefer_color_scheme: &str,
    color_name: &str,
) -> (Option<lightningcss::values::color::CssColor>, u32) {
    // This unpacking could probably be done cleaner.
    // We find all media rules where prefers-color-scheme is used and return the values here.
    for rule in &stylesheet.rules.0 {
        if let lightningcss::rules::CssRule::Media(media_rule) = rule {
            // println!("Found media rule: {:?}", media_rule.query);
            for query in &media_rule.query.media_queries {
                if query.qualifier == None
                    && query.media_type == lightningcss::media_query::MediaType::All
                {
                    //For now only handle conditions where only PrefersColorScheme is used.
                    if let Some(condition) = &query.condition {
                        if let lightningcss::media_query::MediaCondition::Feature(feature) =
                            condition
                        {
                            if let lightningcss::media_query::MediaFeature::Plain { name, value } =
                                feature
                            {
                                let name_str = ToCss::to_css_string(
                                    &name,
                                    lightningcss::printer::PrinterOptions::default(),
                                )
                                .unwrap();
                                if name_str == "prefers-color-scheme" {
                                    let value_str = ToCss::to_css_string(
                                        &value,
                                        lightningcss::printer::PrinterOptions::default(),
                                    )
                                    .unwrap();
                                    if value_str == prefer_color_scheme {
                                        return get_define_color_by_name_from_rules(
                                            stylesheet,
                                            prefer_color_scheme,
                                            &media_rule.rules,
                                            color_name,
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    (None, 0)
}

fn get_color_from_css(
    path: &str,
    prefer_color_scheme: &str,
    color_name: &str,
) -> Option<lightningcss::values::color::CssColor> {
    let css = fs::read_to_string(path).expect("Failed to read CSS file");
    let stylesheet =
        StyleSheet::parse(&css, ParserOptions::default()).expect("Failed to parse CSS");
    let mut color = None;
    let mut location = 0;

    // Support @import rules, if the import is after the definition of the color it should override it, if it's before it should be ignored.
    for rule in &stylesheet.rules.0 {
        if let lightningcss::rules::CssRule::Import(import_rule) = rule {
            let import_path = &import_rule.url;
            let path = Path::new(path);
            let new_path = path.with_file_name(import_path.to_string());
            let tmp_color =
                get_color_from_css(new_path.to_str().unwrap(), prefer_color_scheme, color_name);
            if let Some(ref some_tmp_color2) = tmp_color {
                if import_rule.loc.line >= location {
                    color = Some(some_tmp_color2.clone());
                    location = import_rule.loc.line;
                }
            }
        }
    }

    // Read the colors from the main stylesheet as well.
    let (tmp_color, tmp_location) =
        get_define_color_by_name(&stylesheet, prefer_color_scheme, color_name);
    if let Some(ref tmp_color3) = tmp_color {
        if tmp_location >= location {
            color = Some(tmp_color3.clone());
        }
    }
    color
}

#[repr(C)]
pub struct ColorRGBA {
    pub r: u8,
    pub g: u8,
    pub b: u8,
    pub a: u8,
    pub valid: u8,
}

#[unsafe(no_mangle)]
pub extern "C" fn get_color_from_css_c(
    path: *const libc::c_char,
    prefer_color_scheme: *const libc::c_char,
    color_name: *const libc::c_char,
) -> ColorRGBA {
    use std::ffi::CStr;

    let path = unsafe { CStr::from_ptr(path).to_str().unwrap_or("") };
    let prefer_color_scheme = unsafe { CStr::from_ptr(prefer_color_scheme).to_str().unwrap_or("") };
    let color_name = unsafe { CStr::from_ptr(color_name).to_str().unwrap_or("") };

    if let Some(css_color) = get_color_from_css(path, prefer_color_scheme, color_name) {
        // You may need to adjust this depending on the actual CssColor API
        if let lightningcss::values::color::CssColor::RGBA(rgba) = css_color.to_rgb().unwrap() {
            ColorRGBA {
                r: rgba.red,
                g: rgba.green,
                b: rgba.blue,
                a: rgba.alpha,
                valid: 1,
            }
        } else {
            ColorRGBA {
                r: 0,
                g: 0,
                b: 0,
                a: 0,
                valid: 0,
            }
        }
    } else {
        ColorRGBA {
            r: 0,
            g: 0,
            b: 0,
            a: 0,
            valid: 0,
        }
    }
}
