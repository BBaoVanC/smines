use clap::Parser;


#[derive(Parser, Debug)]
#[clap(author, version, about, long_about = "Simple minesweeper in the terminal.")]
struct Args {
    #[clap(short, long, value_parser, default_value_t = 16)]
    cols: u32,
    #[clap(short, long, value_parser, default_value_t = 16)]
    rows: u32,
    #[clap(short, long, value_parser, default_value_t = 40)]
    mine_count: u32,

    #[clap(short, long, value_parser, default_value_t = false)]
    allow_undo: bool,
}


fn main() {
    let args = Args::parse();
    println!("{:?}", args);
}
