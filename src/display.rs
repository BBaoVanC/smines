//! Widget for tui.rs that displays a minefield game on the screen.
//!
//! # Panics
//!
//! This module is currently written in a way that means it will panic when
//! there are unexpected errors. This should be refactored in the future.

use ndarray::Array2;
use tui::{
    style::{Color, Style},
    text::Span,
    widgets::Widget,
};

use crate::{
    constants::{TILE_TERMINAL_HEIGHT, TILE_TERMINAL_WIDTH},
    game::Game,
    minesweeper::Tile,
};

pub struct GameWidget<'w> {
    game: &'w Game,
}
impl<'n> GameWidget<'n> {
    pub fn new(game: &'n Game) -> Self {
        Self { game }
    }
}
impl Widget for GameWidget<'_> {
    fn render(self, area: tui::layout::Rect, buf: &mut tui::buffer::Buffer) {
        let game = self.game;
        let mut output_tiles = Array2::from_shape_fn((game.width(), game.height()), |(x, y)| {
            let tile = game.get_tile(x, y).unwrap();
            match tile {
                Tile::Mine => Span::styled(" X", Style::default().bg(Color::Black).fg(Color::Red)),
                Tile::Tile(surrounding_mines) => Span::styled(
                    format!(" {surrounding_mines}"),
                    Style::default().bg(Color::Blue).fg(Color::White),
                ),
            }
        });

        // Overwrite the cursor tile
        let mut a = Span::raw("a");
        let cursor_tile = output_tiles
            .get_mut((game.cursor.x, game.cursor.y))
            .unwrap_or(&mut a);
        cursor_tile.style.bg(Color::White).fg(Color::Black);

        for (y, row) in output_tiles.rows().into_iter().enumerate() {
            for (x, span) in row.into_iter().enumerate() {
                // width is 2 because each tile is 1x2 in the terminal
                buf.set_span(
                    area.x
                        .saturating_add(x.saturating_mul(TILE_TERMINAL_WIDTH).try_into().unwrap()),
                    area.y
                        .saturating_add(y.try_into().unwrap())
                        .saturating_mul(TILE_TERMINAL_HEIGHT.try_into().unwrap()),
                    span,
                    2,
                );
            }
        }
    }
}
