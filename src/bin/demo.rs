use smines::{
    game::Game,
    generate::TemplateMinefield,
    minefield::{FieldDimension, Minefield},
};

// use std::io;
// use crossterm::{cursor, execute, terminal};
// struct Term;
// impl Term {
//     fn new() -> Self {
//         execute!(
//             io::stdout(),
//             terminal::EnterAlternateScreen,
//             terminal::Clear(terminal::ClearType::All),
//             cursor::Hide,
//         )
//         .unwrap();
//         terminal::enable_raw_mode().unwrap();
//         Self {}
//     }
// }
// impl Drop for Term {
//     fn drop(&mut self) {
//         execute!(
//             io::stdout(),
//             cursor::MoveTo(0, 0),
//             terminal::Clear(terminal::ClearType::All),
//             terminal::LeaveAlternateScreen,
//             cursor::Show,
//         )
//         .unwrap();
//         terminal::disable_raw_mode().unwrap();
//     }
// }

fn main() {
    // let _term = Term::new();

    let game = Game::from_minefield(Minefield::from_template(
        TemplateMinefield::new_randomly_spread(FieldDimension { x: 16, y: 16 }, 40),
    ));

    println!("{:#?}", game.minefield)

    // println!("{:#?}", game.display());
}
