#include <stdlib.h>
#include <string.h>

#include "rrwg.h"

void *Malloc(const size_t size, const char *cfile, const int line) {
	void *ptr;

	ptr = malloc(size);
	if (!ptr) {
		fprintf(stderr, "%s:%d %s\n", cfile, line, strerror(errno));
                exit(errno);
	}
	return ptr;
}

void *Calloc(const size_t nelems, const size_t size, const char *cfile, const int line) {
	void *ptr;

	ptr = calloc(nelems, size);
	if (!ptr) {
		fprintf(stderr, "%s:%d %s\n", cfile, line, strerror(errno));
                exit(errno);
	}
	return ptr;
}

void *Realloc(void *ptr, const size_t size, const char *cfile, const int line) {
	ptr = realloc(ptr, size);
	if (!ptr) {
		fprintf(stderr, "%s:%d %s\n", cfile, line, strerror(errno));
                exit(errno);
	}
	return ptr;
}

void Free(void *ptr, const char *cfile, const int line) {
	if (ptr) free(ptr);
}
