//! Library that handles Minesweeper game logic

pub mod state;

use ndarray::Array2;
use rand::Rng;
use std::{
    fmt::{self, Display},
    num::TryFromIntError,
};

#[derive(Debug)]
/// Object that holds a Minefield's layout/properties.
///
/// This struct is not meant to hold the actual state of the Minefield, just the
/// underlying information about it, such as its dimensions, total mines, map of
/// the surrounding mines of each tile, and which tiles are mines themselves.
/// 
/// You can access a tile by index by using `minefield.tiles.get((col, row))`.
/// 
/// TODO: Write a test/demo
pub struct Minefield {
    width: usize,
    height: usize,
    mines: Option<usize>,
    /// Two-dimensional array that holds all the [`Tile`]s in the [`Minefield`].
    pub tiles: Array2<Tile>,
}
impl Minefield {
    /// Create a new Minefield struct.
    ///
    /// This function does not generate any mines; be sure to use one of the
    /// generation functions to do so.
    pub fn new(width: usize, height: usize) -> Self {
        Self {
            width,
            height,
            mines: None,
            tiles: Array2::from_shape_simple_fn((width, height), || Tile::Tile(0)),
        }
    }

    /// Distribute a total amount of mines randomy onto a minefield.
    ///
    /// # TODO
    ///
    /// - Prevent infinite loop when `mines` > the total amount of tiles in the
    ///   minefield
    /// - Consider
    pub fn generate_mines_simple(mut self, mines: usize) -> Self {
        let mut placed_mines = 0;
        // TODO: if self.mines is Some, clear all tiles
        // TODO: prevent infinite loops where there's more mines than total tiles
        while placed_mines < mines {
            let mine_col = rand::thread_rng().gen_range(0..self.width);
            let mine_row = rand::thread_rng().gen_range(0..self.height);

            // TODO: maybe prevent the center tile from being a mine, that's a future
            // endeavor

            let mine_tile = self.tiles.get_mut((mine_col, mine_row)).unwrap();
            if let Tile::Tile(_) = mine_tile {
                placed_mines += 1;
                *mine_tile = Tile::Mine;

                // Increment surrounding mines for the surrounding tiles
                for col in (mine_col.saturating_sub(1))..=(mine_col.saturating_add(1)) {
                    if col > self.width - 1 {
                        continue;
                    }

                    for row in (mine_row.saturating_sub(1))..=(mine_row.saturating_add(1)) {
                        if row > self.height - 1 {
                            continue;
                        }

                        let tile = self.tiles.get_mut((col, row)).unwrap();
                        if let Tile::Tile(surrounding) = tile {
                            *surrounding += 1;
                        }
                    }
                }
            }
        }

        self.mines = Some(mines);
        self
    }

    /// Get the amount of columns (the width of the minefield in tiles)
    pub fn width(&self) -> usize {
        self.width
    }

    /// Get the amount of rows (the height of the minefield in tiles)
    pub fn height(&self) -> usize {
        self.height
    }

    /// Get the amount of terminal columns required to display the minefield
    ///
    /// This returns u16 since that's the type that
    /// tui::layout::Constraint::Length takes.
    ///
    /// # Errors
    ///
    /// This function will return [`TryFromIntError`] if the required amount of
    /// terminal columns does not fit into a u16.
    pub fn display_cols(&self) -> Result<u16, TryFromIntError> {
        (self.width * 2).try_into()
    }

    /// Get the amount of terminal rows required to display the minefield.
    ///
    /// This returns u16 since that's the type that
    /// tui::layout::Constraint::Length takes.
    ///
    /// # Errors
    ///
    /// This function will return [`TryFromIntError`] if the required amount of
    /// terminal rows does not fit into a u16.
    pub fn display_rows(&self) -> Result<u16, TryFromIntError> {
        (self.height).try_into()
    }
}

/// Simple way to print out what a minefield looks like, mainly for debugging.
impl Display for Minefield {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "Minefield:")?;
        writeln!(f, "width: {},", self.width)?;
        writeln!(f, "height: {},", self.height)?;
        writeln!(f, "mines: {:?},", self.mines)?;
        writeln!(f, "------------------------")?;
        for row in self.tiles.rows() {
            for tile in row {
                write!(f, "{}", tile)?;
            }
            writeln!(f)?;
        }
        writeln!(f)?;
        Ok(())
    }
}

#[derive(Debug)]
/// Object that represents a tile in a Minefield.
///
/// This enum does not handle any game state and is purely informational.
pub enum Tile {
    /// This tile is a mine.
    ///
    /// Mines do not need to show how many other mines are surrounding it, so
    /// there's no need to store it.
    Mine,
    /// This tile is not a mine.
    ///
    /// This variant takes a u8, but it should never have a value > 8
    Tile(u8),
}
/// Simple way to print out what a tile looks like, mainly for debugging.
impl Display for Tile {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if let Self::Tile(surrounding) = self {
            write!(f, " {}", surrounding)
        } else {
            write!(f, " \u{2588}")
        }
    }
}

/// A basic two-dimensional coordinate.
pub struct Coordinate {
    /// The horizontal position of the coordinate.
    pub x: usize,
    /// The vertical position of the coordinate.
    pub y: usize,
}
