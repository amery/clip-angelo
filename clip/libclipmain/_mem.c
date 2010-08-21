/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>

#include <ci_clip.h>
#include <ci_error.ch>
#include <ci_rdd.h>
#include <ci_coll.h>
#include <ci_rational.h>

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))

#define DBFDATA_NONFREE 0x1

#include <_mem/typedef_struct_MemHeader.h>
#include <_mem/typedef_struct_DbfData.h>

DbfLocale *dbf_get_locale(ClipMachine * ClipMachineMemory);

int clip2dbf(ClipVar * vp, DbfData * dp);

int dbf2clip(DbfData * dp, ClipVar * vp);

#include <_mem/destroy_DbfData.c>
#include <_mem/clip2db.c>
#include <_mem/dbf2clip.c>
#include <_mem/static_mem_error.c>
#include <_mem/read_MemFile.c>
#include <_mem/write_MemFile.c>
#include <_mem/clip___MCLEAR.c>
#include <_mem/clip___MXRELEASE.c>
#include <_mem/clip___MRELEASE.c>
#include <_mem/clip___MRESTORE.c>
#include <_mem/clip___MSAVE.c>
#include <_mem/clip_MEMVARLIST.c>
