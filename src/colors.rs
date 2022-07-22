//! Colors used when displaying tiles

use crossterm::style::{Color, ContentStyle, Stylize};

pub enum ColorType {
    TileHidden,
    TileFlag,
    TileFlagCorrect,
    TileFlagWrong,

    TileMineDead,
    TileMineSafe,

    TileNormal(u8),
}

pub fn get_color(color_type: ColorType) -> ContentStyle {
    match color_type {
        ColorType::TileHidden => ContentStyle::new().with(Color::Grey).on(Color::Reset),
        ColorType::TileFlag => ContentStyle::new().with(Color::Yellow).on(Color::Black),
        ColorType::TileFlagCorrect => ContentStyle::new().with(Color::Green).on(Color::Black),
        ColorType::TileFlagWrong => ContentStyle::new().with(Color::Yellow).on(Color::Black),

        ColorType::TileMineDead => ContentStyle::new().with(Color::Red).on(Color::Black),
        ColorType::TileMineSafe => ContentStyle::new().with(Color::Green).on(Color::Black),

        ColorType::TileNormal(s) => match s {
            0 => ContentStyle::new().with(Color::White).on(Color::Black),
            1 => ContentStyle::new().with(Color::White).on(Color::Blue),
            2 => ContentStyle::new().with(Color::Black).on(Color::Green),
            3 => ContentStyle::new().with(Color::White).on(Color::Red),
            4 => ContentStyle::new().with(Color::Black).on(Color::Cyan),
            5 => ContentStyle::new().with(Color::Black).on(Color::Yellow),
            6 => ContentStyle::new().with(Color::Black).on(Color::Magenta),
            7 => ContentStyle::new().with(Color::White).on(Color::Black),
            8 => ContentStyle::new().with(Color::White).on(Color::Grey),
            _ => unimplemented!(),
        },
    }
}
