#ifndef __RRWG_H__
#define __RRWG_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MACROS */
#define MAXTOKEN 32
/* Maximum file name length, discount the 4 bytes extension. */
#define MAXFN 60

/* TYPEDEFS */
typedef short Boolean;
enum Boolean_enum {FALSE, TRUE};

#include "rrwg-wrapper.h"
#include "rrwg-input.h"
#include "rrwg-graph.h"
#include "rrwg-output.h"
#include "rrwg-main.h"

#define NPARMS 4
extern void walk(struct graph*);

#endif
