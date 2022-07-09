//! Stuff that handles an entire minesweeper game.

use ndarray::Array2;
use tui::{text::{Spans, Span}, style::Color};

use crate::minefield::{Minefield, TileMineState, TileFlagState};

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
pub struct Game {
    state: GameState,
    pub cursor: Cursor,
    minefield: Minefield,
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

    pub fn display(&self) -> Vec<Spans> {
        let mut output_array = self.minefield.tiles.map(|t| {
            let color = match (self.state, t.flag_state, t.mine_state) {
                (GameState::Playing, TileFlagState::Empty, TileMineState::Empty(s)) => match s {
                    0 => (Color::White, Color::Black), // foreground does not matter here so default to white
                    1 => (Color::White, Color::Blue),
                    2 => (Color::Black, Color::Green),
                    3 => (Color::White, Color::Red),
                    4 => (Color::Black, Color::Cyan),
                    5 => (Color::Black, Color::Yellow),
                    6 => (Color::Black, Color::Magenta),
                    7 => (Color::White, Color::Black),
                    8 => (Color::White, Color::Gray),
                    _ => unreachable!(),
                }
                _ => (Color::Black, Color::Black),
            };

            match (self.state, t.is_visible, t.flag_state, t.mine_state) {
                _ => {},
            }
    });


                
        

        //     let text = if !t.is_visible {
        //         match self.state {
        //             GameState::Playing => "  ",
                    
        //         }
        //     }
        //     let text = match (self.state, t.flag_state, t.mine_state) {
        //         (GameState::Playing | GameState::Win, TileFlagState::Flagged, _) => " F",
        //         (GameState::Playing | GameState::Win, TileFlagState::Suspicious, _) => " ?",
        //         (GameState::Playing, TileFlagState::Empty, _) => " ",

        //         (GameState::Loss | GameState::Win, TileFlagState::Empty, TileMineState::Mine) => " X",

        //         (GameState::Loss, TileFlagState::Flagged, TileMineState::Mine) => " F",
        //         (GameState::Loss, TileFlagState::Suspicious, TileMineState::Mine) => " ?",

        //         (GameState::Loss, TileFlagState::Flagged, TileMineState::Empty(_)) => "!F",
        //         (GameState::Loss, TileFlagState::Suspicious, TileMineState::Empty(_)) => "!?",

        //         (GameState::Win, TileFlagState::Empty, TileMineState::Empty(_)) => unreachable!(),
        //         (GameState::Loss, TileFlagState::Empty, TileMineState::Empty(_)) => unreachable!(),
        //     };
        // });

        vec![Spans::from(vec![Span::raw("test")])]
    }
}
