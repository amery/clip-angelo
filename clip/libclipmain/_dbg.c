/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

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
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef OS_MINGW
#include <sys/wait.h>
#endif
#include <sys/time.h>

#include <ci_error.ch>

#ifndef _WIN32
static char in_path[256] = { 0 };
static char out_path[256] = { 0 };

static FILE *fin = 0;

static FILE *fout = 0;

static pid_t pid = 0, tpid = 0;

static int cleanup_installed = 0;

#include <_dbg/static_cleanup.c>
#include <_dbg/static_dbg_signa.c>
#endif

#include <_dbg/clip_DBGCONNECT.c>
#include <_dbg/clip_DBGCLOSE.c>
#include <_dbg/clip_DBGCOMMAND.c>
