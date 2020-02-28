/*
 * Wrapper subroutines to standard C subroutines.
 */
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "rrwg.h"

/* Print error message and exit. */
#define PERROR_EXIT(fn, ln, errno) do {					\
		fprintf(stderr, "%s:%d %s\n", (fn), (ln), strerror((errno))); \
		exit((errno));						\
	} while (0)

void *Malloc(const size_t size, const char *cfile, const int line) {
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
		PERROR_EXIT(cfile, line, errno);

	return ptr;
}

void *Calloc(const size_t nelems, const size_t size, const char *cfile, const int line) {
	void *ptr;

	ptr = calloc(nelems, size);
	if (!ptr)
		PERROR_EXIT(cfile, line, errno);

	return ptr;
}

void *Realloc(void *ptr, const size_t size, const char *cfile, const int line) {
	ptr = realloc(ptr, size);
	if (!ptr)
		PERROR_EXIT(cfile, line, errno);

	return ptr;
}

void Free(void *ptr, const char *cfile, const int line) {
	if (ptr) free(ptr);
}

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
