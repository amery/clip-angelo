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
