#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "rrwg.h"

#define DATA_SEP ";"

#define TITLE "Random Repelling Walks on Graphs"
#define XLAB "time"
#define YLAB "visits"

#define NSTYLES 3
static char*colors[NSTYLES]= {"red", "black", "dark blue"};

static char dat_file_name[MAXFN];
/* Output file name where the commands to plot are written. */
static char R_file_name[MAXFN];
/* File to write the commands to plot the curves. */
static FILE *R_file;

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

static void R_output_format(char*prefix) {
	char *fmt="pdf";

	fprintf(R_file, "%s('%s.%s')\n", fmt, prefix, fmt);
}

static void R_load_data(struct graph *g) {
	fprintf(R_file, "data <- read.table(\"%s%s\", header=FALSE, sep=\"%s\")\n",
		g->name, ".dat", DATA_SEP);
	fprintf(R_file, "xmax <- nrow(data) # xmax is the number of rows\n");
	fprintf(R_file, "par(mfrow=c(%d, 1)) # set the number of rows\n",
		g->w);
}

static void R_plot(struct graph *g, int walker_idx) {
	fprintf(R_file, "x <- seq(1,xmax)\n");
	fprintf(R_file, "ymin <- min(data[,%d:%d]/x)\n", (walker_idx*g->n)+1, (walker_idx+1)*g->n);
	fprintf(R_file, "ymax <- max(data[,%d:%d]/x)\n", (walker_idx*g->n)+1, (walker_idx+1)*g->n);
	fprintf(R_file, "plot(data[,%d]/x, col=\"%s\", type=\"l\", lwd=2, xlab=\"%s\", ylab=\"%s\", \
		       xlim=c(0,%d), ylim=c(ymin,ymax))\n",
		(walker_idx * g->n) + 1, colors[0], "n", "%visits", g->maxsteps);
}

static void R_line(struct graph *g, int walker_idx, int vertex_idx) {
	fprintf(R_file, "lines(data[,%d]/x, col=\"%s\", lwd=2)\n",
		(walker_idx * g->n) + (vertex_idx + 1), colors[vertex_idx % NSTYLES]);

}

static void R_legend(struct graph *g) {
	int i;

	fprintf(R_file, "legend(xmax-xmax/10, ymax-ymax/10, legend=c(");
	for (i=0; i<g->n; i++) {
		fprintf(R_file, "\"v%d\"", i+1);
		if (i != g->n-1)
			fprintf(R_file, ",");
	}

	fprintf(R_file, "), col=c(");
	for (i=0; i<g->n; i++) {
		fprintf(R_file, "\"%s\"", colors[i%NSTYLES]);
		if (i != g->n-1)
			fprintf(R_file, ",");
	}
	fprintf(R_file, "), ncol=1, cex=0.8, lwd=2, bty=\"n\")\n");
}

/*
  Fill the buffer with commands to finish the plot.
*/
static void R_end() {
	fprintf(R_file, "dev.off() # Close the file\n");
}

void R_fwrite_script(struct graph *g) {
	int i, j;

	strncpy(R_file_name, g->name, MAXFN);
	append_suffix(R_file_name, ".R");
	R_file = Fopen(R_file_name, "w");

	R_output_format(g->name);
	R_load_data(g);
	for (i = 0; i < g->w; i++) {
		R_plot(g, i);
		for (j = 1; j < g->n; j++)
			R_line(g, i, j);
		R_legend(g);
	}
	R_end();

	Fclose(R_file);
	fprintf(stderr, "* Wrote %s\n", R_file_name);
}
