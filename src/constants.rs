//! Some strings that don't change, such as instructions

use tui::{
    style::{Color, Style},
    text::{Span, Spans},
};

/// Text which contains the instructions for the game.
///
/// TODO: write a test
pub fn instructions() -> Vec<Spans<'static>> {
    vec![
        Spans::from(vec![
            Span::raw("Use "),
            Span::styled("hjkl", Style::default().fg(Color::Cyan)),
            Span::raw(" or arrow keys to move."),
        ]),
        Spans::from(vec![
            Span::raw("Use "),
            Span::styled("<space>", Style::default().fg(Color::Cyan)),
            Span::raw(" to reveal the tile under the cursor."),
        ]),
    ]
}
