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

use crate::{game::Game, minesweeper::Tile};

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
        let mut output = Array2::from_shape_fn((game.width(), game.height()), |(x, y)| {
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
        let cursor_tile = output.get_mut((game.cursor.x, game.cursor.y)).unwrap();
        cursor_tile.style.bg(Color::White).fg(Color::Black);

        // for row in output.rows() {
        //     for tile in row.iter() {

        //     }
        // }

        // output.rows().into_iter().map(|row| {
        //     Spans::from(row)
        // })

        // for (i, row) in output.rows().into_iter().enumerate() {
        //     buf.set_spans(
        //         area.x,
        //         area.y.saturating_add(i),
        //         &Spans::from(row.into_iter().collect::<Vec<&Span>>().to_owned()),
        //         game.display_cols().try_into().unwrap(),
        //     );
        // }

        for (y, row) in output.rows().into_iter().enumerate() {
            for (x, span) in row.into_iter().enumerate() {
                // width is 2 because each tile is 1x2 in the terminal
                buf.set_span(
                    area.x.saturating_add(x.try_into().unwrap()),
                    area.y.saturating_add(y.try_into().unwrap()),
                    span,
                    2,
                );
            }
        }

        // // Rows
        // self.game
        //     .minefield
        //     .tiles
        //     .rows()
        //     .into_iter()
        //     .map(|row| {
        //         Spans::from(
        //             row.into_iter()
        //                 .map(|tile| {
        //                     if let Tile::Tile(surrounding_mines) = tile {
        //                         Span::styled(
        //                             format!(" {}", surrounding_mines),
        //
        // Style::default().bg(Color::Blue).fg(Color::White),
        //                         )
        //                     } else {
        //                         Span::styled(" X",
        // Style::default().fg(Color::Red).bg(Color::Black))
        // }                 })
        //                 .collect::<Vec<Span>>(),
        //         )
        //     })
        //     .enumerate()
        //     .for_each(|i| {
        //         buf.set_spans(
        //             area.x,
        //             // NOTE: `as` can panic
        //             area.y.saturating_add(i.0 as u16),
        //             &i.1,
        //             self.game.minefield.display_cols().try_into().unwrap(),
        //         );
        //     })
        // // .collect::<Vec<Spans>>()
    }
}
