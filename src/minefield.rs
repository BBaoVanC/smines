//! The actual objects used to represent a minesweeper minefield.

use ndarray::Array2;

use crate::generate::TemplateMinefield;

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
    /// The player has not flagged the tile, or does not think the tile is a mine.
    /// 
    /// There's no reason to have a different flag type for tiles that the player is sure aren't mines
    /// because they should instead just reveal the mine.
    #[default]
    Empty,
}

/// Whether a tile is a mine, otherwise how many surrounding tiles are mines.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum TileMineState {
    /// The tile is a mine. The amount of surrounding mines does not need to be known.
    Mine,
    /// The tile is not a mine. The included [`u8`] is the amount of surrounding tiles that are mines.
    Empty(u8),
}

/// A tile in the minefield of a real game.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Tile {
    pub flag_state: TileFlagState,
    pub mine_state: TileMineState,
}
impl Tile {
    /// Create a new tile. The tile is not flagged by default.
    pub fn new_unflagged(mine_state: TileMineState) -> Self {
        Self {
            flag_state: TileFlagState::Empty,
            mine_state,
        }
    }
}

/// A minefield that's ready to be used in an actual game.
#[derive(Debug)]
pub struct Minefield {
    pub size: FieldDimension,
    pub total_mines: usize,
    pub tiles: Array2<Tile>,
}
impl Minefield {
    /// Create a new minefield based on a [`TemplateMinefield`] that has already been created.
    pub fn from_template(template: TemplateMinefield) -> Self {
        Self {
            size: template.size,
            total_mines: template.total_mines,
            tiles: template.layout.map(|t| Tile::new_unflagged(*t)),
        }
    }
}
