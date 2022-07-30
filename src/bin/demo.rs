use smines::minesweeper::{
    game::Game,
    minefield::{FieldDimension, Minefield},
};

fn main() -> anyhow::Result<()> {
    // let _term = Term::new();

    let game = Game::from_minefield(Minefield::new(FieldDimension { x: 16, y: 16 }, 40)?);

    println!("{:#?}", game.minefield);

    // println!("{:#?}", game.display());

    Ok(())
}
