mod walk;

use std::env;

use huji::Graph;
use crate::walk::{RRWG};

fn print_help_msg() {
    println!("usage:
match_args <string>
    Check whether given string is the answer.
match_args {{increase|decrease}} <integer>
    Increase or decrease given integer by one."
    );
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut rrwg: RRWG;

    match args.len() {
        2 => {
            let fname = &args[1];
            rrwg = RRWG::new(fname);
        },
        _ => print_help_msg(),
    }
}
