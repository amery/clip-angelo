/*
Start total new system v. 0.0
with hard coded long name variables to have clear system
Angelo GIRARDI
*/
#include <string.h>
#include <ci_clip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <fcntl.h>

#ifndef OS_MINGW
#include <termios.h>
#include <sys/wait.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#ifdef HAVE_READLINE_H
/* libreadline */
#include <readline/readline.h>
#include <readline/history.h>

#undef select

extern char *optarg;

extern int optind, opterr, optopt;
static char in_path[256] = { 0 };
static char out_path[256] = { 0 };

static FILE *fin = 0;

static int fout = -1;

static pid_t tpid = 0, mpid = 0;

static pid_t pid = 0;

static int log_level = 0;

static char *progname = "";

static const char usage_str[] = "clip_dbg local commands:\n\
\t! cmd -- execute system cmd\n\
\tIn any command part after | or > used as local pipe or redirection.\n\
\tAll readline command-editing keys are active.\n\
\tDebugged program MUST be started with --debug or --stop option.\n\
";

#ifdef _WIN32
#include <clip_dbg/HAVE_READLINE_H_static_mkfifo.c>
#endif

#include <clip_dbg/HAVE_READLINE_H_static_is_dig.c>

struct termios term, oterm;

#include <clip_dbg/HAVE_READLINE_H_static_cleanup.c>
#include <clip_dbg/HAVE_READLINE_H_static_logg.c>
#include <clip_dbg/HAVE_READLINE_H_static_sigfunc.c>
#include <clip_dbg/HAVE_READLINE_H_static_sigpipe.c>
#include <clip_dbg/HAVE_READLINE_H_static_usage.c>
#include <clip_dbg/HAVE_READLINE_H_static_sigusr.c>
#include <clip_dbg/HAVE_READLINE_H_static_split_vector.c>

static int pipe_flag = 0;

#include <clip_dbg/HAVE_READLINE_H_enum_pipe_state.h>

static FILE *pipe_tmp = 0;

static char pipe_name[L_tmpnam];

static char *pipe_command = 0;

#include <clip_dbg/HAVE_READLINE_H_static_start_pipe.c>
#include <clip_dbg/HAVE_READLINE_H_static_char_pipe.c>
#include <clip_dbg/HAVE_READLINE_H_static_process_line.c>
#include <clip_dbg/HAVE_READLINE_H_main.c>

#else

#include <clip_dbg/main.c>

#endif
