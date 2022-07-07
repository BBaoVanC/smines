//! Logic relating to handling minesweeper gameplay.

use ndarray::Array2;

use crate::minesweeper::Minefield;

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

#[derive(Clone, Copy, Debug)]
/// A cursor on the minefield.
pub struct Cursor {
    /// The horizontal position of the coordinate in tiles.
    pub x: usize,
    /// The vertical position of the coordinate in tiles.
    pub y: usize,
}
impl Cursor {
    pub fn display_x(&self) -> usize {
        self.x * 2
    }
    pub fn display_y(&self) -> usize {
        self.y
    }
}

#[derive(Debug)]
/// An actual minesweeper game, with its own state.
pub struct Game {
    /// The current stage of the game.
    pub status: GameStatus,
    pub flags: Array2<bool>,
    /// The underlying minefield schematic.
    pub minefield: Minefield,
    /// The cursor position on the minefield.
    pub cursor: Cursor,
}
impl Game {
    /// Create a new Minesweeper game.
    pub fn new(width: usize, height: usize, mine_count: usize) -> Self {
        Self {
            status: GameStatus::default(),
            minefield: Minefield::new(width, height).generate_mines_simple(mine_count),
            flags: Array2::from_shape_simple_fn((width, height), || false),
            cursor: Cursor {
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
