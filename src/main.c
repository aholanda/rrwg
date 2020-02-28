#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rrwg.h"

float version=0.2;

/* The walks may be traced in the log file if the user changes the
   program behavior with the flag +l */
static char log_file_name[MAXFN];
FILE *log_file;

/* Flags passed to the program. */
char flags[128];

static void init() {
	int i;

	for (i=0; i<128; i++)
		flags[i] = FALSE;
	flags['b'] = TRUE;
}

static void show_usage_and_exit(char **av) {
	fprintf(stderr, "usage: %s [-b+v] infile.net\n", av[0]);
	fprintf(stderr, "where:\n");
	fprintf(stderr, "\t-b:\tsupress the banner;\n");
	fprintf(stderr, "\t+l:\twrite the walk trace into \"infile.log\".\n");
	exit(EXIT_FAILURE);
}

static void show_banner() {
	if (flags['b'])
		fprintf(stderr, "* This is RRWG version %2.2f\n",
			version);
}

static void eval_arg(char *a, char on_off) {
	Boolean state=FALSE;

	if (on_off == '+')
		state = TRUE;

	while (*(++a) != '\0')
		flags[(int)*a] = state;
}

static Boolean scan_args(int ac, char**av) {
	Boolean ok=FALSE;
	int i;
	char *a;

	for (i=ac-1; i>0; i--) {
		a = av[i];
		if (*a == '+' || *a == '-')
			eval_arg(a, *a);
		else {
			strncpy(net_file_name, a, MAXFN);
			ok = TRUE;
		}
	}
	return ok;
}

static void open_log_file(char *prefix) {
	char buff[100];
	time_t now = time (0);

	/* Append the extension to graph name */
	snprintf(log_file_name, MAXFN, "%s%s", prefix, ".log");
	log_file = Fopen(log_file_name, "w");

	/* Write the current datetime as head */
	strftime(buff, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
	fprintf(log_file, "# Created at %s\n", buff);
}

int main(int argc, char **argv) {
	struct graph *g;
	Boolean go_on = FALSE;

	init();
	go_on = scan_args(argc, argv);
	if (!go_on)
		show_usage_and_exit(argv);
	show_banner();

	g = graph_new();
	g = pjk_read(g, net_file_name);

	if (flags['l'])
		open_log_file(g->name);
	graph_print(g, log_file);

	/* RANDOM WALKS */
	walk(g);

	/* RESULTS */
	data_fwrite(g);
	/* SCRIPT TO GENERATE THE PLOT */
	R_fwrite_script(g);

	if (flags['l']) {
		Fclose(log_file);
		fprintf(stderr, "* Wrote %s\n", log_file_name);
	}
	graph_free(g);

	return EXIT_SUCCESS;
}
