#include <stdio.h>
#include <stdlib.h>

#include "rrwg.h"

int main(int argc, char **argv) {
	struct graph *g;

	g = graph_init();

	g = pjk_read(g, "example0.net");

	graph_print(g);

	walk(g);

	plot(g);


	graph_destroy(g);

	return EXIT_SUCCESS;
}
