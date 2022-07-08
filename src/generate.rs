//! Methods to generate the layout of a minefield, including the mine placement
//! and each tile's surrounding mines.

use ndarray::Array2;
use rand::Rng;

use crate::minefield::{FieldDimension, TileMineState};

/// A simple minefield template which only has the mine placement.
#[derive(Debug)]
pub struct TemplateMinefield {
    /// The dimensions of the minefield.
    pub size: FieldDimension,
    /// The total amount of mines placed in the minefield.
    pub total_mines: usize,
    /// Two-dimensional array of the mine placement, and tile surrounding mine counts.
    pub layout: Array2<TileMineState>,
}
impl TemplateMinefield {
    /// Create a new minefield by randomly spreading a predefined amount of mines.
    pub fn new_randomly_spread(size: FieldDimension, mines: usize) -> Self {
        let mut total_mines = 0usize;
        let mut layout =
            Array2::from_shape_simple_fn((size.x, size.y), || TileMineState::Empty(0));

        // TODO: prevent infinite loops where there's more mines than total tiles
        //       idea for this: create an array that contains a list of every single index
        //       possible, and then pick randomly from that array, and then remove from the array
        //       every time a mine is placed. then if the array is empty, that means that there were too many mines
        //       ---
        //       another idea: add a guard that errors if (mines > size.x * size.y)
        while total_mines < mines {
            let mine_col = rand::thread_rng().gen_range(0..size.x);
            let mine_row = rand::thread_rng().gen_range(0..size.y);

            // TODO: maybe prevent the center tile from being a mine, that's a future
            // endeavor

            let mine_tile = layout.get_mut((mine_col, mine_row)).unwrap();
            // Skip tiles that are already mines
            if let TileMineState::Mine = mine_tile {
                continue;
            }
            
            total_mines += 1;
            *mine_tile = TileMineState::Mine;

            // Increment surrounding mines for the surrounding tiles
            for col in (mine_col.saturating_sub(1))..=(mine_col.saturating_add(1)) {
                if col > size.x - 1 {
                    continue;
                }

                for row in (mine_row.saturating_sub(1))..=(mine_row.saturating_add(1)) {
                    if row > size.y - 1 {
                        continue;
                    }

                    let tile = layout.get_mut((col, row)).unwrap();
                    if let TileMineState::Empty(surrounding) = tile {
                        *surrounding += 1;
                    }
                }
            }
        }

        Self {
            size,
            total_mines,
            layout,
        }
    }
}
