#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rrwg.h"

float version=0.1;

/* Output file name in CSV-like format. */
char dat_file_name[MAXFN];
/* Input file name in Pajek-like format. */
char net_file_name[MAXFN];
/* Flags passed to the program. */
char flags[128];

static void init() {
        int i;

        for (i=0; i<128; i++)
                flags[i] = FALSE;
        flags['b'] = TRUE;
}

static void show_usage_and_exit(char **av) {
        fprintf(stderr, "usage: %s [+v] infile.net\n", av[0]);

        exit(EXIT_FAILURE);
}

static void show_banner(char **av) {
        if (flags['b'])
                fprintf(stderr, "* This is %s version %2.2f\n",
                        av[0], version);
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

int main(int argc, char **argv) {
	struct graph *g;
        Boolean go_on = FALSE;

        init();
        go_on = scan_args(argc, argv);
        if (!go_on)
                show_usage_and_exit(argv);

        show_banner(argv);
        printf("%s\n", net_file_name);

	g = graph_new();

	g = pjk_read(g, net_file_name);

	graph_print(g);

	walk(g);

        data_fwrite(g);
	R_fwrite_script(g);

	graph_free(g);

	return EXIT_SUCCESS;
}
