#ifndef RRWG_GRAPH_H
#define RRWG_GRAPH_H

/* GRAPH definitions */

/* Maximum number of vertices */
#define MAXN 128
/* Maximum number of edges */
#define MAXM 512
/* default number of steps at time 0 (zero) */
#define DEFAULT_STEPS0 1
/* Value to initialize and test maximum steps */
#define NOSTEP 0
/* Value to initialize and test alpha */
#define EMPTY_COEFF 0.0

struct vertex {
	char name[MAXTOKEN];
	struct arc *arcs;
	/* array of visits by walker, it is dinamically allocated. */
	int *visits;
	/* Repellency for some walker */
	double repel;
};

struct arc {
	struct vertex *tip;
	struct arc *next;
};

struct walker {
	char name[MAXTOKEN];
	struct vertex **path; /* all vertices visited */
};

struct graph {
	/* Graph receives the name of the input file. */
	char name[MAXFN]; /* name of the graph */
	struct vertex vertices[MAXN];
	int n, m; /* number of vertices, arcs */
	struct arc area[MAXM];

	/* Walk data */
	char funcname[MAXTOKEN]; /* repelling function label */
	struct walker *walkers; /* who will walk through vertices */
	int w; /* number of walkers */
	/* function plugged to calculate repelling index */
	double (*func)(double x, double y, double alpha, double b);
	/* repellency */
	double alpha;
	/* maximum number of steps */
	int maxsteps;
};

/* enum context marks in which part of the graph description
   the parser is working on.
   Used in parser.c
*/
enum context {GRAPH, VERTEX, ARC, EDGE=ARC};

extern struct graph *graph_new();
extern void graph_assign_function(struct graph*,
				  char*function_name);
extern void graph_init_walkers(struct graph*, int nwalkers);
extern void graph_set_name(struct graph*, char*name);
extern struct vertex *graph_vertex_add(struct graph*, char *name);
extern struct arc *graph_edge_add(struct graph*, char *from, char *to);
extern void graph_print(struct graph*, FILE*);
extern void graph_self_loops_add(struct graph*);
extern void graph_free(struct graph*);
extern void graph_visits_incr(struct graph*,
			      struct vertex*,
			      struct walker*,
			      int time,
			      int nvisits);
extern void graph_visit(struct graph*,
			struct vertex*,
			struct walker*,
			int time);

/* Vertex */
extern void graph_vertex_visit(struct graph*, struct vertex*v,
			       struct walker*w);

#endif /* !RRWG_GRAPH_H */
