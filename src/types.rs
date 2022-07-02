pub struct MinefieldConfig {
    cols: u32,
    rows: u32,
    mines: u32,
}

pub struct Minefield {
    config: MinefieldConfig,
    tiles: Vec<Vec<Tile>>,
    cursor: Cursor,
}
impl Minefield {
    fn new(config: MinefieldConfig) -> Self {
        Self {
            config,
            tiles: vec![Vec::with_capacity(config.rows); config.cols],
            cursor: Cursor::new(config.cols, config.rows),
        }
    }
}

#[derive(Default)]
pub struct Tile {
    is_mine: bool,
    is_visible: bool,
    is_flagged: bool,
    surrounding_mines: u8,
}

pub struct Coordinate {
    col: u32,
    row: u32,
}
pub type Cursor = Coordinate;
impl Cursor {
    fn new(cols: u32, rows: u32) -> Self {
        Self {
            col: cols / 2,
            row: rows / 2,
        }
    }
}
