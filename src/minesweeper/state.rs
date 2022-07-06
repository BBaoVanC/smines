//! Logic relating to holding an actual minesweeper game.

use tui::{
    style::{Color, Style},
    text::{Span, Spans},
    widgets::Widget,
};

use super::{Coordinate, Minefield, Tile};

#[derive(Clone, Copy, Debug, Default)]
/// The status of the game.
pub enum GameState {
    /// The player has won the game.
    Win,
    /// The player has lost the game.
    Loss,
    #[default]
    /// The game is still in progress.
    Playing,
}

#[derive(Debug)]
/// An actual minesweeper game, with its own state.
pub struct Game {
    state: GameState,
    placed_flags: usize,
    /// The underlying minefield schematic.
    pub minefield: Minefield,
    /// The cursor position on the minefield.
    pub cursor: Coordinate,
}
impl Game {
    /// Create a new Minesweeper game.
    pub fn new(width: usize, height: usize, mine_count: usize) -> Self {
        Self {
            state: GameState::default(),
            minefield: Minefield::new(width, height).generate_mines_simple(mine_count),
            placed_flags: 0,
            cursor: Coordinate {
                x: width / 2,
                y: height / 2,
            },
        }
    }

    /// Move the cursor to an exact position.
    ///
    /// TODO: put an example/test
    pub fn move_cursor_absolute(&mut self, x: usize, y: usize) {
        self.cursor.x = x;
        self.cursor.y = y;
    }
    /// Move the cursor relative to itself.
    ///
    /// TODO: put an example/test
    pub fn move_cursor_relative(&mut self, x: isize, y: isize) {
        self.cursor.x = self.cursor.x.saturating_add(x as usize);
        self.cursor.y = self.cursor.y.saturating_add(y as usize);
    }
}

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
                    self.game.minefield.display_cols().unwrap(),
                );
            })
        // .collect::<Vec<Spans>>()
    }
}
