/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */
#include <ci_clip.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <ci_clic.h>
#include <ci_clipcfg.h>

static FILE *out = 0;

static char *fname = 0, *mname = 0;

static char *file_name = 0;

#include <locale/static_make_dir.c>
#include <locale/static_set_locale.c>
#include <locale/set_locale_name.c>
#include <locale/resume_locale.c>
#include <locale/static_put_char.c>
#include <locale/static_put_str.c>
#include <locale/put_locale_string.c>
#include <locale/put_locale_string_plural.c>
