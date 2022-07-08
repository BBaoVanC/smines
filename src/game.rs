//! Logic relating to handling minesweeper gameplay.

use std::{error::Error, backtrace::Backtrace};

use ndarray::Array2;
use thiserror::Error;
use tui::{
    style::{Color, Style},
    text::{Span, Spans},
};

use crate::minesweeper::{Minefield, Tile};

#[derive(Clone, Copy, Debug, Default)]
/// The status of the game.
pub enum GameStatus {
    /// The player has won the game.
    Win,
    /// The player has lost the game.
    Loss,
    #[default]
    /// The game is still in progress.
    Playing,
}

#[derive(Clone, Copy, Debug, Default)]
/// The flagged state of a tile.
pub enum TileFlag {
    Flagged,
    Guessing,
    #[default]
    Empty,
}

#[derive(Clone, Copy, Debug)]
/// A cursor on the minefield.
pub struct Cursor {
    /// The horizontal position of the coordinate in tiles.
    pub x: usize,
    /// The vertical position of the coordinate in tiles.
    pub y: usize,
}

#[derive(Debug)]
/// An actual minesweeper game, with its own state.
pub struct Game {
    status: GameStatus,
    /// Two-dimensional array of all the flags the player has set.
    flags: Array2<TileFlag>,
    minefield: Minefield,
    /// The cursor position.
    pub cursor: Cursor,
}
impl Game {
    /// Create a new Minesweeper game.
    pub fn new(width: usize, height: usize, mine_count: usize) -> Self {
        Self {
            status: GameStatus::default(),
            minefield: Minefield::new(width, height).generate_mines_simple(mine_count),
            flags: Array2::from_shape_simple_fn((width, height), TileFlag::default),
            cursor: Cursor {
                x: width / 2,
                y: height / 2,
            },
        }
    }

    /// Get the amount of columns (the width of the minefield in tiles)
    pub fn width(&self) -> usize {
        self.minefield.width()
    }
    /// Get the amount of rows (the height of the minefield in tiles)
    pub fn height(&self) -> usize {
        self.minefield.height()
    }

    pub fn get_flag(&self, x: usize, y: usize) -> Option<&TileFlag> {
        self.flags.get((x, y))
    }

    /// Get a [`Tile`] by its index
    ///
    /// This function returns [`None`] if the index is out of bounds.
    pub fn get_tile(&self, x: usize, y: usize) -> Option<&Tile> {
        self.minefield.get_tile(x, y)
    }

    // pub fn render(&self) -> Result<Vec<Spans>, RenderError> {
    //     let mut output_tiles = Array2::from_shape_fn((self.width(), self.height()), |(x, y)| {
    //         let tile = match self.get_tile(x, y) {
    //             Some(t) => t,
    //             // Some(t) => {
    //             //     return Span::styled("!!", Style::default().bg(Color::Red).fg(Color::White))
    //             // }
    //             None => {
    //                 return Span::styled("!!", Style::default().bg(Color::Red).fg(Color::White))
    //             }
    //         };

    //         match self.get_flag(x, y).ok_or(RenderError {})?{
    //             TileFlag::Flagged => {
    //                 match self.status {
    //                     GameStatus::Playing => Span::styled(" F", Style::default().bg(Color::Black).fg(Color::Yellow)),
    //                     GameStatus::Win => Span::styled(" F", Style::default().bg(Color::Black).fg(Color::Green)),
    //                     GameStatus::Loss => Span::styled(" F", Style::default().bg(Color::Black).fg(Color::Red)),
    //                 }
    //             },
    //             TileFlag::Guessing => {
    //                 match self.status {
    //                     GameStatus::Playing => Span::styled(" ?", Style::default().bg(Color::Black).fg(Color::Yellow)),
    //                     GameStatus::Win => Span::styled(" ?", Style::default().bg(Color::Black).fg(Color::Green)),
    //                     GameStatus::Loss => Span::styled(" ?", Style::default().bg(Color::Black).fg(Color::Red)),
    //                 }
    //             },
    //             TileFlag::Empty => {
    //                 match self.status
    //             }
    //         }

    //         // match tile {
    //         //     // Tile::Mine => Span::styled(" X", Style::default().bg(Color::Black).fg(Color::Red)),
    //         //     Tile::Mine => match self.status {
    //         //         GameStatus::Playing => {
    //         //             match self.get_flag(x, y).ok_or(RenderError {})? {
    //         //                 TileFlag::Flagged => Span::styled(" F", Style::default().bg(Color::Black).fg(Color::Yellow)),
    //         //                 TileFlag::Guessing => Span::styled(" ?", Style::default().bg(Color::Black).fg(Color::Yellow)),
    //         //                 TileFlag::Empty => Span::raw("  "),
    //         //             }
    //         //         }
    //         //         GameStatus::Win
    //         //     }
    //         //     Tile::Tile(surrounding_mines) => Span::styled(
    //         //         format!(" {surrounding_mines}"),
    //         //         Style::default().fg(Color::White).bg(
    //         //             match self.status {
    //         //                 GameStatus::Win => todo!(),
    //         //             }
    //         //         ),
    //         //     ),
    //         // }
    //     });

    //     // Overwrite the cursor tile
    //     let cursor_tile = output_tiles.get_mut((self.cursor.x, self.cursor.y));
    //     if let Some(cursor_tile) = cursor_tile {
    //         cursor_tile.style.bg(Color::White).fg(Color::Black);
    //     }

    //     Ok(output_tiles
    //         .rows()
    //         .into_iter()
    //         .map(|row| Spans::from(row.iter().map(|span| span.clone()).collect::<Vec<Span>>()))
    //         .collect())
    //     // for (y, row) in output_tiles.rows().into_iter().enumerate() {
    //     //     for (x, span) in row.into_iter().enumerate() {
    //     //         // width is 2 because each tile is 1x2 in the terminal
    //     //         buf.set_span(
    //     //             area.x
    //     //
    //     // .saturating_add(x.saturating_mul(TILE_TERMINAL_WIDTH).try_into().
    //     // unwrap()),             area.y
    //     //                 .saturating_add(y.try_into().unwrap())
    //     //
    //     // .saturating_mul(TILE_TERMINAL_HEIGHT.try_into().unwrap()),
    //     //             span,
    //     //             2,
    //     //         );
    //     //     }
    //     // }
    // }
}

#[derive(Error, Debug)]
#[error("Error rendering minefield for tui.rs")]
pub struct RenderError {
}
