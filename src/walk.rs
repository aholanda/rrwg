use huji::{Graph, VertexIndex};

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
