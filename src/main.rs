use anyhow::Context;
use smines::{display::GameWidget, game::Game, constants::{TILE_TERMINAL_WIDTH, TILE_TERMINAL_HEIGHT}};
use std::io;

use clap::Parser;

use crossterm::{
    cursor,
    event::{self, Event, KeyCode},
    execute, terminal,
};

use tui::{
    backend::CrosstermBackend,
    layout::Rect,
    style::{Color, Style},
    widgets::{self, Borders, Paragraph},
    Terminal,
};

#[derive(Parser, Debug)]
#[clap(author, version)]
/// Simple minesweeper in the terminal.
///
/// This is the Rust rewrite version.
struct Args {
    // #[clap(short, long, value_parser, default_value_t = 16)]
    #[clap(short, long, value_parser, default_value_t = 4)]
    /// Total columns in the minefield
    ///
    /// This sets the amount of columns (lines stacked horizontally) in the
    /// minefield.
    cols: usize,
    // #[clap(short, long, value_parser, default_value_t = 16)]
    #[clap(short, long, value_parser, default_value_t = 4)]
    /// Total rows in the minefield
    ///
    /// This sets the amount of rows (lines stacked vertically) in the
    /// minefield.
    rows: usize,
    // #[clap(short, long, value_parser, default_value_t = 40)]
    #[clap(short, long, value_parser, default_value_t = 4)]
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

    // let minefield =
    //     minesweeper::Minefield::new(args.cols,
    // args.rows).generate_mines_simple(args.mine_count);

    let mut game = Game::new(args.cols, args.rows, args.mine_count);

    loop {
        terminal
            .draw(|f| {
                let width = (game.width() * TILE_TERMINAL_WIDTH).try_into().unwrap();
                let minefield_height = (game.height() * TILE_TERMINAL_HEIGHT).try_into().unwrap();

                let start_x = ((f.size().width) - width) / 2;
                let start_y = (f.size().height - (4 + minefield_height)) / 2;

                let scoreboard = Rect {
                    height: 4,
                    width,
                    x: start_x,
                    y: start_y,
                };

                let minefield = Rect {
                    height: minefield_height,
                    width,
                    x: start_x,
                    y: start_y + 4,
                };

                // Scoreboard
                f.render_widget(
                    widgets::Block::default()
                        .title("Scoreboard")
                        .borders(Borders::ALL),
                    scoreboard,
                );

                // Minefield
                // f.render_widget(GameWidget::new(&game), minefield);
                f.render_widget(Paragraph::new(game.render().unwrap()), minefield)
                // f.render_widget(
                //     widgets::Block::default()
                //         .title("Minefield")
                //         .borders(Borders::ALL),
                //     minefield,
                // );

                // // Short instructions
                // f.render_widget(
                //     widgets::Paragraph::new(constants::instructions()),
                //     instructions,
                // );
            })
            .context("Failed to draw UI")?;

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
