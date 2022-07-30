//! Module with everything relating to drawing the minesweeper game into the
//! terminal using [`crossterm`].

pub mod colors;
pub mod constants;
pub mod draw;

use std::io::{self, Stdout, Write};

use crossterm::{
    cursor,
    terminal::{self, ClearType},
    QueueableCommand,
};

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
