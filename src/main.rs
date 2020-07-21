use std::env;

use huji::{adj::Graph,
           io::{Parser,
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

struct Exp;

impl WalkFunction for Exp {
    fn walk(parms: Parameters) {

    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let rrwg: Walks;

    match args.len() {
        2 => {
            let fname = &args[1];
            let mut pjk = Pajek::new(fname);
            let graph: &mut Graph = pjk.read();
            println!("{:?}", graph);
            rrwg = Walks::new(Map::Graph(&graph), 2);
        },
        _ => print_help_msg(),
    }
}
