//! Logic relating to handling minesweeper gameplay.

use ndarray::Array2;

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

    /// Get the amount of terminal columns required to display the minefield
    ///
    /// This is equal to twice the width, since each tile is 2 terminal columns
    /// wide.
    pub fn display_cols(&self) -> usize {
        self.minefield.width() * 2
    }
    /// Get the amount of terminal rows required to display the minefield.
    ///
    /// This is equal to the height, since each tile is 1 terminal row wide.
    pub fn display_rows(&self) -> usize {
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
}
