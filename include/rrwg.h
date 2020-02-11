#ifndef __RRWG_H__
#define __RRWG_H__
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXTOKEN 32

#ifdef GNUC
#define INLINE __attribute__((always_inline))
#else
#define INLINE inline
#endif

/* begin MEMORY ALLOCATION */
#define ALLOC(sz) Malloc((sz), __FILE__, (int)__LINE__)
#define CALLOC(n, sz) Calloc((n), (sz), __FILE__, (int)__LINE__)
#define REALLOC(ptr, sz) Malloc((ptr), (sz), __FILE__, (int)__LINE__)
#define FREE(ptr) Free((ptr), __FILE__, (int)__LINE__)

extern void *Malloc(const size_t size, const char *cfile, const int line);
extern void *Calloc(const size_t nelems, const size_t size, const char *cfile, const int line);
extern void *Realloc(void *ptr, const size_t size, const char *cfile, const int line);
extern void Free(void *ptr, const char *cfile, const int line)
#ifdef GNUC
__attribute__((always_inline))
#endif
;
/* end MEMORY ALLOCATION */

/* begin GRAPH */
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
	/* array of visits by particle, it is dinamically allocated. */
	int *visits;
        /* Repellency for some particle */
        double repel;
};

struct arc {
	struct vertex *tip;
	struct arc *next;
};

struct walker {
	char name[MAXTOKEN];
	int steps0; /* number of steps at time 0 (zero) */
	struct vertex **path; /* all vertices visited */
};

struct graph {
	char name[MAXTOKEN]; /* name of the graph */
	struct vertex vertices[MAXN];
	int n, m; /* number of vertices, arcs */
	struct arc area[MAXM];

	/* Walk data */
	char function[MAXTOKEN]; /* repelling function label */
	struct walker *walkers; /* who will walk through vertices */
	int w; /* number of walkers */
	/* function plugged to calculate repelling index */
	double (*func)(double x, double y, double alpha);
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

extern struct graph *graph_init();
extern void graph_assign_function(struct graph*,
							char*function_name, char*filename);
extern void graph_init_walkers(struct graph*, int nwalkers);
extern void graph_set_name(char*name);
extern struct vertex *graph_vertex_add(struct graph*, char *name);
extern struct arc *graph_edge_add(struct graph*, char *from, char *to);
extern void graph_print(struct graph*);
extern void graph_self_loops_add(struct graph*);
extern void graph_destroy(struct graph*);
extern void graph_visits_set(struct graph*,
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
/* end GRAPH */

/* begin IO */
#define BUFLEN 256
extern FILE *Fopen(char *filename, const char *mode);
extern void Fclose(FILE *fp)
#ifdef GNUC
	__attribute__((always_inline))
#endif
	;
/* end IO */

/* begin PARSER */
#define MAXFN 256

/* Read file name in Pajek format and return the graph described in
 * it.
 */
extern struct graph *pjk_read(struct graph *g, char*filename);

/*
  Copy the source file name to detination file name with the
  substitution of the extension.
*/
extern void subst_ext(char *src_filename, char *dest_filename, char*ext);

/* Remove the extension of file name. */
extern void rm_ext(char *filename);

/* Append the string suf in the end of str. */
extern void append_suffix(char str[], char *suf);

/* end PARSER */

/* begin PLOT */
/* Write commands to plot the curves into a file named with graph name
 * plus the extension of the program to be used to plot. */
extern void plot(struct graph*);

/* end PLOT */

/* begin WALK */
/* Number of parameters to be set. */
#define NPARMS 4

extern void walk(struct graph*);
/* end WALK */

#endif
