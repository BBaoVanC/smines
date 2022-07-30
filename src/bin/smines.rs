use std::io;

use anyhow::Context;
use clap::Parser;
use crossterm::event::{self, Event, KeyCode};
use smines::{
    minesweeper::{
        game::Game,
        minefield::{FieldDimension, Minefield},
    },
    terminal::Terminal,
};

/// Simple minesweeper in the terminal.
///
/// This is the Rust rewrite version.
#[derive(Parser, Debug)]
#[clap(author, version)]
struct Args {
    /// Total columns in the minefield
    ///
    /// This sets the amount of columns (lines stacked horizontally) in the
    /// minefield.
    // #[clap(short, long, value_parser, default_value_t = 4)]
    #[clap(short, long, value_parser, default_value_t = 16)]
    cols: usize,
    /// Total rows in the minefield
    ///
    /// This sets the amount of rows (lines stacked vertically) in the
    /// minefield.
    // #[clap(short, long, value_parser, default_value_t = 4)]
    #[clap(short, long, value_parser, default_value_t = 16)]
    rows: usize,
    /// Total mines in the minefield
    ///
    /// This sets the amount of mines that will be distributed in the minefield.
    // #[clap(short, long, value_parser, default_value_t = 4)]
    #[clap(short, long, value_parser, default_value_t = 40)]
    mine_count: usize,

    /// Should you be allowed to undo your last move?
    ///
    /// If true, then you can press `u` to undo your last move. Pressing `u`
    /// again will "undo the undo" -- meaning that you'll be back to your
    /// original move.
    #[clap(short, long, value_parser)]
    allow_undo: bool,
}

// Separate run() function to ensure that raw mode gets disabled even if there's
// an error
fn main() -> anyhow::Result<()> {
    // let args = Args::parse();
    let args = Args {
        allow_undo: true,
        cols: 16,
        rows: 16,
        mine_count: 40,
    };

    let mut term = Terminal::init(io::stdout()).context("Failed to initialize terminal.")?;

    let mut game = Game::from_minefield(Minefield::new(
        FieldDimension {
            x: args.cols,
            y: args.rows,
        },
        args.mine_count,
    )?);

    loop {
        term.draw(&game).context("Failed to draw to terminal.")?;

        if let Event::Key(key) = event::read()? {
            match key.code {
                KeyCode::Char('q') => break,

                // Movement
                KeyCode::Char('h') => game.cursor.x -= 1,
                KeyCode::Char('j') => game.cursor.y += 1,
                KeyCode::Char('k') => game.cursor.y -= 1,
                KeyCode::Char('l') => game.cursor.x += 1,
                _ => (),
            }
        }
    }

    Ok(())
}
