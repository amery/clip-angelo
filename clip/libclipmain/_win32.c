/*
	Copyright (C) 2003  Mauricio Abre
	Author   : Mauricio Abre <maurifull@datafull.com>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/

#include <ci_clip.h>
#include <ci_clipcfg.h>
#if defined(OS_MINGW) || defined(OS_CYGWIN)
#include <io.h>
#include <ci__win32.h>
#endif

#ifdef OS_MINGW
#define get_osfhandle _get_osfhandle

#include <_win32/times.c>
#include <_win32/usleep.c>
#include <_win32/gettimeofday.c>
#include <_win32/getpagesize.c>
#include <_win32/inet_aton.c>
#include <_win32/tcgetattr.c>
#include <_win32/tcsetattr.c>
#include <_win32/w32fcntl.c>

#endif
