//! Stuff that handles an entire minesweeper game.

use crate::minefield::{Minefield, Tile};


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
pub struct Cursor<'c> {
    /// The horizontal position.
    pub x: usize,
    /// The vertical position.
    pub y: usize,
    /// A reference to the tile the cursor is currently hovering on.
    pub selected_tile: &'c Tile,
}
impl<'c> Cursor<'_> {
    /// TODO: get rid of the unwrap for selected_tile
    pub fn new(minefield: &'c Minefield) -> Self {
        let x = minefield.size.x / 2;
        let y = minefield.size.y / 2;

        Self {
            x,
            y,
            selected_tile: &minefield.tiles[[x, y]],
        }
    }
}

#[derive(Debug)]
pub struct Game<'g> {
    state: GameState,
    minefield: Minefield,
    cursor: Cursor<'g>,
}
impl Game<'_> {
    // pub fn from_minefield(minefield: Minefield) -> Self {
    //     Self {
    //         state: GameState::default(),
    //         //cursor: Cursor::new(&minefield),
    //         minefield,
    //     }
    // }

    pub fn state(&self) -> GameState {
        self.state
    }

    pub fn minefield(&self) -> &Minefield {
        &self.minefield
    }
}

