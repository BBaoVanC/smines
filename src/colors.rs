//! Colors used when displaying tiles

use tui::style::{Color, Style};

pub enum ColorType {
    TileHidden,
    TileFlag,
    TileFlagCorrect,
    TileFlagWrong,

    TileMineDead,
    TileMineSafe,
}

pub fn get_color(color_type: ColorType) -> Style {
    match color_type {
        ColorType::TileHidden => Style::default().fg(Color::Gray), /* .bg(Color::Reset) */
        ColorType::TileFlag => Style::default().fg(Color::Yellow).bg(Color::Black),
        ColorType::TileFlagCorrect => Style::default().fg(Color::Green).bg(Color::Black),
        ColorType::TileFlagWrong => Style::default().fg(Color::Yellow).bg(Color::Black),

        ColorType::TileMineDead => Style::default().fg(Color::Red).bg(Color::Black),
        ColorType::TileMineSafe => Style::default().fg(Color::Green).bg(Color::Black),
    }
}

pub fn get_color_from_surrounding(surrounding_mines: u8) -> Style {
    match surrounding_mines {
        0 => Style::default().fg(Color::White).bg(Color::Black),
        1 => Style::default().fg(Color::White).bg(Color::Blue),
        2 => Style::default().fg(Color::Black).bg(Color::Green),
        3 => Style::default().fg(Color::White).bg(Color::Red),
        4 => Style::default().fg(Color::Black).bg(Color::Cyan),
        5 => Style::default().fg(Color::Black).bg(Color::Yellow),
        6 => Style::default().fg(Color::Black).bg(Color::Magenta),
        7 => Style::default().fg(Color::White).bg(Color::Black),
        8 => Style::default().fg(Color::White).bg(Color::Gray),
        _ => unimplemented!(),
    }
}

/// TODO: get rid of this ugly lifetime workaround
pub fn get_str_from_surrounding(surrounding_mines: u8) -> &'static str {
    match surrounding_mines {
        0 => " 0",
        1 => " 1",
        2 => " 2",
        3 => " 3",
        4 => " 4",
        5 => " 5",
        6 => " 6",
        7 => " 7",
        8 => " 8",
        _ => unimplemented!(),
    }
}
