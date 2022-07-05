use anyhow::Context;
use smines::{constants, minesweeper};
use std::io;

use clap::Parser;

use crossterm::{
    cursor,
    event::{self, Event, KeyCode},
    execute, terminal,
};

use tui::{
    backend::CrosstermBackend,
    layout::{Constraint, Direction, Layout},
    widgets::{self, Borders},
    Terminal,
};

#[derive(Parser, Debug)]
#[clap(author, version)]
/// Simple minesweeper in the terminal.
///
/// This is the Rust rewrite version.
struct Args {
    #[clap(short, long, value_parser, default_value_t = 16)]
    /// Total columns in the minefield
    ///
    /// This sets the amount of columns (lines stacked horizontally) in the
    /// minefield.
    cols: usize,
    #[clap(short, long, value_parser, default_value_t = 16)]
    /// Total rows in the minefield
    ///
    /// This sets the amount of rows (lines stacked vertically) in the
    /// minefield.
    rows: usize,
    #[clap(short, long, value_parser, default_value_t = 40)]
    /// Total mines in the minefield
    ///
    /// This sets the amount of mines that will be distributed in the minefield.
    mine_count: usize,

    #[clap(short, long, value_parser)]
    /// Should you be allowed to undo your last move?
    ///
    /// If true, then you can press `u` to undo your last move. Pressing `u`
    /// again will "undo the undo" -- meaning that you'll be back to your
    /// original move.
    allow_undo: bool,
}

struct Term;
impl Term {
    fn new() -> Self {
        execute!(
            io::stdout(),
            terminal::EnterAlternateScreen,
            terminal::Clear(terminal::ClearType::All),
            cursor::Hide,
        )
        .unwrap();
        terminal::enable_raw_mode().unwrap();
        Self {}
    }
}
impl Drop for Term {
    fn drop(&mut self) {
        execute!(
            io::stdout(),
            cursor::MoveTo(0, 0),
            terminal::Clear(terminal::ClearType::All),
            terminal::LeaveAlternateScreen,
            cursor::Show,
        )
        .unwrap();
        terminal::disable_raw_mode().unwrap();
    }
}

// Separate run() function to ensure that raw mode gets disabled even if there's
// an error
fn main() -> anyhow::Result<()> {
    let args = Args::parse();

    let _term = Term::new();

    let backend = CrosstermBackend::new(io::stdout());
    let mut terminal = Terminal::new(backend).context("Failed to create terminal")?;

    let minefield = minesweeper::Minefield::new(args.cols, args.rows);

    loop {
        terminal
            .draw(|f| {
                // Add 2 to everything so it fits the borders too
                let horizontal = Layout::default()
                    .direction(Direction::Horizontal)
                    .constraints([
                        Constraint::Min(1 * 2),
                        Constraint::Length(minefield.display_cols().unwrap() + 2),
                        Constraint::Min(1 + 2),
                    ])
                    .split(f.size());

                let chunks = Layout::default()
                    .direction(Direction::Vertical)
                    .constraints([
                        Constraint::Min(1),
                        Constraint::Length(4 + 2),
                        Constraint::Length(minefield.display_rows().unwrap() + 2),
                        Constraint::Length(4 + 2),
                        Constraint::Min(1),
                    ])
                    .split(horizontal[1]);

                let (scoreboard, minefield, instructions) = (chunks[1], chunks[2], chunks[3]);

                // Scoreboard
                f.render_widget(
                    widgets::Block::default()
                        .title("Scoreboard")
                        .borders(Borders::ALL),
                    scoreboard,
                );

                // Minefield
                f.render_widget(
                    widgets::Block::default()
                        .title("Minefield")
                        .borders(Borders::ALL),
                    minefield,
                );

                // Short instructions
                f.render_widget(
                    widgets::Paragraph::new(constants::instructions()),
                    instructions,
                )
            })
            .context("Failed to draw UI")?;

        if let Event::Key(key) = event::read()? {
            if let KeyCode::Char('q') = key.code {
                break;
            }
        }
    }

    Ok(())
}
