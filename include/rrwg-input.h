#ifndef RRWG_INPUT_H
#define RRWG_INPUT_H

/* Read file name in Pajek format and return the graph described in
 * it.
 */
extern struct graph *pjk_read(struct graph *g, char*filename);

/*
  Copy the source file name to detination file name with the
  substitution of the extension.
*/
extern void subst_ext(char *src_filename, char *dest_filename, char*ext);

/* Remove the extension of file name. */
extern void rm_ext(char *filename);

/* Append the string suf in the end of str. */
extern void append_suffix(char str[], char *suf);

#endif /* !RRWG_INPUT_H */
