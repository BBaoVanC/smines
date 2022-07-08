//! Methods to generate the layout of a minefield, including the mine placement
//! and each tile's surrounding mines.

use ndarray::Array2;
use rand::Rng;

use crate::minefield::{Dimension, TileMineStatus};

#[derive(Debug)]
pub struct TemplateMinefield {
    pub size: Dimension,
    pub total_mines: usize,
    pub layout: Array2<TileMineStatus>,
}
impl TemplateMinefield {
    pub fn new(size: Dimension, mines: usize) -> Self {
        let mut total_mines = 0usize;
        let mut layout =
            Array2::from_shape_simple_fn((size.x, size.y), || TileMineStatus::Empty(0));

        // TODO: if self.mines is Some, clear all tiles
        // TODO: prevent infinite loops where there's more mines than total tiles
        while total_mines < mines {
            let mine_col = rand::thread_rng().gen_range(0..size.x);
            let mine_row = rand::thread_rng().gen_range(0..size.y);

            // TODO: maybe prevent the center tile from being a mine, that's a future
            // endeavor

            let mine_tile = layout.get_mut((mine_col, mine_row)).unwrap();
            if let TileMineStatus::Empty(_) = mine_tile {
                total_mines += 1;
                *mine_tile = TileMineStatus::Mine;

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
                        if let TileMineStatus::Empty(surrounding) = tile {
                            *surrounding += 1;
                        }
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
