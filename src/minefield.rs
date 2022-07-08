//! The actual objects used to represent a minesweeper minefield.

use ndarray::Array2;

use crate::generate::TemplateMinefield;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum TileMineStatus {
    Mine,
    Empty(u8),
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Dimension {
    pub x: usize,
    pub y: usize,
}

#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub enum TileFlagStatus {
    Flagged,
    Suspicious,
    #[default]
    Empty,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Tile {
    pub flag_status: TileFlagStatus,
    pub mine_status: TileMineStatus,
}
// impl From<TileMineStatus> for Tile {
//     fn from(mine_status: TileMineStatus) -> Self {
impl Tile {
    pub fn new(mine_status: TileMineStatus) -> Self {
        Self {
            flag_status: TileFlagStatus::default(),
            mine_status,
        }
    }
}

#[derive(Debug)]
pub struct Minefield {
    size: Dimension,
    total_mines: usize,
    tiles: Array2<Tile>,
}
impl From<TemplateMinefield> for Minefield {
    fn from(template: TemplateMinefield) -> Self {
        Self {
            size: template.size,
            total_mines: template.total_mines,
            tiles: template.layout.map(|t| Tile::new(*t)),
        }
    }
}
