/*
Start total new system v. 0.0
with hard coded long name variables to have clear system
Angelo GIRARDI
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <ci_clipcfg.h>

char *CLIPROOT = CLIP_ROOT;

int load_unicode_table(char *name, int *unitbl);

int print_unicode_table(int *);

#include <clip_trans/main.c>
#include <clip_trans/static_freadstr.c>

#define PATH_MAX 4096

#include <clip_trans/load_unicode_table.c>
#include <clip_trans/print_unicode_table.c>
