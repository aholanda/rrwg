#ifndef RRWG_OUTPUT_H
#define RRWG_OUTPUT_H

extern void data_fwrite(struct graph*);

/* Write commands to plot the curves into a file named with graph name
 * plus the extension of the program to be used to plot. */
extern void R_fwrite_script(struct graph*);

#endif /* !RRWG_OUTPUT_H */
