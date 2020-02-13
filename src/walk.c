#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "rrwg.h"

/*
  Sum all visits (steps) occurred in the neighbors of v for all walkers.
*/
static int sum_all_visits_of_v_mates(struct graph*g,
                                     struct vertex*v) {
	struct vertex *x;
	struct arc *a;
	int i, sum;

	for (sum=0, a=v->arcs; a; a=a->next) {
		x = a->tip;
		for (i=0; i<g->w; i++)
			sum += x->visits[i];
	}
	return sum;
}

/*
  Assess the repelling coefficient for all walkers other than pidx
  (walker index) in the vertex v.
*/
static double calc_repellency(struct graph*g, struct vertex *v,
			      int idx, int allvisits) {
	int i; /* counter */
	double sum=0.0; /* repelling coefficient */

	assert(g->w>idx);

	for (i=0; i<g->w; i++)
		if (i!=idx)
			sum += (double)v->visits[i]/allvisits;
	/*
	  All visits are in te same vertex:
	  - The numerator is the normalized visits of all walkers
	  but pidx.
	  - The denominator is the normalized visits of all walkers.
	*/
	return g->func(sum, g->w, g->alpha,
                        (double)v->visits[idx]/allvisits);
}

static double calc_total_repellency(struct graph*g, struct vertex *v,
                                   int walker_idx, int allvisits) {
        struct vertex *x; /* v mate */
        struct arc *a;
        double sum=0.0; /* denominator and numerator */
        int i;

	for (i=0, a=v->arcs; a; a=a->next, i++) {
		x = a->tip;
		x->repel = calc_repellency(g, x, walker_idx, allvisits);
                sum += x->repel;
	}
        return sum;
}

/*
  Choose the next location to be visited by walker p with index
  pidx.
*/
static struct vertex *choose_next_destination(struct graph*g,
					      int idx, int time) {
	struct vertex *v=NULL, *x=NULL;
	struct arc *a;
        struct walker *w=NULL;
	double cum=0.0, r=2.0; /* cummulative sum and random number */
        int i; /* general-purpose counter */
	int allvisits=0; /* all visits in the neighborhood of v */
   	double one; /* total coefficient repelling used to normalize rv */

        w = &g->walkers[idx];
	/* Get the current location */
	v = w->path[time];
	assert(v);

	allvisits = sum_all_visits_of_v_mates(g, v);
        one = calc_total_repellency(g, v, idx, allvisits);

        /* generate a fractional random number between 0.0 and 1.0 */
	r = ( (double)rand() / (double)RAND_MAX );
        /* Normalize random number */
        r = r * one;
	for (i=0, a=v->arcs; a; a=a->next, i++) {
                x = a->tip;
                cum += x->repel; //x->repel;
                if (r < cum)
                        break;
	}
	return x;
}

void walk(struct graph *g) {
	struct walker *w;
	int i, t; /* counters */
	static time_t tm;
	/* Array indexed by walker id to mark next vertex to be
	 * visited. */
	struct vertex **go_to;

	assert(g);

	/* Intializes random number generator */
	srand((unsigned) time(&tm));

	go_to = (struct vertex**)CALLOC(g->w, sizeof(struct vertex*));
	for (t=0; t<g->maxsteps; t++) {
		for (i=0; i<g->w; i++) {
			go_to[i] = choose_next_destination(g, i, t);
                        printf("%d chooses %s\n", i, go_to[i]->name);
		}
		/* Set the walkers next step */
		for (i=0; i<g->w; i++) {
			w = &g->walkers[i];
			graph_visit(g, go_to[i], w, t+1);
		}
	}
	FREE(go_to);
}
