#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "rrwg.h"

struct graph graphs[16];
struct graph *g;

struct graph *graph_init() {
	g = &graphs[0];
	return g;
}

void graph_set_name(char *name) {
	assert(name);
	strncpy(g->name, name, MAXFN);
}

static double __exp__(double x, double y, double alpha) {
	return exp(-alpha*x);
}

static double __geom__(double x, double y, double alpha) {
#warning implement GEOM function
	return 1.0;
}
void graph_assign_function(struct graph *g, char*name, char*fn) {
       assert(g);
       assert(name);

       if (strncmp(name, "exp", 4)==0)
               g->func = &__exp__;
       else if (strncmp(name, "geom", 5)==0)
               g->func = &__geom__;
       else {
               fprintf(stderr,
                "fatal: unrecognized function \"%s\" in %s\n",
                name, fn);
               exit(EXIT_FAILURE);
       }
}
void graph_init_walkers(struct graph*g, int nwalkers) {
        assert(nwalkers>1);

        g->w = nwalkers;
        g->walkers = CALLOC(g->w, sizeof(struct walker));
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
		v = &g->vertices[g->n++];
		strncpy(v->name, name, MAXTOKEN);
		v->arcs = NULL;
                v->repel = 0.0;
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


void graph_visits_set(struct graph*g,
		        struct vertex*v,
		        struct walker*w,
                        int time,
		        int nvisits) {
        int idx = w - &g->walkers[0];

	assert(g);
	assert(v);
        assert(w);
	assert(idx>=0);
        if (v->visits==NULL)
                v->visits = CALLOC(g->w, sizeof(struct walker*));

        if (w->path==NULL)
                w->path = CALLOC(g->maxsteps+1, sizeof(struct vertex*));

	v->visits[idx] = nvisits;
}

void graph_visit(struct graph*g, struct vertex*v,
                 struct walker*w, int time) {
	assert(g);
	assert(v);

        graph_visits_set(g, v, w, time, 1);
        w->path[time] = v;
}

INLINE void graph_destroy(struct graph*g) {
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
	}
}

void graph_print(struct graph*g) {
	struct vertex *v, *w;
	struct arc *a;
	int i, j;

	printf("* Graph(V, E) %s(%d, %d)\n", g->name, g->n, g->m);
	for (i=0; i<g->n; i++) {
		v = &g->vertices[i];
		printf("%s:", v->name);
		for (a=v->arcs; a; a=a->next){
			w = a->tip;
			printf(" %s", w->name);
		}
		printf("; P.");
		for (j=0; j<g->w; j++)
			printf(" %d=%d", j+1, v->visits[j]);

		printf("\n");
	}
}
