//! Module with everything relating to drawing the minesweeper game into the
//! terminal using [`crossterm`].

pub mod colors;
pub mod constants;

use std::io::{self, Stdout, Write};

use crossterm::{
    cursor,
    style::Print,
    terminal::{self, ClearType},
    QueueableCommand,
};
use thiserror::Error;

use crate::{minesweeper::game::Game, terminal::constants::TILE_TERMINAL_WIDTH};

// Prevent this struct from being created without using init()
#[non_exhaustive]
pub struct Terminal {
    pub stdout: Stdout,
    // pub game: &'t Game,
}
impl Terminal {
    pub fn init(mut stdout: Stdout) -> Result<Terminal, io::Error> {
        stdout
            .queue(terminal::EnterAlternateScreen)?
            .queue(terminal::Clear(ClearType::All))?
            .queue(cursor::Hide)?
            .flush()?;
        terminal::enable_raw_mode()?;

        // Ok(Self { stdout, game })
        Ok(Self { stdout })
    }

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

        self.stdout.queue(Print("This is cool"))?;
        self.stdout.flush()?;

        Ok(())
    }
}
impl Drop for Terminal {
    fn drop(&mut self) {
        self.stdout
            .queue(cursor::MoveTo(0, 0))
            .unwrap()
            .queue(terminal::Clear(terminal::ClearType::All))
            .unwrap()
            .queue(terminal::LeaveAlternateScreen)
            .unwrap()
            .queue(cursor::Show)
            .unwrap()
            .flush()
            .unwrap();
        terminal::disable_raw_mode().unwrap();
    }
}

#[derive(Debug, Error)]
pub enum DrawError {
    #[error("error while writing")]
    WriteError(#[from] io::Error),
    #[error("failed to query the terminal size")]
    SizeQueryError(#[source] crossterm::ErrorKind),
}
