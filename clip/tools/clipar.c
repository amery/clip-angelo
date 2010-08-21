/*
Start total new system v. 0.0
with hard coded long name variables to have clear system
Angelo GIRARDI
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <ci_coll.h>
#include <ci_clipcfg.h>

#ifdef OS_MINGW
#include <ci_getopt.h>
#endif

extern char *optarg;

extern int optind;

static int table_flag = 0;

static int c_flag = 0, verbose_level = 0;

static int print_table(char *arname);

static int make_c_file(char *arname, int argc, char **argv);

static int make_pa_file(char *arname, int argc, char **argv);

static void v_printf(int level, const char *fmt, ...);

static char tmp_file[255];

#include <clipar/main.c>
#include <clipar/static_error.c>
#include <clipar/static_v_printf.c>

#define M_OFFS(base,nl,ns) ((base)+8+nl*sizeof(long)+ns*sizeof(short))
#define F_OFFS(fp,nl,ns,nb) ((fp)+nl*sizeof(long)+ns*sizeof(short)+nb)

#include <clipar/static_read_module.c>
#include <clipar/static_print_table.c>
#include <clipar/static_read_file.c>
#include <clipar/static_fwerr.c>
#include <clipar/static_write_short.c>
#include <clipar/static_write_long.c>
#include <clipar/static_write_str.c>
#include <clipar/static_write_buf.c>

#include <clipar/typedef_struct_OffsEntry.h>

#include <clipar/static_cmp_OffsEntry.c>
#include <clipar/static_make_pa_file.c>
#include <clipar/static_make_c_file.c>
