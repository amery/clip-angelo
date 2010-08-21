/*
	Copyright (C) 2001  ITK
	Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>, Sergey Rosenfeld <serg@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include <ci_clip.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef OS_MINGW
#include <sys/wait.h>
#endif
#include <sys/time.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>

#include <ci_hash.h>
#include <ci_hashcode.h>

#include <ci_error.ch>
#include <ci_fileio.ch>

#ifdef _WIN32
#include <io.h>
#endif

#ifdef OS_CYGWIN
#include <io.h>
#include <w32api/windows.h>
#include <sys/cygwin.h>
#endif

#include <ci_ncp.h>

#ifdef USE_TASKS
#include <../libcliptask/ci_task.h>
#endif

#include <_file/typedef_struct_OneTaskLock.h>
#include <_file/typedef_struct_TaskLock.h>
/* everytime buffering or not */
/* #define FILEIO_BUFFERING */

/* always read until buffer is full or timeout */
/* #define FILEIO_FULLREAD */

/* always write until whole buffer is flushed or timeout */
#define FILEIO_FULLWRITE

#include <_file/destroy_c_file.c>
#include <_file/_set_lock.c>
#include <_file/_clip_creat.c>
#include <_file/_clip_open.c>
#include <_file/_clip_close.c>

/* TODO? the macros below are used in different files (eg. I copied it
   from screen.c - I think it is better to put all of them in one
   header file, isn't it? */

#define	timer_add(a, b, result)							  \
  do {										  \
	(result)->tv_sec = (a)->tv_sec + (b)->tv_sec;				  \
	(result)->tv_usec = (a)->tv_usec + (b)->tv_usec;				  \
	if ((result)->tv_usec >= 1000000)						  \
	  {										  \
	++(result)->tv_sec;							  \
	(result)->tv_usec -= 1000000;						  \
	  }										  \
  } while (0)

#define timer_sub(a, b, result) \
   do {                         \
	   (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;     \
	   (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;  \
	   if ((result)->tv_usec < 0) {                      \
		  --(result)->tv_sec;                        \
		  (result)->tv_usec += 1000000;              \
	   }                                                 \
   } while (0)

#include <_file/_clip_select.c>
#include <_file/_clip_read.c>
#include <_file/_clip_write.c>
#include <_file/_clip_ftype.c>
#include <_file/clip_INIT_FILE_SETS.c>
#include <_file/clip_CURRDRIVE.c>
#include <_file/clip_STARTPATH.c>
#include <_file/clip_EXENAME.c>
#include <_file/clip_CHMOD.c>
#include <_file/clip_FOPEN.c>
#include <_file/clip_FCREATE.c>
#include <_file/clip_FCLOSE.c>
#include <_file/clip_FILENO.c>
#include <_file/clip_FTYPE.c>
#include <_file/clip_FTIMEOUT.c>
#include <_file/_clip_fready.c>
#include <_file/clip_FREADREADY.c>
#include <_file/clip_FWRITEREADY.c>
#include <_file/clip_FREAD.c>
#include <_file/clip___FILELOCK.c>
#include <_file/clip_FREADSTR.c>
#include <_file/clip_FILEGETSTR.c>
#include <_file/clip_FWRITE.c>
#include <_file/clip_FWRITELN.c>
#include <_file/clip_FFLUSH.c>
#include <_file/clip_FSEEK.c>
#include <_file/clip_FTRUNC.c>
#include <_file/clip_FILEEOF.c>
#include <_file/clip_FTELL.c>
#include <_file/clip_FLOCKF.c>
#include <_file/clip_FLENGTH.c>
#include <_file/clip_FERASE.c>
#include <_file/clip_FRENAME.c>
#include <_file/clip_GETENV.c>
#include <_file/clip_GETENVA.c>
#include <_file/clip_GETE.c>
#include <_file/clip_PUTENV.c>
#include <_file/clip_SETENV.c>
#include <_file/_clip_parse_path.c>
#include <_file/clip_PARSEFILEPATH.c>
#include <_file/clip_PARSEFILENAME.c>
#include <_file/clip_ISPRINTER.c>
#include <_file/clip_OS.c>
#include <_file/clip_MAKEPATH.c>
#include <_file/clip_MEMOREAD.c>
#include <_file/clip_MEMOWRIT.c>
#include <_file/clip_MEMOWRITE.c>
#include <_file/clip_FERROR.c>
#include <_file/clip_FERRORSTR.c>
#include <_file/clip_DOSERROR.c>
#include <_file/clip_FILE.c>
#include <_file/clip_DIRECTORY.c>
#include <_file/_clip_fileStrModeToNumMode.c>
#include <_file/static__clip_wrlock.c>
#include <_file/static__clip_rdlock.c>
#include <_file/_clip_setlock.c>
#include <_file/_clip_unlock.c>
