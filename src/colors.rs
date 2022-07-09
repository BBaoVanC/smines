//! Colors used when displaying tiles

use tui::style::{Color, Style};

pub enum ColorType {
    TileZero,
    TileOne,
    TileTwo,
    TileThree,
    TileFour,
    TileFive,
    TileSix,
    TileSeven,
    TileEight,

    TileHidden,
    TileFlag,
    TileFlagWrong,
}

pub fn get_color(color_type: ColorType) -> Style {
    match color_type {
        ColorType::TileZero => Style::default().fg(Color::White).bg(Color::Black),
        ColorType::TileOne => Style::default().fg(Color::White).bg(Color::Blue),
        ColorType::TileTwo => Style::default().fg(Color::Black).bg(Color::Green),
        ColorType::TileThree => Style::default().fg(Color::White).bg(Color::Red),
        ColorType::TileFour => Style::default().fg(Color::Black).bg(Color::Cyan),
        ColorType::TileFive => Style::default().fg(Color::Black).bg(Color::Yellow),
        ColorType::TileSix => Style::default().fg(Color::Black).bg(Color::Magenta),
        ColorType::TileSeven => Style::default().fg(Color::White).bg(Color::Black),
        ColorType::TileEight => Style::default().fg(Color::White).bg(Color::Gray),

        ColorType::TileHidden => Style::default().fg(Color::Gray), /* .bg(Color::Reset) */
        ColorType::TileFlag => Style::default().fg(Color::Yellow).bg(Color::Black),
        ColorType::TileFlagWrong => Style::default().fg(Color::Yellow).bg(Color::Black),
    }
}
