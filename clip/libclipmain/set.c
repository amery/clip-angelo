/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>

#include <ci_clip.h>
#include <ci_clipcfg.h>

#ifdef OS_CYGWIN
#include <io.h>
#endif

#include <ci_set.ch>
#include <ci_screen.h>
#include <ci_hashcode.h>
#include <ci_rational.h>
#include <ci_charset.h>

extern char *_get_unix_name(ClipMachine * ClipMachineMemory, char *dname);

static void set_printer_charset(ClipMachine * ClipMachineMemory);

#include <set/static_set_flag0.c>
#include <set/static_set_flag.c>
#include <set/static_set_flag1.c>
#include <set/static__clip_addExtToFile.c>
#include <set/static_set_color.c>
#include <set/static_get_color.c>
#include <set/_clip_close_printer.c>
#include <set/clip___SETCENTURY.c>
#include <set/clip_SETCANCEL.c>
#include <set/clip_SET.c>
#include <set/clip_SETCOLOR.c>
#include <set/clip_COLORTON.c>
#include <set/clip_FT_COLOR2N.c>
#include <set/clip_NTOCOLOR.c>
#include <set/clip_INVERTATTR.c>
#include <set/clip_COLORSELECT.c>
#include <set/clip_STANDARD.c>
#include <set/clip_UNSELECT.c>
#include <set/clip_ENHANCED.c>
#include <set/clip_SETCLEARB.c>
#include <set/clip_GETCLEARB.c>
#include <set/clip_SETCLEARA.c>
#include <set/clip_GETCLEARA.c>
#include <set/static_set_printer_charset.c>
