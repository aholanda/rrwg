#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "rrwg.h"

FILE *Fopen(char *filename, const char *mode) {
	FILE *fp;

	fp = fopen(filename, mode);
	if (!fp) {
		fprintf(stderr, "%s: %s\n", strerror(errno), filename);
                exit(errno);
	}
	return fp;
}

void Fclose(FILE *fp) {
	if (fp)
		fclose(fp);
}
