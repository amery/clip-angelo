/*
   Copyright (C) 2001  ITK
   Author   : Paul Lasarev <paul@itk.ru>
   License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
/*
   TODO:
   +step without entrance in funcs
   +switch to turn off debugging in codeblocks
   +ClipVar serialisation

   delete breakpoint
   db workplace info
   ClipMachine flags info
 */

#include <ci_clipcfg.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#ifdef OS_MINGW
#include <ltdl.h>
#else
#include <dlfcn.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <ci_clip.h>
#include <ci_clipvm.h>
#include <ci_hash.h>
#include <ci_screen.h>
#include <ci_error.ch>
#include <ci_hashcode.h>
#include <ci_rational.h>
#include <ci_charset.h>
#include <ci_coll.h>
#include <ci_rdd.h>

int _clip_debugnext = 0;

typedef int DbgCommand(ClipMachine * ClipMachineMemory, int argc, char **argv);

#include <clipdbg/typedef_struct_DbgCommandBucket.h>

static void delete_ClipBreakPoint(void *bp);

static int compare_ClipBreakPoint(void *p1, void *p2);
static ClipBreakPoint cur_bp = { 0, 0, 0 };

ClipBreakPoint *_clip_breakpoint = 0;

static Coll breakpoints = { 0, 0, 0, 0, 0, 0 };
static Coll sbreakpoints = { 0, 0, 0, 0, 0, 0 };

static Coll watchs = { 0, 0, 0, 0, 0, 0 };
static Coll swatchs = { 0, 0, 0, 0, 0, 0 };

static void print_dbg(ClipMachine * ClipMachineMemory, ClipVar * vp, int binary);

static DbgCommandBucket dbg_buckets[];

static int debug_flag = 0;	/* will be setted by _clip_sigdebug,

				   that means - there is data on debugger input socket
				 */
static int debugging = 0;

static int in_breakpoint = 0;

static ClipBreakPoint *prev_bp = 0;

FILE *_clip_dbg_in = 0;

static int dbg_in_fd = -1;

FILE *_clip_dbg_out = 0;

static Coll *dbg_commands = 0;

static int cur_frame = 0;

static ClipFrame *get_frame(ClipMachine * ClipMachineMemory);

int _clip_process_dbg(ClipMachine * ClipMachineMemory, char *cmd);

static int in_sigdebug = 0;

static int next_line = 0;

static char *next_file = 0, *next_proc = 0;

static int until_line = 0;

static char *until_file = 0;

static int bp_in_codeblock = 0;

static const char *nullstr(const char *str);

#include <clipdbg/static_split_vector.c>
#include <clipdbg/static_get_frame.c>
#include <clipdbg/static_find_dbg_command.c>
#include <clipdbg/static_expr_command.c>
#include <clipdbg/static_kill_command.c>
#include <clipdbg/static_help_command.c>
#include <clipdbg/static_nullstr.c>
#include <clipdbg/static_frame_command.c>
#include <clipdbg/static_bt_command.c>
#include <clipdbg/static_ls_command.c>
#include <clipdbg/static_is_dig.c>
#include <clipdbg/static_parse_name.c>
#include <clipdbg/static_print_dbg.c>
#include <clipdbg/static_print_var.c>
#include <clipdbg/static_print_command.c>
#include <clipdbg/static_delete_ClipBreakPoint.c>
#include <clipdbg/static_compare_ClipBreakPoint.c>
#include <clipdbg/static_bp_command.c>
#include <clipdbg/static_rm_command.c>
#include <clipdbg/static_continue_command.c>
#include <clipdbg/static_interrupt_command.c>
#include <clipdbg/static_next_command.c>
#include <clipdbg/static_step_command.c>
#include <clipdbg/static_until_command.c>
#include <clipdbg/static_print_wa.c>
#include <clipdbg/static_data_command.c>
#include <clipdbg/static_option_command.c>
#include <clipdbg/static_watch_command.c>

#include <clipdbg/_clip_sigdebug.c>
#include <clipdbg/_clip_sigint.c>
#include <clipdbg/_clip_sigint_real.c>
#include <clipdbg/_clip_line.c>
#include <clipdbg/_clip_in_breakpoint.c>
#include <clipdbg/_clip_debug.c>
#include <clipdbg/_clip_process_dbg.c>

static DbgCommandBucket dbg_buckets[] = {
   {"?", help_command,
    "help [command]\n\tlist all commands\n\tif <command> is given, print help about command"},
   {"break", bp_command,
    "break[point] [line|procname|filename:line] -- set/show breakpoints"},
   {"cont", continue_command, "cont[inue]  -- continue execution"},
   {"data", data_command, "data[base] [alias] [subcommand] -- database info"},
   {"expr", expr_command, "expr <expression> -- eval expression"},
   {"frame", frame_command,
    "frame [frame_no] -- set/print current frame info"},
   {"int", interrupt_command, "int[errupt]  -- interrupt execution"},
   {"help", help_command,
    "help [command]\n\tlist all commands\n\tif <command> is given, print help about command"},
   {"ls", ls_command,
    "ls [local|static|private|public] -- print list of variables"},
   {"next", next_command, "next -- execute up to line differeint line "},
   {"option", option_command, "option opt [val] -- set/show debug options:\n\
\tcodeblock [0|1] - disable/enable breakpoints in codeblocks\n\
"},
   {"print", print_command,
    "print [-local|-static|-private|-public|-memvar|-field|-binary] name [..nameN] -- print variable value"},
   {"rm", rm_command,
    "rm [line|procname|filename:line] -- remove breakpoints"},
   {"quit", kill_command,
    "quit [exitcode]\n\tterminate program with code exitcode"},
   {"step", step_command,
    "step -- step program, proceeding through function calls"},
   {"trace", bt_command, "trace  -- print call trace"},
   {"until", until_command, "until [[file:]line] -- execute until the line"},
   {"watch", watch_command, "watch [-clear]|[expr] -- watch expression"},
   {0, 0, 0}
};
