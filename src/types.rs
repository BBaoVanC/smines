//! Various types used for the Minesweeper game

use std::num::TryFromIntError;

#[derive(Debug)]
/// Object that holds a Minefield and everything in it.
pub struct Minefield {
    pub cols: u32,
    pub rows: u32,
}
impl Minefield {
    /// Create a new Minefield instance
    pub fn new(cols: u32, rows: u32) -> Self {
        Self { cols, rows }
    }

    /// Get the amount of columns required to display the minefield in a
    /// terminal.
    ///
    /// This returns u16 since that's the type that
    /// tui::layout::Constraint::Length takes.
    ///
    /// # Errors
    ///
    /// This function will return [`TryFromIntError`] if the required amount of terminal columns
    /// does not fit into a u16.
    pub fn display_cols(&self) -> Result<u16, TryFromIntError> {
        (self.cols * 2).try_into()
    }

    /// Get the amount of rows required to display the minefield in a terminal.
    ///
    /// This returns u16 since that's the type that
    /// tui::layout::Constraint::Length takes.
    ///
    /// # Errors
    ///
    /// This function will return [`TryFromIntError`] if the required amount of terminal rows
    /// does not fit into a u16.
    pub fn display_rows(&self) -> Result<u16, TryFromIntError> {
        (self.rows).try_into()
    }
}
