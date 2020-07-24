use std::env;

use huji::{adj::Graph,
           io::{Parser,
                pajek::Pajek}};
use walks::{Location, LocationIndex, Map, Walker};

fn print_help_msg() {
    println!("usage:
match_args <pajek filename>
    Check whether given string is the answer.
match_args {{increase|decrease}} <integer>
    Increase or decrease given integer by one."
    );
}


fn sum_all_visits_in_the_neighborhood(cur_loc_idx: LocationIndex,
                                      walker: &Walker) -> usize {
    let sum: usize = 0;
    let neighbors = Location::get_neighbors(walker.get_map(),
                                            cur_loc_idx);

    // Traverse the neighbors of loc_idx, summing the number of
    // visits of some chosen walker. The sum must be equal for all
    // walkers.
    for i in neighbors {
        // 0 could be any value of visitor index, pick one almost
        // randomly.
        sum += walker.get_visits(i);
    }
    sum
}

fn walk() -> LocationIndex {
    0
}

const NW: u8 = 2;
const MAX: usize = 8;

fn main() {
    let args: Vec<String> = env::args().collect();
    let walkers: Vec<Walker> = vec![];

    match args.len() {
        2 => {
            let fname = &args[1];
            let mut pjk = Pajek::new(fname);
            let mut graph: Graph = pjk.read();
            println!("{:?}", graph);
            for i in 0..NW {
                walkers.push(Walker::new(&Map::Graph(graph), 1, MAX, walk));
            }
        },
        _ => print_help_msg(),
    }
}
