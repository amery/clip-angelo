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
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#ifndef OS_MINGW
#include <sys/wait.h>
#endif

#include <ci_error.ch>

#undef SYSTEM

#include <_system/clip_SYSCMD.c>
#include <_system/clip_PID.c>

#ifdef OS_CYGWIN
#include <w32api/windows.h>
#include <w32api/winuser.h>

#include <_system/clip___WINEXEC.c>
#endif

#include <sys/utsname.h>

#include <_system/clip_UNAME.c>
