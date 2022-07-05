//! Library that handles Minesweeper game logic

use ndarray::Array2;
use std::num::TryFromIntError;

#[derive(Debug)]
/// Object that holds a Minefield and everything in it.
pub struct Minefield {
    width: usize,
    height: usize,
    mines: Option<usize>,
    pub tiles: Array2<Tile>,
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
            // tiles: vec![vec![Tile::new(false); height]; width],
            // tiles: Array2D::filled_with(Tile::new(false), height, width),
            tiles: Array2::from_shape_simple_fn((width, height), || Tile::new(false)),
        }
    }

    // pub fn generate_mines_simple(self, mines: usize) -> Self {

    //     self
    // }

    /// Distribute mines in the minefield based on a percentage chance that each
    /// tile is a mine.
    ///
    /// Because it's all based on random chance, it's entirely possible for
    /// every single tile to be a mine, or for no tiles to be a mine. Use with
    /// caution, and prefer other distribution methods instead.
    /// 
    /// TODO: add some example to test the logic of `... < chance`
    pub fn generate_mines_by_percentage(mut self, chance: f64) -> Self {
        let mut mines: usize = 0;

        for tile in self.tiles.iter_mut() {
            if rand::random::<f64>() < chance {
                tile.set_mine();
                mines += 1;
            } else {
                tile.unset_mine();
            }
        }
        // for col in self.tiles.columns_iter() {
        //     for tile in col {
        //         if rand::random::<f64>() < chance {
        //             tile.set_mine();
        //             mines += 1;
        //         } else {
        //             tile.unset_mine();
        //         }
        //     }
        // }

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

#[derive(Clone, Copy, Debug, Default)]
pub enum TileState {
    Flagged,
    Visible,
    #[default]
    Hidden,
}

// deliberately not Copy so it stays owned
#[derive(Clone, Debug)]
pub struct Tile {
    state: TileState,
    is_mine: bool,
}
impl Tile {
    pub fn new(is_mine: bool) -> Self {
        Self {
            state: TileState::default(),
            is_mine,
        }
    }

    pub fn state(&self) -> TileState {
        self.state
    }

    pub fn is_mine(&self) -> bool {
        self.is_mine
    }

    pub fn set_mine(&mut self) {
        self.is_mine = true;
    }

    pub fn unset_mine(&mut self) {
        self.is_mine = false;
    }
}
