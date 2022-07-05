use anyhow::Context;
use std::{borrow::Borrow, io};

use clap::Parser;

use crossterm::{cursor, execute, terminal};

use tui::{
    backend::Backend,
    backend::CrosstermBackend,
    layout::{Constraint, Direction, Layout, Rect},
    widgets::{Block, Borders, Widget},
    Frame, Terminal,
};

#[derive(Parser, Debug)]
#[clap(author, version)]
/// Simple minesweeper in the terminal.
struct Args {
    #[clap(short, long, value_parser, default_value_t = 16)]
    /// Total columns in the minefield
    ///
    /// This sets the amount of columns (lines stacked horizontally) in the
    /// minefield.
    cols: u32,
    #[clap(short, long, value_parser, default_value_t = 16)]
    /// Total rows in the minefield
    ///
    /// This sets the amount of rows (lines stacked vertically) in the
    /// minefield.
    rows: u32,
    #[clap(short, long, value_parser, default_value_t = 40)]
    /// Total mines in the minefield
    ///
    /// This sets the amount of mines that will be distributed in the minefield.
    mine_count: u32,

    #[clap(short, long, value_parser)]
    /// Should you be allowed to undo your last move?
    ///
    /// If true, then you can press `u` to undo your last move. Pressing `u`
    /// again will "undo the undo" -- meaning that you'll be back to your
    /// original move.
    allow_undo: bool,
}

struct Term {}
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

    {
        let _term = Term::new();

        let backend = CrosstermBackend::new(io::stdout());
        let mut terminal = Terminal::new(backend).context("Failed to create terminal")?;

        terminal
            .draw(|f| ui(f, &args))
            .context("Failed to draw UI")?;
        std::thread::sleep(std::time::Duration::from_secs(3));
    }

    println!("{:?}", &args);

    Ok(())
}

fn ui<B: Backend, C: Borrow<Args>>(f: &mut Frame<B>, config: C) {
    let config = config.borrow();
    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .margin(1)
        .constraints([
            Constraint::Length(4),
            Constraint::Length(config.rows.try_into().unwrap()),
            Constraint::Length(4),
        ])
        .split(Rect {
            x: 0,
            y: 0,
            width: config.cols.try_into().unwrap(),
            height: config.rows.try_into().unwrap(),
        });

    // Scoreboard
    f.render_widget(
        Block::default().title("Scoreboard").borders(Borders::ALL),
        chunks[0],
    );

    // Minefield
    f.render_widget(
        Block::default().title("Minefield").borders(Borders::ALL),
        chunks[1],
    );

    // Short instructions
    f.render_widget(
        Block::default().title("Instructions").borders(Borders::ALL),
        chunks[2],
    );
}
