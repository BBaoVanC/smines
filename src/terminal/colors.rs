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
        ColorType::TileHidden => ContentStyle::new().with(Color::DarkGrey).on(Color::Reset),
        ColorType::TileFlag => ContentStyle::new().with(Color::DarkYellow).on(Color::Black),
        ColorType::TileFlagCorrect => ContentStyle::new().with(Color::DarkGreen).on(Color::Black),
        ColorType::TileFlagWrong => ContentStyle::new().with(Color::DarkYellow).on(Color::Black),

        ColorType::TileMineDead => ContentStyle::new().with(Color::DarkRed).on(Color::Black),
        ColorType::TileMineSafe => ContentStyle::new().with(Color::DarkGreen).on(Color::Black),

        ColorType::TileNormal(s) => match s {
            0 => ContentStyle::new().with(Color::White).on(Color::Black),
            1 => ContentStyle::new().with(Color::White).on(Color::DarkBlue),
            2 => ContentStyle::new().with(Color::Black).on(Color::DarkGreen),
            3 => ContentStyle::new().with(Color::White).on(Color::DarkRed),
            4 => ContentStyle::new().with(Color::Black).on(Color::DarkCyan),
            5 => ContentStyle::new().with(Color::Black).on(Color::DarkYellow),
            6 => ContentStyle::new().with(Color::Black).on(Color::DarkMagenta),
            7 => ContentStyle::new().with(Color::White).on(Color::Black),
            8 => ContentStyle::new().with(Color::White).on(Color::Black),
            _ => unimplemented!(),
        },
    }
}
