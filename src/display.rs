//! Widget for tui.rs that displays a minefield game on the screen.

use tui::{
    style::{Color, Style},
    text::{Span, Spans},
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
        // Rows
        self.game
            .minefield
            .tiles
            .rows()
            .into_iter()
            .map(|row| {
                Spans::from(
                    row.into_iter()
                        .map(|tile| {
                            if let Tile::Tile(surrounding_mines) = tile {
                                Span::styled(
                                    format!(" {}", surrounding_mines),
                                    Style::default().bg(Color::Blue).fg(Color::White),
                                )
                            } else {
                                Span::styled(" X", Style::default().fg(Color::Red).bg(Color::Black))
                            }
                        })
                        .collect::<Vec<Span>>(),
                )
            })
            .enumerate()
            .for_each(|i| {
                buf.set_spans(
                    area.x,
                    // NOTE: `as` can panic
                    area.y.saturating_add(i.0 as u16),
                    &i.1,
                    self.game.minefield.display_cols().try_into().unwrap(),
                );
            })
        // .collect::<Vec<Spans>>()
    }
}
