use std::{
    borrow::Cow,
    io::{self, Write},
    num::TryFromIntError,
};

use crossterm::{
    cursor,
    style::{Color, Print, PrintStyledContent, StyledContent, Stylize},
    terminal::{self, ClearType},
    QueueableCommand,
};
use thiserror::Error;

use super::{
    colors::{get_color, ColorType},
    constants::{TILE_TERMINAL_HEIGHT, TILE_TERMINAL_WIDTH},
    Terminal,
};
use crate::minesweeper::{
    game::{Game, GameState},
    minefield::{Tile, TileFlagState, TileMineState},
};

#[derive(Debug, Error)]
pub enum DrawError {
    #[error("error while writing")]
    WriteError(#[from] io::Error),
    #[error("failed to query the terminal size")]
    SizeQueryError(#[source] crossterm::ErrorKind),
    #[error("error converting a number while drawing minefield")]
    NumberConversionError(#[from] TryFromIntError),
}

impl Terminal {
    pub fn draw(&mut self, game: &Game) -> Result<(), DrawError> {
        self.stdout.queue(terminal::Clear(ClearType::All))?;
        let size = terminal::size().map_err(DrawError::SizeQueryError)?;

        // Add 2 to include minefield borders
        // Add 4 to y to include scoreboard + its borders
        let min_cols = (game.size().x * TILE_TERMINAL_WIDTH) + 2;
        let min_rows = game.size().y + 4 + 2;

        if (usize::from(size.0) < min_cols) || (usize::from(size.1) < min_rows) {
            self.stdout.queue(Print(format!(
                "Please make your terminal at least {min_cols} cols by {min_rows} rows."
            )))?;
            self.stdout.queue(Print(format!(
                "Current size: {} cols by {} rows",
                size.0, size.1
            )))?;

            self.stdout.flush()?;
            return Ok(());
        }

        let start_x = (size.0 as usize - game.size().x) / 2;
        let start_y = (size.1 as usize - game.size().y) / 2;

        for (y, row) in game.minefield.tiles.rows().into_iter().enumerate() {
            self.stdout.queue(cursor::MoveTo(
                start_x.try_into()?,
                (start_y + y).try_into()?,
            ))?;
            for tile in row {
                self.stdout
                    .queue(PrintStyledContent(tile.display(game.state)))?;
            }
        }

        let cursor_tile_pos: (u16, u16) = (game.cursor.x.try_into()?, game.cursor.y.try_into()?);
        let cursor_x = start_x as u16 + (cursor_tile_pos.0 * TILE_TERMINAL_WIDTH as u16);
        let cursor_y = start_y as u16 + (cursor_tile_pos.1 * TILE_TERMINAL_HEIGHT as u16);
        self.stdout.queue(cursor::MoveTo(cursor_x, cursor_y))?;
        self.stdout.queue(PrintStyledContent(
            game.minefield.tiles[[game.cursor.x, game.cursor.y]]
                .display(game.state)
                .with(Color::Black)
                .on(Color::White),
        ))?;

        self.stdout.flush()?;

        Ok(())
    }
}

impl Tile {
    pub fn display(&self, game_state: GameState) -> StyledContent<Cow<'static, str>> {
        let output: (Cow<_>, ColorType) = match (
            game_state,
            self.flag_state,
            self.mine_state,
            self.is_visible,
        ) {
            // // DEBUGGING
            // (GameState::Playing, TileFlagState::Empty, TileMineState::Mine, false) => {
            //     (" X".into(), ColorType::TileFlag)
            // }
            // (GameState::Playing, TileFlagState::Empty, TileMineState::Empty(0), false) => {
            //     ("  ".into(), ColorType::TileNormal(0))
            // }
            // (GameState::Playing, TileFlagState::Empty, TileMineState::Empty(s), false) => {
            //     (format!(" {}", s).into(), ColorType::TileNormal(s))
            // }

            // Hidden tiles (the mine state should be ignored)
            (GameState::Playing, TileFlagState::Empty, _, false) => {
                (" ?".into(), ColorType::TileHidden)
            }
            // Flagged tiles (the mine state should be ignored)
            (GameState::Playing, TileFlagState::Flagged, _, false) => {
                (" F".into(), ColorType::TileFlag)
            }
            (GameState::Playing, TileFlagState::Suspicious, _, false) => {
                (" ?".into(), ColorType::TileFlag)
            }
            // Visible tiles while still alive (meaning they have to be safe)
            // These tiles don't change visually when you win/lose
            (_, TileFlagState::Empty, TileMineState::Empty(0), true) => {
                ("  ".into(), ColorType::TileNormal(0))
            }
            (_, TileFlagState::Empty, TileMineState::Empty(s @ 0..=8), true) => {
                (format!(" {}", s).into(), ColorType::TileNormal(s))
            }

            // Correctly flagged tiles when losing
            (GameState::Loss, TileFlagState::Flagged, TileMineState::Mine, false) => {
                (" F".into(), ColorType::TileFlag)
            }
            (GameState::Loss, TileFlagState::Suspicious, TileMineState::Mine, false) => {
                (" ?".into(), ColorType::TileFlag)
            }
            // Incorrectly flagged tiles when losing
            (GameState::Loss, TileFlagState::Flagged, TileMineState::Empty(_s), false) => {
                (" F".into(), ColorType::TileFlagWrong)
            }
            (GameState::Loss, TileFlagState::Suspicious, TileMineState::Empty(_s), false) => {
                (" ?".into(), ColorType::TileFlagWrong)
            }
            // Unflagged mines when losing (show them if they're hidden)
            (GameState::Loss, TileFlagState::Empty, TileMineState::Mine, _) => {
                (" X".into(), ColorType::TileMineDead)
            }
            // Hidden tiles when losing
            // The TileMineState::Mine version of this is handled right above
            (GameState::Loss, TileFlagState::Empty, TileMineState::Empty(_s), false) => {
                (" ?".into(), ColorType::TileHidden)
            }

            // The player won and there were some unflagged mines
            (GameState::Win, TileFlagState::Empty, TileMineState::Mine, false) => {
                (" X".into(), ColorType::TileMineSafe)
            }
            // Correct flags when winning (this should be every flag)
            (GameState::Win, TileFlagState::Flagged, TileMineState::Mine, false) => {
                (" F".into(), ColorType::TileFlagCorrect)
            }
            (GameState::Win, TileFlagState::Suspicious, TileMineState::Mine, false) => {
                (" ?".into(), ColorType::TileFlagCorrect)
            }

            // A flagged tile cannot be visible
            (_, TileFlagState::Flagged | TileFlagState::Suspicious, _, true) => {
                unreachable!()
            }
            // The player can't be alive if a mine has been revealed
            (GameState::Playing, TileFlagState::Empty, TileMineState::Mine, true) => {
                unreachable!()
            }
            // If the player won, there can't be any visible mines
            (GameState::Win, TileFlagState::Empty, TileMineState::Mine, true) => {
                unreachable!()
            }
            // If the player won, there can't be any incorrect flags
            (
                GameState::Win,
                TileFlagState::Flagged | TileFlagState::Suspicious,
                TileMineState::Empty(_s),
                false,
            ) => unreachable!(),
            // If the player won, every tile that's not a mine (safe) must be visible
            (GameState::Win, TileFlagState::Empty, TileMineState::Empty(_s), false) => {
                unreachable!()
            }
            // A tile can't have more than 8 surrounding mines
            (_, _, TileMineState::Empty(9..=u8::MAX), _) => unreachable!(),
        };

        get_color(output.1).apply(output.0)
    }
}
