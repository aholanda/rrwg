mod walk;

use std::{env, fs};

use toml::Value;

use huji::Graph;
use crate::walk::{RRWG, Walker};

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
    let mut rrwg: &RRWG;

    match args.len() {
        2 => {
            let fname = &args[1];
            read_file(fname);
        },
        _ => print_help_msg(),
    }

    let walker = Walker::new(0);
    let graph = Graph::new();

    println!("{:?}", walker);
}

fn read_file(filename: &String) {
    let toml_content = fs::read_to_string(filename)
        .expect("Could not open file");

    let rrwg_info = toml_content.parse::<Value>().unwrap();
}
