use std::env;

use huji::{data::Graph,
           io::{Parse,
                pajek::Pajek}};
use walks::{Map, Walks};

fn print_help_msg() {
    println!("usage:
match_args <pajek filename>
    Check whether given string is the answer.
match_args {{increase|decrease}} <integer>
    Increase or decrease given integer by one."
    );
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let rrwg: Walks;

    match args.len() {
        2 => {
            let fname = &args[1];
            let graph: Graph = *<Pajek as Parse>::read(fname);
            println!("{:?}", graph);
            rrwg = Walks::new(Map::Graph(&graph));
        },
        _ => print_help_msg(),
    }
}
