//! Stuff that handles an entire minesweeper game.

use crate::minesweeper::minefield::{FieldDimension, Minefield};

/// The current status of the game.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub enum GameState {
    /// The player won the game.
    Win,
    /// The player lost the game.
    Loss,
    /// The game is still in progress.
    #[default]
    Playing,
}

/// A cursor which has a tile selected on the minefield.
#[derive(Clone, Copy, Debug)]
pub struct Cursor {
    /// The horizontal position.
    pub x: usize,
    /// The vertical position.
    pub y: usize,
}
impl Cursor {
    pub fn new(minefield: &Minefield) -> Self {
        let x = minefield.size.x / 2;
        let y = minefield.size.y / 2;

        Self { x, y }
    }
}

#[derive(Debug)]
#[non_exhaustive]
pub struct Game {
    pub state: GameState,
    pub cursor: Cursor,
    pub minefield: Minefield,
}
impl Game {
    /// Create a game instance from an existing minefield instance.
    pub fn from_minefield(minefield: Minefield) -> Self {
        Self {
            state: GameState::default(),
            cursor: Cursor::new(&minefield),
            minefield,
        }
    }

    pub fn size(&self) -> FieldDimension {
        self.minefield.size
    }
}
