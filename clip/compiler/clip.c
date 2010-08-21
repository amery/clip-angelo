/*
    Copyright (C) 2001  ITK
    Author   : Paul Lasarev <paul@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/

#include <ci_clip.h>
#include <ci_clipcfg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifndef OS_MINGW
#include <sys/times.h>
#else
#include <ci__win32.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <locale.h>

#include <ci_clic.h>
#include <getopt.h>
#include <license.h>

#include <dirent.h>

char *CLIPROOT = CLIP_ROOT;

char *CLIP_LOCALE_ROOT = CLIP_ROOT;

int compat_as_word = 0;

static int syntax_tree_flag = 0;

static int write_obj_flag = 1;

static int codegen_flag = 1;

static int compile_flag = 1, c_flag = 0;

static int exec_flag = 0;

int shared_flag = 0;

int static_flag = 0;

#ifdef STD_LIBDIR
int eshared_flag = 1;
#else
int eshared_flag = 0;
#endif
int rmc_flag = 1;

int genDebug = 0, pcode_flag = 0, pc_flag = 0, xpc_flag = 0, asm_flag = 0;

int optLevel = 0;

int vmLongJmps = 0;

int verbose = 1;

int main_flag = 0;

int nomain_flag = 0;

int auto_memvar = 0;

const char *std_ch_filename = "ci_std.ch";

char *CC = "gcc";

char *COMPILE_FLAG = "-c";

char *CFLAGS = "";

char *ADDCFLAGS = ADD_CFLAGS;

char *COPT = "-O2";

char *CDBG = "-g";

char *OUT_FLAG = "-o";

char *INCLUDE_FLAG = "-I";

char *OBJSUF = ".o";

char *SOBJSUF = ".so";

char *LIBSUF = ".a";

char *SLIBSUF = DLLSUFF;

char *CLIPLIB = "libclip.a";

char *MATHLIB = "-lm";

char *CLIPLIBS = "";

char *CLIP_MODULE = 0;

/*char *DLLIB = "-ldl"; */
char *CLIPSLIB = "libclip" DLLSUFF;

char *SFLAGS = "-shared";

char *LINK = "gcc";

static char *oname = 0;

int namespace_warning = 0;

char *clic_charset = 0;

char *outdir = 0;

char *out_charset = 0;

int names_flag = 0;

Coll poName;

Coll paName;

Coll include_files;

Coll lib_dirs;

Coll arglibs;

char *prgname;

#ifdef USE_AS
static int use_asm = 1;
#else
static int use_asm = 0;
#endif
#include <clip/static_get_env.c>

#define GETENV(val) get_env(val,getenv(val))
#include <clip/static_getEnvironment.c>

#undef GETENV
#include <clip/static_getrc.c>

#define flag_STATIC 257
#define flag_FULLSTATIC 258
#include <clip/static_struct_option_long_options.c>

#include <clip/get_opt.c>
#include <clip/static_diff_clock.c>

static char **ovect = 0;

static int onum = 0;

#include <clip/static_add_name.c>

char *__progname;

#include <clip/static_lib_name.c>
#include <clip/static_init_module.c>
#include <clip/static_write_names.c>
#include <clip/static_check_names.c>
#include <clip/static_pr_errorcount.c>
#include <clip/main.c>
