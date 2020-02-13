% RRWG(1)
% Adriano J. Holanda
% 2019-12-17 v0.3 rrwg man page

# NAME

rrwg - simulate repelling random walks on graphs

# SYNOPSIS

rrwg [-b] [+s] [+l] infile[.net] [outfile.dat]

# DESCRIPTION

*rrwg* simulates the the repelling random walks on graphs using
dotfile[.gv] as input in a simplified graphviz dot format. The
outfile[.dat] is optional because the program use the prefix file name
dotfile if the output file is ommited.

# COMMAND-LINE OPTIONS
-b
: suppress the banner line printed when rrwg starts the execution.

+l
: generate a file named "outfile.log" with the execution trace in terms of visits.

+s
: show the statistics of execution after the program is finished.

# EXAMPLES OF INPUT FILE CONTENT

````
# BEGIN OF FILE
# Comments comes after #
# Two walkers' repelling random walks on the 3-complete graph.
name=complete
alpha=0.45
function=exp
max_steps=1000
walkers=2

* Vertices
a *1=1 2=10
b *2=10
c

* Edges
a -- b
b -- c
a -- c
# END OF FILE

# BEGIN OF FILE
alpha=0.75;
function=geom;
max_steps=5;
walkers=2;

* Vertices
a *1=100
b 1=18 *2=50
c
d

* Edges
a -- b
b -- c
a -- c
b -- d
# END OF FILE
````

The input file must contain the graph description and the initial
parameters in a simplified Pajek format with extension ".net". In the
graph named "complete", the parameters were declared before the
beginning of vertices' description. The vertices' description begin
after "* Vertices" string mark. Three vertices are described, "a", "b"
and "c"; and three edges are defined, like "a -- b" that creates a
edge between the vertices "a" and "b". The double-hyphen `--`
indicates the transitivity. All edges starts after the string
"* Edges". The name of the graph is optional.

The undirected graph called "incomplete" has four vertices {a, b, c,
d} and four edges {{a, b}, {b, c}, {a, c}, {b, d}}. At time 0, the
vertex \f[C]a\f[] was visited 100 times by walker 1 and b 18 times
by walker 1 and 50 by 2.  When the assignment is omitted like in the
vertices {c, d}, one visit is assigned by default.

The following fields are recognized:

name /[optional]/
: The name of the graph.

function
: The function used to calculate the repelling index, values may be "exp" that stands for exponential and
"geom" that is geometric abbreviature, the following equations are used to represent each category:

exp
: f(x) =  exp(-x*alpha)

geom
: f(x,y) =  x*(2-y)^alpha

alpha
: repelling coefficient used in the chosen function

max_steps
: number maximum of steps to perform, it is the time limit when the simulations stops


walkers
: number of walkers to simulate

````
a *1=10 2=5
b
````

Example on how to assign 10 visits already walked to walker 1 that
starts at vertex "a".  In the same vertex "a", the walker 2 has
visited 5 times before the begin of simulation. The walkers are
separated by space. The asterisk "*" indicates the starting point of
walker identified by walker_id.  When the number of visits is
omitted, the value one is assigned by default.

# AUTHORS
Adriano J. Holanda developed the program. Rafael Rosales proposed the
idea and helped in the simulation design.
