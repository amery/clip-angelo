/*
Start total new system v. 0.0
with hard coded long name variables to have clear system
Angelo GIRARDI
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <ci_coll.h>
#include <ci_po_util.h>

static char *filename = "-";

static int dupflag = 0;

static int verbose = 0;

static char *prgname = "";

#include <po_extr/static_debug.c>

#include <po_extr/typedef_struct_EntryData.h>

static int entry_handler(void *par, char *filename, int mline, char *txt);

#include <po_extr/main.c>
#include <po_extr/static_entry_handler.c>
