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
#include <cl_iconv.h>

#include <ci_coll.h>
#include <ci_po_util.h>
#include <ci_clipcfg.h>

#include <po_subst/typedef_struct_PoFile.h>

static Coll po_files;

static int read_po_file(char *filename);

#include <po_subst/typedef_struct_ParseData.h>

static int entry_handler(void *par, char *filename, int mline, char *txt);

static int norm_handler(void *par, char *txt, int len);

static int verbose = 0;

static char *prgname = "";

static char *filename = "stdin";

static char *encoding = 0;

#include <po_subst/static_debug.c>
#include <po_subst/main.c>
#include <po_subst/static_read_po_file.c>
#include <po_subst/static_find_msg.c>
#include <po_subst/static_entry_handler.c>
#include <po_subst/static_norm_handler.c>
