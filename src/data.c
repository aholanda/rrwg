#include <stdio.h>
#include <string.h>

#include "rrwg.h"

extern char dat_file_name[MAXFN];

void data_fwrite(struct graph *g) {
        FILE *file;
	struct walker *w;
	struct vertex *v;
	int i, j, t;
	char *sep = "";

	strncpy(dat_file_name, g->name, MAXFN);
	append_suffix(dat_file_name, ".dat");
	file = Fopen(dat_file_name, "w");

        /* Zereod the visits by walkers in the vertices. */
	for (i=0; i<g->w; i++)
		for(j=0; j<g->n; j++) {
			v = &g->vertices[j];
                        v->visits[i] = 0;
                }

        /* Perform the counting of visits. */
	for (t=1; t<=g->maxsteps; t++) {
		for (i=0; i<g->w; i++) {
			w = &g->walkers[i];
			for(j=0; j<g->n; j++) {
				v = &g->vertices[j];
				if (w->path[t]==v)
					fprintf(file, "%d", ++v->visits[i]);
				else
					fprintf(file, "%d", v->visits[i]);

				sep = ((i+1)*(j+1)) != (g->w*g->n) ? DATA_SEP : "\n";
				fprintf(file, "%s", sep);
			}
		}
	}
	Fclose(file);
	fprintf(stderr, "* Wrote %s\n", dat_file_name);
}

