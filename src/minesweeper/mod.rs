//! Library that handles Minesweeper game logic

use ndarray::Array2;
use rand::Rng;
use std::{num::TryFromIntError, fmt::{Display, self}};


#[derive(Debug)]
/// Object that holds a Minefield's layout/properties.
///
/// This struct is not meant to hold the actual state of the Minefield, just the
/// underlying information about it, such as its dimensions, total mines, map of
/// the surrounding mines of each tile, and which tiles are mines themselves.
pub struct Minefield {
    width: usize,
    height: usize,
    mines: Option<usize>,
    pub tiles: Array2<TileSurroundingMines>,
}
impl Minefield {
    /// Create a new Minefield instance.
    ///
    /// This function does not generate any mines; be sure to use one of the
    /// generation functions to do so.
    pub fn new(width: usize, height: usize) -> Self {
        Self {
            width,
            height,
            mines: None,
            tiles: Array2::from_shape_simple_fn((width, height), || TileSurroundingMines::Tile(0)),
        }
    }

    /// Distribute a total amount of mines randomy onto a minefield.
    /// 
    /// # TODO
    /// 
    /// - Prevent infinite loop when `mines` > the total amount of tiles in the minefield
    /// - Consider 
    pub fn generate_mines_simple(mut self, mines: usize) -> Self {
        let mut placed_mines = 0;
        // TODO: if self.mines is Some, clear all tiles
        // TODO: prevent infinite loops where there's more mines than total tiles
        while placed_mines < mines {
            let mine_col = rand::thread_rng().gen_range(0..self.width);
            let mine_row = rand::thread_rng().gen_range(0..self.height);

            // TODO: maybe prevent the center tile from being a mine, that's a future endeavor

            let mine_tile = self.tiles.get_mut((mine_col, mine_row)).unwrap();
            if let TileSurroundingMines::Tile(_) = mine_tile {
                placed_mines += 1;
                *mine_tile = TileSurroundingMines::Mine;

                // Increment surrounding mines for the surrounding tiles
                for col in (mine_col.saturating_sub(1))..=(mine_col.saturating_add(1)) {
                    if col > self.width-1 {
                        continue;
                    }

                    for row in (mine_row.saturating_sub(1))..=(mine_row.saturating_add(1)) {
                        if row > self.height-1 {
                            continue;
                        }

                        let tile = self.tiles.get_mut((col, row)).unwrap();
                        if let TileSurroundingMines::Tile(surrounding) = tile {
                            *surrounding += 1;
                        }
                    }
                }
            }
        }

        self.mines = Some(mines);
        self
    }

    /// Distribute mines in the minefield based on a percentage chance that each
    /// tile is a mine.
    ///
    /// Because it's all based on random chance, it's entirely possible for
    /// every single tile to be a mine, or for no tiles to be a mine. Use with
    /// caution, and prefer other distribution methods instead.
    ///
    /// TODO: add some example to test the logic of `... < chance`
    ///
    /// DO NOT USE -- INCOMPLETE
    #[deprecated]
    pub fn generate_mines_by_percentage(mut self, chance: f64) -> Self {
        let mut mines: usize = 0;
        // No need to check if self.tiles is Some since this function will clear all
        // tiles completely anyways
        for tile in self.tiles.iter_mut() {
            if rand::random::<f64>() < chance {
                *tile = TileSurroundingMines::Mine;
                mines += 1;
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

#[derive(Clone, Copy, Debug, Default)]
pub enum TileState {
    Flagged,
    Visible,
    #[default]
    Hidden,
}

#[derive(Debug)]
pub enum TileSurroundingMines {
    Mine,
    Tile(u8),
}
impl Display for TileSurroundingMines {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if let Self::Tile(surrounding) = self {
            write!(f, " {}", surrounding)
        } else {
            write!(f, " \u{2588}")
        }
    }
}
// deliberately not Copy so it stays owned
// #[derive(Clone, Debug, Default)]
// pub struct Tile {
//     is_mine: bool,
//     surrounding_mines: Option<u8>,
// }
// impl Tile {
//     pub fn is_mine(&self) -> bool {
//         self.is_mine
//     }

//     pub fn set_mine(&mut self) {
//         self.is_mine = true;
//     }
//     pub fn unset_mine(&mut self) {
//         self.is_mine = false;
//     }

//     pub fn set_surrounding_mines(&mut self, surrounding: u8) {
//         self.surrounding_mines = Some(surrounding);
//     }
//     pub fn unset_surrounding_mines(&mut self) {
//         self.surrounding_mines = None;
//     }
// }
