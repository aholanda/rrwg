% RRWG(1)
% Adriano J. Holanda developed the program. Rafael Rosales proposed the idea and helped in the simulation design
% 2020-02-13 v0.1 rrwg man page

# NAME

rrwg - simulate repelling random walks on graphs

# SYNOPSIS

rrwg [-b] [+v] filename.net

# DESCRIPTION

*rrwg* simulates the the repelling random walks on graphs using
"filename.net" as input in a simplified Pajek-like format. After
 the walks are completed, "filename.dat" is generated with the
 walkers' path in terms of vertices. A R file called "filename.R"
 is also created with commands to plot the data.

# COMMAND-LINE OPTIONS
-b
: suppress the banner line printed when rrwg starts the execution.

+l
: create a file named 'filename.log' with each walkers' next step decision.

# EXAMPLES OF INPUT FILE CONTENT

````
# BEGIN OF FILE "complete.net"
# Comments comes after #
# Two walkers' repelling random walks on the 3-complete graph.
alpha=0.45
function=exp
max_steps=1000
walkers=2

* Vertices
a @1=1 2=10
b @2=10
c

* Edges
a -- b
b -- c
a -- c
# END OF FILE

# BEGIN OF FILE "incomplete.net"
alpha=0.75;
function=geom;
max_steps=5;
walkers=2;

* Vertices
a @1=33
b 1=18 @2=50
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
vertex "a" was visited 33 times by walker 1 and b 18 times
by walker 1 and 50 by 2.  When the assignment is omitted like in the
vertices {c, d}, one visit is assigned by default.

The following fields are recognized:

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
a @1=10 @2=11
b 1=2
````

Example on how to assign 10 visits already walked to walker "1" that
starts at vertex "a".  In the same vertex "a", the walker "2" has
visited 11 times before the begin of simulation. The walkers are
separated by space. The at "@" indicates the starting point of
walker identified by walker_id.  When the number of visits is
omitted, the value 1 (one) is assigned by default. The number
of steps given by each walker at time zero must be equal.
