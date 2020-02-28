#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "rrwg.h"

struct graph *graph_new() {
	struct graph *g;
	g = CALLOC(1, sizeof(struct graph));
	return g;
}

void graph_set_name(struct graph *g, char *name) {
	assert(g);
	assert(name);
	strncpy(g->name, name, MAXFN);
}

static double expa(double x, double y, double alpha, double b) {
	return exp(-alpha*x);
}

static double geom(double x, double y, double alpha, double b) {
	return b*pow(y-x, alpha);
}
void graph_assign_function(struct graph *g, char*name) {
       assert(g);
       assert(name);

       if (strncmp(name, "exp", 4)==0)
	       g->func = &expa;
       else if (strncmp(name, "geom", 5)==0)
	       g->func = &geom;
       else {
	       fprintf(stderr,
		"fatal: unrecognized function \"%s\" in %s\n",
		name, net_file_name);
	       exit(EXIT_FAILURE);
       }
       strncpy(g->funcname, name, MAXTOKEN);
}
void graph_init_walkers(struct graph*g, int nwalkers) {
	struct walker *w;
	int i;

	assert(nwalkers>1);
	assert(g->maxsteps>0);

	g->w = nwalkers;
	g->walkers = CALLOC(g->w, sizeof(struct walker));
	for (i=0; i<g->w; i++) {
		w = &g->walkers[i];
		snprintf(w->name, MAXTOKEN, "walker%d", i+1);
		w->path = CALLOC(g->maxsteps+1, sizeof(struct vertex*));
	}
}

static struct vertex *vertex_lookup(struct graph*g, char *name) {
	struct vertex *v;
	int i;

	for (i=0; i<g->n; i++) {
		v = &g->vertices[i];
		if (strncmp(name, v->name, MAXTOKEN)==0) return v;
	}
	return NULL;
}

struct vertex *graph_vertex_add(struct graph*g, char *name) {
	struct vertex *v;
	assert(g && name);

	if (g->n>=MAXN) {
		fprintf(stderr, "%s:%d Graph capacity for vertices exceeded: %d\n",
			__FILE__, (int)__LINE__, MAXN);
		exit(EXIT_FAILURE);
	}

	v = vertex_lookup(g, name);
	if (!v) {
		assert(g->w > 1);
		v = &g->vertices[g->n++];
		strncpy(v->name, name, MAXTOKEN);
		v->arcs = NULL;
		v->repel = 0.0;
		// Allocate memory to store the visits by the walkers
		// in the vertices
		v->visits = CALLOC(g->w, sizeof(struct walker*));
	}
	return v;
}

static struct arc *arc_add(struct graph*g, char *from, char *to) {
	char *names[2] = {from, to};
	struct vertex *vs[2];
	struct arc *a;
	int i;

	for (i=0; i<2; i++) {
		vs[i] = vertex_lookup(g, names[i]);
		if (vs[i]==NULL)
			vs[i] = graph_vertex_add(g, names[i]);
	}
	for (a=vs[0]->arcs; a; a=a->next)
		if (a->tip==vs[1])
			return a;

	if (g->m>=MAXM) {
		fprintf(stderr, "%s:%d Graph capacity for vertices exceeded: %d\n",
		      __FILE__, (int)__LINE__, MAXM);
		exit(EXIT_FAILURE);
	}
	a = &g->area[g->m++];
	a->tip = vs[1];
	a->next = vs[0]->arcs;
	vs[0]->arcs = a;

	return a;
}

struct arc *graph_arc_add(struct graph*g, char *from, char *to) {
	assert(g && from && to);
	return arc_add(g, from, to);
}

struct arc *graph_edge_add(struct graph*g, char *from, char *to) {
	assert(g && from && to);
	(void)arc_add(g, to, from);
	return arc_add(g, from, to);
}

void graph_self_loops_add(struct graph *g) {
	struct vertex *v;
	int i;

	assert(g);

	for (i=0; i<g->n; i++){
		v = &g->vertices[i];
		graph_edge_add(g, v->name, v->name);
	}
}

/*
  Increment the number of visits of walker w in
  the vertex v by nvisits.
*/
void graph_visits_incr(struct graph*g,
			struct vertex*v,
			struct walker*w,
			int time,
			int nvisits) {
	int idx = w - &g->walkers[0];

	assert(g);
	assert(v);
	assert(w);
	assert(idx>=0);
	assert(nvisits>0);

	v->visits[idx] += nvisits;
}

void graph_visit(struct graph*g, struct vertex*v,
		 struct walker*w, int time) {
	assert(g);
	assert(v);

	graph_visits_incr(g, v, w, time, 1);
	w->path[time] = v;
}

inline void graph_free(struct graph *g) {
	struct vertex *v;
	struct walker *w;
	int i;

	if (g) {
		for (i=0; i<g->n; i++) {
			v = &g->vertices[i];
			FREE(v->visits);
		}
		for (i=0; i<g->w; i++) {
			w = &g->walkers[i];
			FREE(w->path);
		}
		FREE(g->walkers);
		FREE(g);
	}
}

void graph_print(struct graph*g, FILE *f) {
	struct vertex *v, *x;
	struct arc *a;
	struct walker *w;
	int i, j;
	char loc;

	if (f==NULL)
		f = stdout;

	fprintf(f, "* Parameters\n");
	fprintf(f, "alpha=%2.3f\n", g->alpha);
	fprintf(f, "function=%s\n", g->funcname);
	fprintf(f, "maxsteps=%d\n", g->maxsteps);
	fprintf(f, "walkers=%d\n", g->w);
	fprintf(f, "* Graph(V, E)=%s(%d, %d)\n", g->name, g->n, g->m);
	for (i=0; i<g->n; i++) {
		v = &g->vertices[i];
		fprintf(f, "%s:", v->name);
		for (a=v->arcs; a; a=a->next){
			x = a->tip;
			fprintf(f, " %s", x->name);
		}
		fprintf(f, ";");
		for (j=0; j<g->w; j++) {
			loc = ' ';
			w = &g->walkers[j];
			if (w->path[0]==v)
				loc = '@';
			fprintf(f, " %cvisits(walker%d, t0)=%d", loc, j+1, v->visits[j]);
		}

		fprintf(f, "\n");
	}
}
