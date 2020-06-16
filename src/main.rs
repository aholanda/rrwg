mod walker;

use huji::{Graph, VertexIndex};

#[derive(Debug)]
pub struct Walker {
    name: String,
    path: Vec<VertexIndex>,
}

impl Walker {
    pub fn new(name: &String) -> Self {
        Self{
            name: name.to_string(),
            path: vec![],
        }
    }

    pub fn go_to(&mut self, v: VertexIndex) {
        self.path.push(v);
    }
}

fn main() {
    let walker = Walker::new(&1.to_string());
    let graph = Graph::new();

    println!("{:?}", walker);
}

fn walk(graph: &Graph) {
    {}
}
