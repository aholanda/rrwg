use std::fs::File;
use std::io::prelude::*;
use huji::{Graph, VertexIndex};

use huji::io::pajek;

pub type VisitorIndex = usize;

#[derive(Debug)]
pub struct Walker {
    id: VisitorIndex,
    path: Vec<VertexIndex>,
}

impl Walker {
    pub fn new(index: VisitorIndex) -> Self {
        Self{
            id: index,
            path: vec![],
        }
    }

    pub fn go_to(&mut self, v: VertexIndex) {
        self.path.push(v);
    }
}

pub struct RRWG<'r> {
    graph: Graph<'r>,
    walkers: Vec<Walker>,
    alpha: f64,
    max_steps: usize,
    function: fn(f64, f64, f64, f64) -> f64,
}

impl RRWG<'_> {
    pub fn new(filename: &str) -> Self {
        let mut rrwg: RRWG;

        let mut file = File::open(filename)
            .expect("Unable to open file");
        let mut contents = String::new();

        file.read_to_string(&mut contents)
            .expect("Unable to read file");

        pajek::read(filename);

        Self {
            graph: Graph::new(),
            walkers: vec![],
            alpha: 0.1,
            max_steps: 10,
            function: Self::exp,
        }
    }

    fn exp(x: f64, y: f64, alpha: f64, b: f64) -> f64 {
        0.2
    }

}
