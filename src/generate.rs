//! Methods to generate the layout of a minefield, including the mine placement
//! and each tile's surrounding mines.

use ndarray::Array2;
use rand::{prelude::SliceRandom, Rng};

use crate::minefield::{FieldDimension, TileMineState};

/// A simple minefield template which only has the mine placement.
#[derive(Debug)]
pub struct TemplateMinefield {
    /// The dimensions of the minefield.
    pub size: FieldDimension,
    /// The total amount of mines placed in the minefield.
    pub total_mines: usize,
    /// Two-dimensional array of the mine placement, and tile surrounding mine
    /// counts.
    pub layout: Array2<TileMineState>,
}
impl TemplateMinefield {
    /// Create a new minefield by randomly spreading a predefined amount of
    /// mines.
    pub fn new_randomly_spread(size: FieldDimension, mines: usize) -> Self {
        let mut total_mines = 0usize;
        let mut layout = Array2::from_shape_simple_fn((size.x, size.y), || TileMineState::Empty(0));

        let mut rng = rand::thread_rng();

        let mut possible_cols: Vec<usize> = (0..size.x).collect();
        let mut possible_rows: Vec<usize> = (0..size.y).collect();
        possible_cols.shuffle(&mut rng);
        possible_rows.shuffle(&mut rng);

        // TODO: make this actually work
        for mine_col in &possible_cols {
            for mine_row in &possible_rows {
                let mine_tile = layout.get_mut((*mine_col, *mine_row)).unwrap();
                *mine_tile = TileMineState::Mine;
                total_mines += 1;

                // Increment surrounding mine count for each surrounding tile
                for col in (mine_col.saturating_sub(1))..=(mine_col.saturating_add(1)) {
                    if col > size.x - 1 {
                        continue;
                    }

                    for row in (mine_row.saturating_sub(1))..=(mine_row.saturating_add(1)) {
                        if row > size.y - 1 {
                            continue;
                        }

                        if let TileMineState::Empty(surrounding) =
                            layout.get_mut((col, row)).unwrap()
                        {
                            *surrounding += 1;
                        }
                    }
                }

                if total_mines >= mines {
                    break;
                }
            }
        }

        debug_assert_eq!(total_mines, mines);

        Self {
            size,
            total_mines,
            layout,
        }
    }
}
