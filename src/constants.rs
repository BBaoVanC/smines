//! Some strings that don't change, such as instructions

use crossterm::style::{Color, StyledContent, Stylize};

/// Text which contains the instructions for the game.
///
/// TODO: write a test
pub fn instructions() -> Vec<Vec<StyledContent<&'static str>>> {
    vec![
        vec![
            "Use ".stylize(),
            "hjkl".with(Color::Cyan),
            " or arrow keys to move.".stylize(),
        ],
        vec![
            "Use ".stylize(),
            "<space>".with(Color::Cyan),
            " to reveal the tile under the cursor.".stylize(),
        ],
    ]

    // vec![
    //     Spans::from(vec![
    //         Span::raw("Use "),
    //         Span::styled("hjkl", Style::default().fg(Color::Cyan)),
    //         Span::raw(" or arrow keys to move."),
    //     ]),
    //     Spans::from(vec![
    //         Span::raw("Use "),
    //         Span::styled("<space>", Style::default().fg(Color::Cyan)),
    //         Span::raw(" to reveal the tile under the cursor."),
    //     ]),
    // ]
}

pub const TILE_TERMINAL_WIDTH: usize = 2;
pub const TILE_TERMINAL_HEIGHT: usize = 1;
