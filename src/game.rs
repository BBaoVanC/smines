//! Stuff that handles an entire minesweeper game.

use crate::{
    colors::{get_color, ColorType},
    minefield::{FieldDimension, Minefield, TileFlagState, TileMineState},
};

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

    pub fn size(&self) -> FieldDimension {
        self.minefield.size
    }

    /*
    #[deprecated]
    pub fn display(&self) -> Vec<Spans> {
        let output_array = self.minefield.tiles.map(|t| {
            // TODO: think of more cases that are unreachable and mark them
            let raw = match (self.state, t.flag_state, t.mine_state, t.is_visible) {
                // Revealed tiles that are safe. They don't change visually based on GameState.
                (_, TileFlagState::Empty, TileMineState::Empty(s), true) => {
                    (get_color_from_surrounding(s), get_str_from_surrounding(s))
                }

                (GameState::Playing, TileFlagState::Empty, _, false) => {
                    (get_color(ColorType::TileHidden), "  ")
                }
                (GameState::Playing, TileFlagState::Flagged, _, false) => {
                    (get_color(ColorType::TileFlag), " F")
                }
                (GameState::Playing, TileFlagState::Suspicious, _, false) => {
                    (get_color(ColorType::TileFlag), " ?")
                }

                (GameState::Loss, TileFlagState::Empty, TileMineState::Mine, _) => {
                    (get_color(ColorType::TileMineDead), " X")
                }
                (GameState::Loss, TileFlagState::Empty, TileMineState::Empty(_), false) => {
                    (get_color(ColorType::TileHidden), "  ")
                }
                (GameState::Loss, TileFlagState::Flagged, TileMineState::Empty(_), _) => {
                    (get_color(ColorType::TileFlagWrong), "!F")
                }
                (GameState::Loss, TileFlagState::Suspicious, TileMineState::Empty(_), _) => {
                    (get_color(ColorType::TileFlagWrong), "!?")
                }
                (GameState::Loss, TileFlagState::Flagged, TileMineState::Mine, false) => {
                    (get_color(ColorType::TileFlag), " F")
                }
                (GameState::Loss, TileFlagState::Suspicious, TileMineState::Mine, false) => {
                    (get_color(ColorType::TileFlag), " ?")
                }

                (GameState::Win, TileFlagState::Empty, TileMineState::Mine, _) => {
                    (get_color(ColorType::TileMineSafe), " X")
                }
                (GameState::Win, TileFlagState::Flagged, TileMineState::Mine, false) => {
                    (get_color(ColorType::TileFlagCorrect), " F")
                }
                (GameState::Win, TileFlagState::Suspicious, TileMineState::Mine, false) => {
                    (get_color(ColorType::TileFlagCorrect), " ?")
                }

                // A tile that's been flagged cannot be revealed. Not even in a win/loss.
                (_, TileFlagState::Flagged | TileFlagState::Suspicious, _, true) => unreachable!(),
                // Winning requires every non-mine tile to be visible.
                (GameState::Win, TileFlagState::Empty, TileMineState::Empty(_), false) => {
                    unreachable!()
                }
                // Winning is impossible if any of the flags are incorrect.
                (
                    GameState::Win,
                    TileFlagState::Flagged | TileFlagState::Suspicious,
                    TileMineState::Empty(_),
                    _,
                ) => unreachable!(),
                // If a tile that's a mine is revealed, then the game cannot still be in progress
                // (it has to be a loss).
                (GameState::Playing, TileFlagState::Empty, TileMineState::Mine, true) => {
                    unreachable!()
                }
            };
            Span::styled(raw.1, raw.0)
        });

        // Draw cursor
        output_array[[self.cursor.x, self.cursor.y]]
            .style
            .fg(Color::Black)
            .bg(Color::White);

        output_array
            .rows()
            .into_iter()
            .map(|r| {
                Spans::from(
                    // TODO: find a better way to do this without cloning every span
                    r.iter().cloned().collect::<Vec<Span>>(),
                )
            })
            .collect()
    }
    */
}
