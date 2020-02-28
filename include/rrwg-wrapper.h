#ifndef RRWG_WRAPPER_H
#define RRWG_WRAPPER_H

#define BUFLEN 512

/* Memory allocation */
extern void *Malloc(const size_t size, const char *cfile, const int line);
extern void *Calloc(const size_t nelems, const size_t size, const char *cfile, const int line);
extern void *Realloc(void *ptr, const size_t size, const char *cfile, const int line);
extern void Free(void *ptr, const char *cfile, const int line);

#define ALLOC(sz) Malloc((sz), __FILE__, (int)__LINE__)
#define CALLOC(n, sz) Calloc((n), (sz), __FILE__, (int)__LINE__)
#define REALLOC(ptr, sz) Malloc((ptr), (sz), __FILE__, (int)__LINE__)
#define FREE(ptr) Free((ptr), __FILE__, (int)__LINE__)

/* IO */
extern FILE *Fopen(char *filename, const char *mode);
extern void Fclose(FILE *fp);

#endif /* !RRWG_WRAPPER_H */
