//! The actual objects used to represent a minesweeper minefield.

use ndarray::Array2;
use rand::{prelude::SliceRandom, thread_rng};
use thiserror::Error;

/// The dimensions of a minefield.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct FieldDimension {
    /// The width of the minefield
    pub x: usize,
    /// The height of the minefield
    pub y: usize,
}

/// Whether a tile is flagged, and the type of flag it has.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub enum TileFlagState {
    /// The player is certain this tile is a mine.
    Flagged,
    /// The player is unsure if this tile is a mine.
    Suspicious,
    /// The player has not flagged the tile, or does not think the tile is a
    /// mine.
    ///
    /// There's no reason to have a different flag type for tiles that the
    /// player is sure aren't mines because they should instead just reveal
    /// the mine.
    #[default]
    Empty,
}

/// Whether a tile is a mine, otherwise how many surrounding tiles are mines.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum TileMineState {
    /// The tile is a mine. The amount of surrounding mines does not need to be
    /// known.
    Mine,
    /// The tile is not a mine. The included [`u8`] is the amount of surrounding
    /// tiles that are mines.
    Empty(u8),
}

/// A tile in the minefield of a real game.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Tile {
    pub flag_state: TileFlagState,
    pub mine_state: TileMineState,
    pub is_visible: bool,
}
impl Tile {
    /// Create a new tile. The tile is not flagged by default.
    pub fn new_unflagged(mine_state: TileMineState) -> Self {
        Self {
            flag_state: TileFlagState::Empty,
            mine_state,
            is_visible: false,
        }
    }
}

#[derive(Debug, Error)]
pub enum MinefieldError {
    #[error("There was not enough space in the minefield to place all of the mines.")]
    NotEnoughSpace,
}

/// A minefield that's ready to be used in an actual game.
#[derive(Debug)]
pub struct Minefield {
    pub size: FieldDimension,
    pub total_mines: usize,
    pub tiles: Array2<Tile>,
}
impl Minefield {
    /// Create a minefield with certain dimensions and mine count.
    pub fn new(size: FieldDimension, total_mines: usize) -> Result<Self, MinefieldError> {
        let mut rng = thread_rng();

        let mut all_tiles = Vec::with_capacity(size.x * size.y);
        for x in 0..size.x {
            for y in 0..size.y {
                all_tiles.push((x, y));
            }
        }
        all_tiles.shuffle(&mut rng);

        let mine_positions = all_tiles.iter().take(total_mines);
        let mut template_tiles =
            Array2::from_shape_simple_fn((size.x, size.y), || TileMineState::Empty(0));
        if mine_positions.len() < total_mines {
            Err(MinefieldError::NotEnoughSpace)
        } else {
            for (x, y) in mine_positions.clone() {
                template_tiles[[*x, *y]] = TileMineState::Mine;
            }
            for (mine_col, mine_row) in mine_positions {
                for col in mine_col.saturating_sub(1)..=(mine_col.saturating_add(1)) {
                    if col > size.x - 1 {
                        // subtract 1 since index starts at 0
                        continue;
                    }

                    for row in mine_row.saturating_sub(1)..=(mine_row.saturating_add(1)) {
                        if row > size.y - 1 {
                            // subtract 1 since index starts at 0
                            continue;
                        }

                        if let TileMineState::Empty(s) = template_tiles.get_mut((col, row)).unwrap()
                        {
                            *s += 1;
                        }
                    }
                }
            }
            Ok(Self {
                size,
                total_mines,
                tiles: template_tiles.map(|t| Tile::new_unflagged(*t)),
            })
        }

        // let mut possible_cols = (0..size.x).collect::<Vec<usize>>();
        // let mut possible_rows = (0..size.y).collect::<Vec<usize>>();
        // possible_cols.shuffle(&mut rng);
        // possible_rows.shuffle(&mut rng);

        // let possible_mines =
        // possible_cols.iter().zip(possible_rows).take(mines);
        // if possible_mines.len() < mines {}
    }

    // /// Create a new minefield based on a [`TemplateMinefield`] that has already
    // /// been created.
    // pub fn from_template(template: TemplateMinefield) -> Self {
    //     Self {
    //         size: template.size,
    //         total_mines: template.total_mines,
    //         tiles: template.layout.map(|t| Tile::new_unflagged(*t)),
    //     }
    // }
}
