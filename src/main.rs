use clap::Parser;

use crossterm::{cursor, execute, terminal};

use std::{borrow::Borrow, io, panic};

use tui::{
    backend::Backend,
    backend::CrosstermBackend,
    layout::{Constraint, Direction, Layout, Rect},
    widgets::{Block, Borders, Widget},
    Frame, Terminal,
};

#[derive(Parser, Debug)]
#[clap(
    author,
    version,
    about,
    long_about = "Simple minesweeper in the terminal."
)]
struct Args {
    #[clap(short, long, value_parser, default_value_t = 16)]
    cols: u32,
    #[clap(short, long, value_parser, default_value_t = 16)]
    rows: u32,
    #[clap(short, long, value_parser, default_value_t = 40)]
    mine_count: u32,

    #[clap(short, long, value_parser, default_value_t = false)]
    allow_undo: bool,
}

fn setup_terminal() {
    execute!(
        io::stdout(),
        terminal::EnterAlternateScreen,
        terminal::Clear(terminal::ClearType::All),
        cursor::Hide,
    ).unwrap();
    terminal::enable_raw_mode().unwrap();
}

fn close_terminal() {
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

// Separate run() function to ensure that raw mode gets disabled even if there's an error
fn main() {
    let settings = better_panic::Settings::auto()
        .most_recent_first(false)
        .backtrace_first(false);
    settings.clone().install();

    panic::set_hook(Box::new(move |ctx| {
        close_terminal();
        settings.clone().create_panic_handler()(ctx);
    }));


    let args = Args::parse();

    let backend = CrosstermBackend::new(io::stdout());
    let mut terminal = Terminal::new(backend).expect("Failed to create terminal");


    setup_terminal();
    terminal.draw(|f| ui(f, args)).expect("Failed to draw UI");
    close_terminal();
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
