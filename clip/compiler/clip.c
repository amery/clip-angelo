/*
    Copyright (C) 2001  ITK
    Author   : Paul Lasarev <paul@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/

#include "ci_clip.h"
#include "ci_clipcfg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifndef OS_MINGW
#include <sys/times.h>
#else
#include "ci__win32.h"
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <locale.h>

#include "ci_clic.h"
#include "getopt.h"
#include "license.h"

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

static void
get_env(char *name, char *val)
{
   if (!name || !val || !*val)
      return;

   if (!strcmp(name, "CC"))
      CC = val;
   else if (!strcmp(name, "CLIPROOT"))
      CLIPROOT = strdup(val);
   else if (!strcmp(name, "CLIP_LOCALE_ROOT"))
      CLIP_LOCALE_ROOT = strdup(val);
   else if (!strcmp(name, "CLIP_MODULE"))
      CLIP_MODULE = strdup(val);
   else if (!strcmp(name, "CFLAGS"))
      CFLAGS = strdup(val);
   else if (!strcmp(name, "COPT"))
      COPT = strdup(val);
   else if (!strcmp(name, "CDBG"))
      CDBG = strdup(val);
   else if (!strcmp(name, "OUT_FLAG"))
      OUT_FLAG = strdup(val);
   else if (!strcmp(name, "INCLUDE_FLAG"))
      INCLUDE_FLAG = strdup(val);
   else if (!strcmp(name, "OBJSUF"))
      OBJSUF = strdup(val);
   else if (!strcmp(name, "SOBJSUF"))
      SOBJSUF = strdup(val);
   else if (!strcmp(name, "LIBSUF"))
      LIBSUF = strdup(val);
   else if (!strcmp(name, "SLIBSUF"))
      SLIBSUF = strdup(val);
   else if (!strcmp(name, "COMPILE_FLAG"))
      COMPILE_FLAG = strdup(val);
   else if (!strcmp(name, "SFLAGS"))
      SFLAGS = strdup(val);
   else if (!strcmp(name, "CLIPLIB"))
      CLIPLIB = strdup(val);
   else if (!strcmp(name, "CLIPLIBS"))
      CLIPLIBS = strdup(val);
   else if (!strcmp(name, "CLIPSLIB"))
      CLIPSLIB = strdup(val);
   else if (!strcmp(name, "CLIP_NAMES"))
      {
	 if (!strcasecmp(val, "yes") || !strcasecmp(val, "on"))
	    names_flag = 1;
	 else if (!strcasecmp(val, "no") || !strcasecmp(val, "off"))
	    names_flag = 0;
	 else
	    names_flag = atoi(val);
      }
   else if (!strcmp(name, "CLIP_CMDSTR"))
      {
	 int argc;

	 char *str = strdup(val);

	 char *argv[32], *s;

	 for (argc = 0; argc < 32 && (s = strtok(str, ",")) != NULL; ++argc)
	    {
	       argv[argc] = s;
	       str = NULL;
	    }
	 get_opt(argc, argv);
      }
#ifdef USE_AS
   else if (!strcmp(name, "CLIP_ASM"))
      {
	 if (!strcasecmp(val, "yes") || !strcasecmp(val, "on"))
	    use_asm = 1;
	 else if (!strcasecmp(val, "no") || !strcasecmp(val, "off"))
	    use_asm = 0;
      }
#endif

}

#define GETENV(val) get_env(val,getenv(val))
static void
getEnvironment()
{
   GETENV("CC");
   GETENV("CFLAGS");
   GETENV("COPT");
   GETENV("CDBG");
   GETENV("OUT_FLAG");
   GETENV("INCLUDE_FLAG");
   GETENV("OBJSUF");
   GETENV("SOBJSUF");
   GETENV("LIBSUF");
   GETENV("SLIBSUF");
   GETENV("COMPILE_FLAG");
   GETENV("SFLAGS");
   GETENV("CLIPLIB");
   GETENV("CLIPLIBS");
   GETENV("CLIPROOT");
   GETENV("CLIP_LOCALE_ROOT");
   GETENV("CLIPSLIB");
   GETENV("CLIP_CMDSTR");
   GETENV("CLIP_MODULE");
   GETENV("CLIP_NAMES");
#ifdef USE_AS
   GETENV("CLIP_ASM");
#endif

}

#undef GETENV

static void
getrc(char *name)
{
   FILE *file;

   int argc = 0;

   char **argv = 0;

   char buf[256];

   file = fopen(name, "rt");
   if (!file)
      return;

   while (fgets(buf, sizeof(buf), file))
      {
	 int l;

	 char *s, *p;

	 l = strlen(buf);
	 if (l < 1)
	    continue;

	 while ((--l >= 0) && (buf[l] == '\n' || buf[l] == '\r'))
	    buf[l] = 0;

	 for (s = buf; *s && isspace(*s); s++, l--)
	    ;

	 if (l < 1)
	    continue;

	 if (*s == '#')
	    continue;

	 for (p = s; *p && !isspace(*p); p++)
	    ;
	 if (*p)
	    {
	       *p = 0;
	       for (p++; *p && isspace(*p); p++)
		  ;
	    }

	 argv = (char **) realloc(argv, sizeof(char *) * (argc + 1));

	 argv[argc++] = strdup(s);
	 if (*p)
	    {
	       argv = (char **) realloc(argv, sizeof(char *) * (argc + 1));

	       argv[argc++] = strdup(p);
	    }
      }

   fclose(file);
   if (argc > 0)
      get_opt(argc, argv);
/*
   while(argc)
   free(argv[--argc]);
 */
   free(argv);
}

#define flag_STATIC 257
#define flag_FULLSTATIC 258

static struct option long_options[] = {
   {"auto-memvar", optional_argument, 0, 'a'},
   {"namespace-warning", optional_argument, 0, 'w'},
   {"verbose", optional_argument, 0, 'v'},
   {"compile", no_argument, 0, 'c'},
   {"main", no_argument, 0, 'm'},
   {"nomain", no_argument, 0, 'n'},
   {"output", required_argument, 0, 'o'},
   {"outdir", required_argument, 0, 'd'},
   {"optimise", optional_argument, 0, 'O'},
   {"include-path", required_argument, 0, 'I'},
   {"library-path", required_argument, 0, 'L'},
   {"define", required_argument, 0, 'D'},
   {"show-syntax-tree", no_argument, 0, 't'},
   {"debug", optional_argument, 0, 'g'},
   {"exec", no_argument, 0, 'e'},
   {"pcode", no_argument, 0, 'p'},
   {"c-pcode", no_argument, 0, 'l'},
   {"shared", no_argument, 0, 's'},
   {"shared-exec", no_argument, 0, 'r'},
   {"preprocess", no_argument, 0, 'P'},
   {"show-command", no_argument, 0, 'C'},
   {"use-std-ch", required_argument, 0, 'U'},
   {"source-charset", required_argument, 0, 'S'},
   {"target-charset", required_argument, 0, 'T'},
   {"environment", required_argument, 0, 'E'},
   {"help", no_argument, 0, 'h'},
   {"help-environment", no_argument, 0, 'H'},
   {"version", no_argument, 0, 'V'},
   {"noremove-c", no_argument, 0, 'R'},
   {"compat", required_argument, 0, 'q'},
   {"names", no_argument, 0, 'N'},
   {"asm", no_argument, 0, 'x'},
   {"static", no_argument, 0, flag_STATIC},
   {"full-static", no_argument, 0, flag_FULLSTATIC},
   {0, 0, 0, 0}
};

void
get_opt(int arg_c, char **arg_v)
{
   int ch;

   int option_index = 0;

   int argc = arg_c + 1, i;

#ifdef OS_MINGW
   char **argv = (char **) malloc(sizeof(char *) * (arg_c + 1));
#else
   char **argv = (char **) alloca(sizeof(char *) * (arg_c + 1));
#endif

   argv[0] = prgname;
   for (i = 0; i < arg_c; i++)
      argv[i + 1] = arg_v[i];

   optind = 0;
   while ((ch = getopt_long(argc, argv, "+v::T:S:o:d:I:i:D:d:E:L:U:u:q:MmnhHaecrsplVgbtPCOwRNx", long_options, &option_index)) != EOF)
      {
/*		printf("get_opt: ch=%c argc=%d optind=%d\n", ch, argc, optind);*/
	 switch (ch)
	    {
	    case 'a':
	       if (optarg)
		  auto_memvar = atoi(optarg);
	       else
		  auto_memvar = 1;
	       break;
	    case 'w':
	       if (optarg)
		  namespace_warning = atoi(optarg);
	       else
		  namespace_warning = 1;
	       break;
	    case 'v':
	       if (optarg)
		  {
		     if (isdigit(optarg[0]))
			verbose = atoi(optarg);
		     else
			{
			   for (verbose = 1; *optarg; optarg++)
			      if (*optarg == 'v')
				 verbose++;
			}
		  }
	       else
		  ++verbose;
	       break;
	    case 'c':
	       compile_flag = 0;
	       c_flag = 1;
	       break;
	    case 'M':
	    case 'm':
	       main_flag = 1;
	       break;
	    case 'n':
	       nomain_flag = 1;
	       break;
	    case 'o':
	       oname = optarg;
	       break;
	    case 'O':
	       if (optarg)
		  optLevel = atoi(optarg);
	       else
		  ++optLevel;
	       break;
	    case 'I':
	    case 'i':
	       append_Coll(&includePaths, strdup(optarg));
	       break;
	    case 'L':
	       append_Coll(&lib_dirs, strdup(optarg));
	       break;
	    case 'D':
	       append_Coll(&predefines, strdup(optarg));
	       break;
	    case 'd':
	       outdir = optarg;
	       break;
	    case 't':
	       ++syntax_tree_flag;
	       break;
	    case 'g':
	    case 'b':
	       if (optarg)
		  genDebug = atoi(optarg);
	       else
		  ++genDebug;
	       break;
	    case 'e':
	       exec_flag = 1;
	       break;
	    case 'p':
	       pcode_flag = 1;
	       break;
	    case 'l':
	       pc_flag = 1;
	       break;
	    case 'x':
	       xpc_flag = 1;
	       break;
	    case 's':
	       shared_flag = 1;
	       break;
	    case 'r':
	       eshared_flag = 1;
	       break;
	    case flag_STATIC:
	       shared_flag = 0;
	       eshared_flag = 0;
	       static_flag = 1;
	       break;
	    case flag_FULLSTATIC:
	       shared_flag = 0;
	       eshared_flag = 0;
	       static_flag = 1;
	       CFLAGS = "-static";
	       break;
	    case 'P':
	       ++preproc_flag;
	       break;
	    case 'C':
	       ++command_flag;
	       break;
	    case 'R':
	       rmc_flag = 0;
	       break;
	    case 'U':
	    case 'u':
#if 0
	       if (!optarg || !*optarg)
		  {
		     std_ch_filename = NULL;
		     freeAll_Coll(&include_files);
		  }
	       else
#else
	       if (optarg && *optarg)
#endif
		  {
		     std_ch_filename = NULL;
		     append_Coll(&include_files, strdup(optarg));
		  }
	       break;
	    case 'S':
	       sourceCharset = strdup(optarg);
	       if (!preproc_flag)
		  v_printf(2, "set source charset to %s\n", sourceCharset);
	       changeCharset = 1;
	       break;
	    case 'T':
	       targetCharset = clic_charset = strdup(optarg);
	       if (!preproc_flag)
		  v_printf(2, "set target charset to %s\n", targetCharset);
	       changeCharset = 1;
	       break;
	    case 'E':
	       {
		  char *s, *p;

		  s = strdup(optarg);
		  p = strchr(s, '=');
		  if (!p)
		     {
			yyerror("invalid argument for -E: '%s'", optarg);
			break;
		     }
		  else
		     {
			*p = 0;
			get_env(s, p + 1);
		     }
		 /*free(s); */
	       }
	       break;
	    case 'q':
	       if (!strcmp(optarg, "as-word"))
		  compat_as_word = 1;
	       else if (!strcmp(optarg, "no-as-word"))
		  compat_as_word = 0;
	       else
		  yyerror("invalid compatability word: '%s'\n" "possible values: as-word, no-as-word", optarg);
	       break;
	    case 'N':
	       names_flag = 1;
	       break;
	    default:
	       fprintf(stdout, _clic_gettext("try '%s --help' for options details\n"), prgname);
	       exit(3);
	    case 'h':
	       {
		  fprintf(stdout, _clic_gettext("Usage: %s [options] [<filename> ...]\n\
options:\n\
 -h, --help\n\
	this help\n\
 -H, --help-environment\n\
	help about acceptable environment variables\n\
 -V, --version\n\
	print clip version\n\
 -v[<level>], --verbose[=<level>]\n\
	set/increase verbose level\n\
 -a, --auto-memvar[=1|0]\n\
	set auto memvar variables mode\n\
 -w, --namespace-warning[=1|0]\n\
	set warning about implicit var declaration\n\
 -e, --exec\n\
	generate executable file\n\
 -c, --compile\n\
	stop after generation C code\n\
 -g,-b, --debug[=1|0]\n\
	generate debug info\n\
 -t, --show-syntax-tree\n\
	print program syntax tree\n\
 -o<name>, --output=<name>\n\
	set output file name\n\
 -d<name>, --outdir=<name>\n\
	set output directory\n\
 -O, --optimise[=<val>]\n\
	set optimisation level\n\
 -M, --main\n\
	use first <filename> as start program point\n\
 -n, --nomain\n\
	don't declare module procedure\n\
 -p, --pcode\n\
	generate pseudocode module\n\
 -l, --c-pcode\n\
	compile C-pseudocode module\n\
 -s, --shared\n\
	compile into shared object\n\
 -r, --shared-exec\n\
	always when possible link with shared libraries\n\
 --static\n\
	static link CLIP libraries to executable\n\
 --full-static\n\
	static link all libraries to executable\n\
 -I<path>, --include-path=<path>\n\
	add include search path\n\
 -L<path>, --library-path=<path>\n\
	add library path\n\
 -D<name>, --define=<name>\n\
	define macro name\n\
 -D<name=value>, --define=<name=value>\n\
	define macro name as <value>\n\
 -U[<filename>], --use-std-ch[=<filename>]\n\
	use alternate (or none) standard include file (default==ci_std.ch)\n\
 -S<charset>, --source-charset\n\
	define source charset of string constants\n\
 -T<charet>, --target-charset\n\
	define target charset of string constants\n\
 -P, --preprocess\n\
	preprocess only\n\
 -C, --show-command\n\
	print command definitions\n\
 -R, --noremove-c\n\
	do not remove temporary '.c' files\n\
 -N, --names\n\
	warning on unresolved function names (see CLIP_NAMES envar)\n\
 -E<name=value>, --environment=<name=value>\n\
	exact to set same environ variable(see -H for details)\n\
 -q <word>, --compat=<word>\n\
	set syntax compatability modes:\n\
		no-as-word  	- default value\n\
		as-word\n\
\n\
 <filename> files to process\n\
\n\
 all above options may be placed in .cliprc file in current directory,\n\
 in $HOME/.cliprc file, or in any file in $CLIPROOT/cliprc directory\n\
"), prgname);
		  exit(2);
	       }
	    case 'H':
	       {
		  fprintf(stdout, _clic_gettext("\n\
%s can accept next environment variables:\n\
\n\
CC\n\
\tname of C compiler (default=='%s')\n\
CFLAGS\n\
\toptions for C compiler (default=='%s')\n\
COMPILE_FLAG\n\
\t'compile-only' flag for C compiler (default=='%s')\n\
COPT\n\
\toptimisation flag for C compiler (default=='%s')\n\
CDBG\n\
\tdebug flag for C compiler (default=='%s')\n\
OUT_FLAG\n\
\toutput flag for C compiler (default=='%s')\n\
INCLUDE_FLAG\n\
\tinclude flag for C compiler (default=='%s')\n\
OBJSUF\n\
\tsuffix for output object files (default=='%s')\n\
SOBJSUF\n\
\tsuffix for output shared files (default=='%s')\n\
LIBSUF\n\
\tsuffix for library files (default=='%s')\n\
SLIBSUF\n\
\tsuffix for shared library files (default=='%s')\n\
SFLAGS\n\
\toptions for C compiler - shared version (default=='%s')\n\
CLIPLIB\n\
\truntime support library (default=='%s')\n\
CLIPSLIB\n\
\truntime support shared library (default=='%s')\n\
CLIPLIBS\n\
\tadditional libs (default=='%s')\n\
CLIPROOT\n\
\troot for all clip related files (default=='%s')\n\
CLIP_LOCALE_ROOT\n\
\troot for locale messages\n\
CLIP_MODULE\n\
\tcurrent locale module (default=='%s')\n\
CLIP_CMDSTR\n\
\tcomma-separated list of command-line options\n\
CLIP_NAMES\n\
\tgenerate .ex, .nm files (possible values yes/no|on/off|0/1)\n\
predefined macros:\n\
\t__FILE__      string - name of current compile file\n\
\t__BASE_FILE__ string - name of root compile file\n\
\t__LINE__      numeric - number of current line in current file\n\
\t__VERSION__   string - version of clip\n\
\t__CLIP__	string - version of clip\n\
\t__SYSTEM__	string - name of operating system\n\
\t__DATE__      string - date in format 'MON DD YYYY'\n\
\t__TIME__      string - current time in format 'HH:MM::SS'\n\
\t__CLIP_MODULE__      string - name of current locale module\n\
"), prgname, CC, CFLAGS, COMPILE_FLAG, COPT, CDBG, OUT_FLAG, INCLUDE_FLAG, OBJSUF, SOBJSUF, LIBSUF, SLIBSUF, SFLAGS, CLIPLIB, CLIPSLIB, CLIPLIBS, CLIPROOT, CLIP_MODULE);
		  exit(2);
	       }
	    case 'V':
	       printVersion(stdout);
	       fprintf(stdout, "\n");
	       exit(0);
	    }
      }
   optind--;
#ifdef OS_MINGW
   free(argv);
#endif
}

static char *
diff_clock(clock_t beg, clock_t end)
{
   static char buf[32];

   clock_t diff = end - beg;

   int /*m, */ s, ms;

   long clk_tck;

   clk_tck = sysconf(_SC_CLK_TCK);	/*CLK_TCK; */

   s = diff / clk_tck;
   ms = (diff % clk_tck) * 100 / clk_tck;
  /*m=s/60;
     s=s%60; */
   sprintf(buf, "%d.%02ds", s, ms);
   return buf;
}

static char **ovect = 0;

static int onum = 0;

static void
add_name(const char *name)
{
   char buf[256], *e, *s;

   if (!name || !*name)
      return;

   strcpy(buf, name);
   e = strrchr(buf, '.');
   s = strrchr(buf, '/');
   if (e && (!s || e > s))
      {
	 if (!strcasecmp(e, ".c") || !strcasecmp(e, ".cc") || !strcasecmp(e, ".prg"))
	    strcpy(e, OBJSUF);
	 else if (!strcasecmp(e, OBJSUF) || !strcasecmp(e, SOBJSUF) || !strcasecmp(e, LIBSUF) || !strcasecmp(e, SLIBSUF))
	    ;
	 else if (!strcasecmp(e, ".po") || !strcasecmp(e, ".pa"))
	    goto o_nam;
	 else if (buf[0] == '-' && buf[1] == 'l')
	    ;
	 else
	    return;
      }
   else
      {
	/*e = buf + strlen(buf);
	   strcpy(e, OBJSUF); */
#ifndef STD_LIBDIR
	 if (name[0] == '-' && name[1] == 'l')
	    {
	       snprintf(buf, sizeof(buf), "%s/lib/lib%s%s", CLIPROOT, name + 2, static_flag ? LIBSUF : DLLSUFF);
	       if (access(buf, R_OK))
		  strncpy(buf, name, sizeof(buf));
	    }
#endif
      }

   ovect = (char **) realloc(ovect, sizeof(char *) * (onum + 1));

   ovect[onum] = strdup(buf);
   ++onum;
 o_nam:
   if (!oname)
      {
	 strcpy(buf, name);
	 e = strrchr(buf, '.');
	 s = strrchr(buf, '/');
	 if (e && (!s || e > s))
	    *e = 0;
	 else
	    e = buf + strlen(buf);
#if 0
	 if (shared_flag && !exec_flag)
	    strcpy(e, SOBJSUF);
#endif
	 oname = strdup(buf);
      }
}

char *__progname;

static void
lib_name(char *buf, int buflen, char *root, char *prefix, char *e, int elen)
{
   if ((!root || !root[0]) && elen > 3 && !memcmp(e, "lib", 3))
      {
	 int l;

	 char *s = strrchr(e + 3, '.');

	 if (s)
	    l = s - (e + 3);
	 else
	    l = elen - 3;
	 snprintf(buf, buflen, "-l%.*s", l, e + 3);
      }
   else if (!root || !root[0] || e[0] == '/' || (e[0] == '.' && e[1] == '/') || (e[0] == '.' && e[1] == '.' && e[2] == '/'))
      snprintf(buf, buflen, "%.*s", elen, e);
   else
      snprintf(buf, buflen, "%s/%s/%.*s", root, prefix, elen, e);
}

static void
init_module(void)
{
   char path[256], *s;

   if (!getcwd(path, sizeof(path)))
      {
	 CLIP_MODULE = "root";
	 return;
      }

   s = strrchr(path, '/');
   if (s)
      s++;
   else
      s = path;

   if (!strcmp(s, "/"))
      s = "root";

   CLIP_MODULE = strdup(s);
}

static void
write_names(File * file)
{
   char path[256];

   char *e;

   int l;

   FILE *out;

   int i;

   if (!names_flag)
      return;

   snprintf(path, sizeof(path), "%s", file->cname_of_File);
   e = strrchr(path, '.');
   if (e)
      l = e - path;
   else
      l = strlen(path);

   snprintf(path + l, sizeof(path) - l, ".nm");

   out = fopen(path, "wt");
   if (out)
      {
	 for (i = 0; i < file->Coll_functions_of_File.count; i++)
	    {
	       Function *fp = (Function *) file->Coll_functions_of_File.items[i];

	       if (fp->isPublic_of_Function != 1)
		  continue;
	       fprintf(out, "%s\n", fp->name_of_Function);
	    }
	 fclose(out);
      }
   else
      yywarning("cannot open defname file '%s': %s", path, strerror(errno));

   snprintf(path + l, sizeof(path) - l, ".ex");

   out = fopen(path, "wt");
   if (out)
      {
	 for (i = 0; i < file->Coll_undeclExternFunctions_of_File.count; i++)
	    {
	       char *s = (char *) file->Coll_undeclExternFunctions_of_File.items[i];

	       fprintf(out, "%s\n", s);
	    }
	 fclose(out);
      }
   else
      yywarning("cannot open exname file '%s': %s", path, strerror(errno));

}

static void
check_names(Coll * ex, Coll * nm)
{
   int i;

   char *s;

   if (!names_flag)
      return;

   for (i = 0; i < ex->count; i++)
      {
	 int no;

	 s = (char *) ex->items[i];
	 if (!search_Coll(nm, s, &no))
	    yywarning("possible not defined function %s", s);
      }
}

static void
pr_errorcount(int level)
{
   char fmt[128];

   snprintf(fmt, sizeof(fmt), "%%d %s\n", _clic_ngettext("error", "errors", clic_errorcount));

   vr_printf(level, fmt, clic_errorcount);
}

int
main(int argc, char **argv)
{
   int /*ch, */ i, ii, ret, err_flag = 0;

   clock_t beg, end, Beg, End;

   int files = 0;

   struct tms ts;

   char buf[256];

   char *e;

   prgname = strrchr(argv[0], '/');

   if (prgname)
      prgname++;
   else
      prgname = argv[0];

   e = getenv("CLIP_HOSTCS");
   if (e && *e)
      {
	 sourceCharset = targetCharset = strdup(e);
      }
   else if (!e)
      {
	 e = getenv("CLIP_LANG");
	 if (e == NULL)
	    e = getenv("LANG");
	 if (!e || !*e || !strcmp(e, "C"))
	    e = getenv("LC_MESSAGES");
	 if (!e || !*e || !strcmp(e, "C"))
	    e = getenv("LC_ALL");
	 if (e && *e)
	    {
	       char *s = strrchr(e, '.');

	       if (s)
		  {
		     snprintf(buf, sizeof(buf), "%s", s + 1);
		     for (s = buf; *s; s++)
			*s = tolower(*s);
		     sourceCharset = targetCharset = strdup(buf);
		  }
	    }
      }

   {
      e = getenv("CLIP_LANG");
      if (e == NULL)
	 e = getenv("LANG");
      if (!e || !*e || !strcmp(e, "C"))
	 e = getenv("LC_MESSAGES");
      if (!e || !*e || !strcmp(e, "C"))
	 e = getenv("LC_ALL");
      if (e && *e)
	 {
	    char *s = strrchr(e, '.');

	    if (s)
	       {
		  snprintf(buf, sizeof(buf), "%s", s + 1);
		  for (s = buf; *s; s++)
		     *s = tolower(*s);
		  out_charset = strdup(buf);
	       }
	 }
   }

   {
      char *locale;

      locale = getenv("CLIP_LANG");
      if (!locale || !*locale)
	 locale = getenv("LANG");
     /*if (locale && *locale &&
        strcasecmp(locale, "C") && strcasecmp(locale, "POSIX")) */
     /*
        setlocale(LC_ALL, locale);
      */
   }

   if (!sourceCharset)
      sourceCharset = targetCharset = strdup("c");

   getEnvironment();

   init_Coll(&includePaths, NULL, NULL);
   init_Coll(&lib_dirs, NULL, NULL);
   init_Coll(&arglibs, NULL, NULL);

#if 1
   insert_Coll(&includePaths, ".");
   snprintf(buf, sizeof(buf), "%s/include", CLIPROOT);
   insert_Coll(&includePaths, strdup(buf));
#ifdef STD_LIBDIR
   snprintf(buf, sizeof(buf), STD_LIB_DIR);
   insert_Coll(&lib_dirs, strdup(buf));
#endif
   snprintf(buf, sizeof(buf), "%s/lib", CLIPROOT);
   insert_Coll(&lib_dirs, strdup(buf));
#endif
   init_Coll(&predefines, NULL, NULL);
   init_Coll(&poName, NULL, NULL);
   init_Coll(&paName, NULL, NULL);
   init_Coll(&include_files, NULL, NULL);

   snprintf(buf, sizeof(buf), "__CLIP__=\"%s\"", CLIP_VERSION);
   append_Coll(&predefines, strdup(buf));

   init_module();

   {
      char buf[256], *s;

      s = getenv("HOME");
      if (s && *s)
	 {
	    snprintf(buf, sizeof(buf), "%s/.cliprc", s);
	    getrc(buf);
	 }

   }

   getrc(".cliprc");

   {
      char buf[256], *s;

      DIR *dp;

      s = CLIPROOT;
      if (s && *s)
	 {
	    snprintf(buf, sizeof(buf), "%s/.cliprc", s);
	    getrc(buf);
	 }

      snprintf(buf, sizeof(buf), "%s/cliprc", CLIPROOT);
      dp = opendir(buf);
      if (dp)
	 {
	    struct dirent *ep;

	    struct stat st;

	    Coll files;

	    int i;

	    init_Coll(&files, free, strcmp);
	    while ((ep = readdir(dp)))
	       {
		  snprintf(buf, sizeof(buf), "%s/cliprc/%s", CLIPROOT, ep->d_name);
		  if (stat(buf, &st))
		     continue;
		  if (!S_ISREG(st.st_mode))
		     continue;
		  if (access(buf, R_OK))
		     continue;
		  insert_Coll(&files, strdup(buf));
	       }
	    closedir(dp);

	    for (i = 0; i < files.count; i++)
	       {
		  char *name = (char *) files.items[i];

		  getrc(name);
	       }

	    destroy_Coll(&files);
	 }
   }

   argc--;
   argv++;
   get_opt(argc, argv);

   argc -= optind;
   argv += optind;

   if (err_flag)
      return 1;

#if 0
   insert_Coll(&includePaths, ".");
   snprintf(buf, sizeof(buf), "%s/include", CLIPROOT);
   insert_Coll(&includePaths, strdup(buf));
#ifdef STD_LIBDIR
   snprintf(buf, sizeof(buf), STD_LIBDIR);
   insert_Coll(&lib_dirs, strdup(buf));
#endif
   snprintf(buf, sizeof(buf), "%s/lib", CLIPROOT);
   insert_Coll(&lib_dirs, strdup(buf));
#endif

   if (syntax_tree_flag)
      {
	 write_obj_flag = 0;
	 codegen_flag = 1;
	 compile_flag = 0;
	 pcode_flag = 0;
	 pc_flag = 0;
	 asm_flag = 0;
	 exec_flag = 0;
      }

   if (!write_obj_flag)
      {
	/*codegen_flag = 0; */
	 compile_flag = 0;
      }
   if (preproc_flag)
      {
	 write_obj_flag = 0;
	 codegen_flag = 0;
	 syntax_tree_flag = 0;
	 compile_flag = 0;
	 exec_flag = 0;
	 pcode_flag = 0;
	 pc_flag = 0;
	 asm_flag = 0;
	 shared_flag = 0;
      }

   if (pcode_flag)
      {
	 pc_flag = 0;
	 asm_flag = 0;
	 shared_flag = 0;
      }

   if (pc_flag)
      {
	 pcode_flag = 1;
#ifdef USE_AS
	 if (use_asm)
	    asm_flag = 1;
	 else
	    asm_flag = 0;
#endif
      }

   if (xpc_flag)
      {
	 pcode_flag = 1;
	 pc_flag = 1;
#ifdef USE_AS
	 if (use_asm)
	    asm_flag = 0;
	 else
	    asm_flag = 1;
#endif
      }

#if 0
   if (shared_flag && pcode_flag)
      {
	 v_printf(0, "conflict between -s and -p flags\n");
	 exit(1);
      }
#endif

   if (pcode_flag && c_flag)
      {
	 v_printf(0, "conflict between -c and -p flags\n");
	 exit(1);
      }

  /*if ( exec_flag && !main_flag && !shared_flag)
     {
     v_printf(0, "-e(xec) flag without -M(ain) or -s(hared) flags\n");
     exit(2);
     } */

   if (pcode_flag)
      {
	 compile_flag = 0;
      }

   if (nomain_flag && main_flag)
      {
	 v_printf(0, "conflict between -n and -m flags\n");
	 exit(1);
      }

   if (!exec_flag && oname)
      {
	 char *e;

	 if (oname[0] == '/')
	    snprintf(buf, sizeof(buf), "%s", oname);
	 else
	    snprintf(buf, sizeof(buf), "%s%s%s", outdir ? outdir : "", outdir ? "/" : "", oname);
	 e = strrchr(buf, '/');
	 if (e)
	    {
	       *e = 0;
	       outdir = strdup(buf);
	    }
      }

   if (!outdir)
      outdir = ".";

   if (outdir)
      {
	 char cdir[256];

	 getcwd(cdir, sizeof(cdir));

	 if (!chdir(outdir))
	    {
	       getcwd(buf, sizeof(buf));
	       outdir = strdup(buf);
	       chdir(cdir);
	    }
	 else
	    {
	       yyerror("cannot change to output dir '%s': %s", outdir, strerror(errno));
	       exit(1);
	    }
      }

   if (!preproc_flag)
      {
	 v_printf(2, "set source charset to %s\n", sourceCharset);
	 v_printf(2, "set target charset to %s\n", targetCharset);
      }

   init_lex();
   init_parser();

   if (argc < 1)
      ii = -1;
   else
      ii = 0;

   Beg = times(&ts);
   if (argc > 0)
      {
	 for (i = 0; i < argc; i++)
	    {
	       char *e;

	       e = argv[i];
	       if (e[0] == '-' && e[1] == 'L')
		  {
		     insert_Coll(&lib_dirs, strdup(e + 2));
		     continue;
		  }
	       e = strrchr(argv[i], '.');
	       if (!e)
		  {
		     e = argv[i];
		     if (e[0] == '-' && e[1] == 'l')
		       /*append_Coll(&arglibs, strdup(e+2)) */ ;
		     else
			yyerror("unknown file type '' file '%s'", argv[i]);
		     continue;
		  }
	       else if (!strcasecmp(e, ".po"))
		  insert_Coll(&poName, strdup(argv[i]));
	       else if (!strcasecmp(e, ".pa"))
		  insert_Coll(&paName, strdup(argv[i]));
	       else if (strcasecmp(e, ".prg") && strcasecmp(e, ".c") && strcasecmp(e, ".cc") && strcasecmp(e, OBJSUF) && strcasecmp(e, SOBJSUF) && strcasecmp(e, ".a") && strcasecmp(e, ".lib"))
		  {
		    /*yywarning("unknown file type '%s' file '%s'", e, argv[i]); */
		     continue;
		  }
	    }
      }

   for (; clic_errorcount == 0 && ii < argc; ii++)
      {
	 ++files;
	 if (ii < 0)
	    {
	       v_printf(1, "no input files, so use stdin; -h will help\n");
	       fflush(stderr);
	       set_locale_name("stdin");
	       ret = clic_parse("stdin", stdin);
	       add_name("stdin");
	    }
	 else
	    {
	       char *e;

	       e = strrchr(argv[ii], '.');
	       add_name(argv[ii]);

	       if (!e)
		  continue;
	       else if (!strcasecmp(e, ".c") || !strcasecmp(e, ".cc") || !strcasecmp(e, ".cpp"))
		  {
		     if (!preproc_flag)
			{
			   v_printf(1, "process file '%s' ..", argv[ii]);
			   v_neednl = 1;
			}

		     beg = times(&ts);
		     compile_CFile(argv[ii]);
		     end = times(&ts);

		     if (!preproc_flag)
			{
			   v_neednl = 0;
			   if (clic_errorcount == 0)
			      v_printf(1, ".. done, %s\n", diff_clock(beg, end));
			   else
			      pr_errorcount(1);
			}
		     continue;
		  }
	       else if (strcasecmp(e, ".prg"))
		  {
		     continue;
		  }

	       if (ii > 0)
		  main_flag = 0;

	       if (!preproc_flag)
		  {
		     v_printf(1, "parsing file '%s' ..", argv[ii]);
		     v_neednl = 1;
		  }
	       beg = times(&ts);
	       set_locale_name(argv[ii]);
	       ret = clic_parse(argv[ii], 0);
	       end = times(&ts);

	       if (!preproc_flag)
		  {
		     v_neednl = 0;
		     if (clic_errorcount == 0)
			v_printf(1, ".. done (%d/%d %s, %d %s, %s)\n",
				 clic_line, all_lines, _clic_ngettext("line", "lines", clic_line), clic_warncount, _clic_ngettext("warning", "warnings", clic_warncount), diff_clock(beg, end));
		     else
			vr_printf(1, "%d %s, %d %s\n", clic_errorcount, _clic_ngettext("error", "errors", clic_errorcount), clic_warncount, _clic_ngettext("warning", "warnings", clic_warncount));
		  }
	    }
	 if (ret)
	    break;

	 if (clic_errorcount == 0 && codegen_flag)
	    {
	       v_printf(2, "codegen file '%s' ..", curFile->name_of_File);
	       v_neednl = 1;
	       beg = times(&ts);
	       codegen_File(curFile);
	       end = times(&ts);
	       v_neednl = 0;
	       if (clic_errorcount == 0)
		  v_printf(2, ".. done, %s\n", diff_clock(beg, end));
	       else
		  pr_errorcount(2);
	    }
	 if (clic_errorcount == 0 && syntax_tree_flag)
	    {
	       print_File(curFile);
	    }
	 if (clic_errorcount == 0 && write_obj_flag)
	    {
	       if (pcode_flag)
		  {
		     long len;

		     v_printf(1, "writing file '%s' ..", curFile->s_cname_of_File);
		     v_neednl = 1;
		     beg = times(&ts);
		     write_OFile(curFile, &len);
		     write_names(curFile);
		     end = times(&ts);
		     v_neednl = 0;
		     if (clic_errorcount == 0)
			v_printf(1, ".. done, %ld %s ,%s\n", len, _clic_ngettext("byte", "bytes", len), diff_clock(beg, end));
		     else
			pr_errorcount(1);
		  }
	       else
		  {
		     v_printf(2, "writing file '%s' ..", curFile->s_cname_of_File);
		     v_neednl = 1;
		     write_File(curFile);
		     write_names(curFile);
		     v_neednl = 0;
		     if (clic_errorcount == 0)
			v_printf(2, ".. done\n");
		     else
			pr_errorcount(2);
		  }
	    }

	 if (clic_errorcount == 0 && (compile_flag || pc_flag))
	    {
	       if (ii)
		  main_flag = 0;
	       v_printf(1, "compile file '%s' ..", curFile->s_cname_of_File);
	       v_neednl = 1;
	       beg = times(&ts);
	       compile_File(curFile->cname_of_File);
	       end = times(&ts);
	       v_neednl = 0;
	       if (clic_errorcount == 0)
		  v_printf(1, ".. done, %s\n", diff_clock(beg, end));
	       else
		  pr_errorcount(1);

	       if (clic_errorcount == 0 && shared_flag && !exec_flag)
		  {
		     v_printf(1, "make shared object '%s' ..", curFile->s_cname_of_File);
		     v_neednl = 1;
		     beg = times(&ts);
		     share_File(curFile->cname_of_File);
		     end = times(&ts);
		     v_neednl = 0;
		     if (clic_errorcount == 0)
			v_printf(1, ".. done, %s\n", diff_clock(beg, end));
		     else
			pr_errorcount(1);

		  }
	    }

	 if (ii < 0)
	    break;

	 delete_File(curFile);
	 curFile = NULL;
      }

   if (clic_errorcount == 0 && exec_flag)
      {
	 char cmd[1024 * 8], *e;

	 char cfuncname[256], ofuncname[256];

	 char *libroot;

	 int i;

	 Coll ex, nm;

	 init_Coll(&ex, free, strcasecmp);
	 init_Coll(&nm, free, strcasecmp);
#ifdef STD_LIBDIR
	 libroot = 0;
#else
	 libroot = CLIPROOT;
#endif

	 ++files;
#ifdef STD_LIBDIR
	 if (eshared_flag || shared_flag)
	    {
	       snprintf(cmd, sizeof(cmd), "-lclip");
	       add_name(cmd);
	    }
	 else
#endif
	    {
	       e = (eshared_flag || shared_flag) ? CLIPSLIB : CLIPLIB;
	       lib_name(cmd, sizeof(cmd), libroot, "lib", e, strlen(e));
	       add_name(cmd);
	    }
	 for (e = CLIPLIBS; *e;)
	    {
	       int l;

	       l = strspn(e, " \t");
	       e += l;
	       l = strcspn(e, " \t");
	       if (!l)
		  break;
	       lib_name(cmd, sizeof(cmd), libroot, "lib", e, l);
	       add_name(cmd);
	       e += l;
	    }
	 for (e = ADDLIBS; *e;)
	    {
	       int l;

	       l = strspn(e, " \t");
	       e += l;
	       l = strcspn(e, " \t");
	       if (!l)
		  break;
	       memcpy(cmd, e, l);
	       cmd[l] = 0;
	       add_name(cmd);
	       e += l;
	    }
	 add_name(MATHLIB);
	 add_name(DLLIB);

	/* generate _cfunctions */
	 if (asm_flag)
	    sprintf(cfuncname, "%s_ref.s", oname);
	 else
	    sprintf(cfuncname, "%s_ref.c", oname);
	 sprintf(ofuncname, "%s_ref.o", oname);
	 v_printf(1, "generate reference file '%s' ..", cfuncname);
	 v_neednl = 1;
	 beg = times(&ts);
	 write_Cfunc(cfuncname, onum, ovect, &ex, &nm);
	 check_names(&ex, &nm);
	 end = times(&ts);
	 v_neednl = 0;
	 if (clic_errorcount == 0)
	    v_printf(1, ".. done, %s\n", diff_clock(beg, end));
	 else
	    pr_errorcount(1);
	 if (clic_errorcount)
	    goto end;

	 v_printf(1, "compile file '%s' ..", cfuncname);
	 v_neednl = 1;
	 beg = times(&ts);
	 compile_File(cfuncname);
	 end = times(&ts);
	 v_neednl = 0;
	 if (clic_errorcount == 0)
	    v_printf(1, ".. done, %s\n", diff_clock(beg, end));
	 else
	    pr_errorcount(1);

#ifdef USE_LD
	 if (use_asm && (shared_flag || eshared_flag))
	    {
	       int ll;

	       const char *ld_prg, *ld_end;

	       if (shared_flag || eshared_flag)
		  {
		     ld_prg = LD_PRG;
		     ld_end = LD_END;
		  }
	       else
		  {
		     ld_prg = LDS_PRG;
		     ld_end = LDS_END;
		  }

	       snprintf(cmd, sizeof(cmd), "%s", ld_prg);
	       ll = strlen(cmd);
	       for (e = cmd + ll, i = 0; i < lib_dirs.count; ++i)
		  {
		     snprintf(e, sizeof(cmd) - ll, " -L%s", (char *) lib_dirs.items[i]);
		     ll = strlen(cmd);
		     e = cmd + ll;
		  }
	       ll = strlen(cmd);
	       snprintf(cmd + ll, sizeof(cmd) - ll, " %s %s %s -o %s", optLevel ? COPT : "", genDebug ? CDBG : "", ofuncname, oname);
	       ll = strlen(cmd);
	       for (e = cmd + ll, i = 0; i < onum; ++i)
		  {
		     snprintf(e, sizeof(cmd) - ll, " %s", ovect[i]);
		     ll = strlen(cmd);
		     e = cmd + ll;
		  }
	       ll = strlen(cmd);
	       snprintf(cmd + ll, sizeof(cmd) - ll, " %s", ld_end);
	    }
	 else
#endif
	    {
	       sprintf(cmd, "%s", CC);
	       for (e = cmd + strlen(cmd), i = 0; i < includePaths.count; ++i)
		  {
		     sprintf(e, " %s %s", INCLUDE_FLAG, (char *) includePaths.items[i]);
		     e = cmd + strlen(cmd);
		  }

	       for (e = cmd + strlen(cmd), i = 0; i < lib_dirs.count; ++i)
		  {
		     sprintf(e, " -L%s", (char *) lib_dirs.items[i]);
		     e = cmd + strlen(cmd);
		  }

	       sprintf(cmd + strlen(cmd), " %s %s %s %s %s %s %s", optLevel ? COPT : "", genDebug ? CDBG : "", CFLAGS, ADDCFLAGS, ofuncname, OUT_FLAG, oname);
	       for (e = cmd + strlen(cmd), i = 0; i < onum; ++i)
		  {
		     sprintf(e, " %s", ovect[i]);
		     e = cmd + strlen(cmd);
		  }
	    }

	 v_printf(1, "make file '%s' ..", oname);
	 v_neednl = 1;
	 beg = times(&ts);
	 v_printf(2, "%s\n", cmd);
	 if (system(cmd))
	    yyerror("C level error in command: %s", cmd);
	 else if (rmc_flag)
	    {
	       unlink(cfuncname);
	       unlink(ofuncname);
	    }

	 end = times(&ts);
	 v_neednl = 0;
	 if (clic_errorcount == 0)
	    v_printf(1, ".. done, %s\n", diff_clock(beg, end));
	 else
	    pr_errorcount(1);
      }

 end:
   End = times(&ts);

   resume_parser();
   resume_lex();
   resume_locale();

   if (!preproc_flag)
      v_printf(1, "clip: %d %s, %s\n", files, _clic_ngettext("file", "files", files), diff_clock(Beg, End));
   return clic_errorcount == 0 ? 0 : 1;
}
