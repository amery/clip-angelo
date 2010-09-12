/*
	Copyright (C) 2001  ITK
	Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>
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
#include <sys/time.h>
#ifndef OS_MINGW
#include <sys/times.h>
#endif

#include <ci_error.ch>
#include <ci_hashcode.h>

#ifdef OS_CYGWIN
#include <w32api/windows.h>
#endif

#include <_date/clip_STOD.c>
#include <_date/clip_CTOD.c>
#include <_date/_clip_sysdate.c>
#include <_date/clip_DATE.c>
#include <_date/clip_DTOC.c>
#include <_date/clip_DAY.c>
#include <_date/clip_DOW.c>
#include <_date/clip_DTOS.c>
#include <_date/clip_MONTH.c>
#include <_date/static__clip_seconds.c>
#include <_date/clip_SECONDS.c>
#include <_date/clip_SECOND.c>
#include <_date/static__clip_timezone.c>
#include <_date/clip_TIMEZONE.c>
#include <_date/clip_SECONDSCPU.c>
#include <_date/clip_TIME.c>
#include <_date/clip_YEAR.c>
#include <_date/clip_ADDMONTH.c>
#include <_date/clip_ADDYEAR.c>
#include <_date/clip_BOM.c>
#include <_date/clip_BOQ.c>
#include <_date/clip_BOY.c>
#include <_date/clip_DOY.c>
#include <_date/clip_EOM.c>
#include <_date/clip_EOQ.c>
#include <_date/clip_EOY.c>
#include <_date/clip_ISLEAP.c>
#include <_date/clip_LASTDAYOM.c>
#include <_date/clip_QUARTER.c>
#include <_date/clip_SECTOTIME.c>
#include <_date/clip_TIMETOSEC.c>
#include <_date/clip_VALIDTIME.c>
#include <_date/clip_TIMEVALID.c>
#include <_date/clip_WEEK.c>
#include <_date/clip_WOM.c>
/****************************************/
/* date & time class support */
#include <_date/typedef_struct_ClipDateTime.h>
#include <_date/_clip_dt_normalize.c>
#include <_date/clip_DT_CREATE.c>
#include <_date/clip_DT_NORMALIZE.c>
#include <_date/clip_DT_CMP.c>
#include <_date/clip_DT_DATE.c>
#include <_date/clip_DT_TIME.c>
#include <_date/_clip_dt_info.c>
#include <_date/clip_DT_ISDTDATA.c>
#include <_date/clip_DT_YEAR.c>
#include <_date/clip_DT_MONTH.c>
#include <_date/clip_DT_DAY.c>
#include <_date/clip_DT_HOUR.c>
#include <_date/clip_DT_MIN.c>
#include <_date/clip_DT_SEC.c>
#include <_date/clip_DT_MSEC.c>
#include <_date/clip_DT_ADD.c>
#include <_date/clip_DT_SUB.c>
#include <_date/clip_DTOJ.c>
#include <_date/clip_JTOD.c>

/* Visual FoxPro DateTime */

#include <_date/_clip_str_to_time.c>
#include <_date/_clip_ctot.c>
#include <_date/clip_CTOT.c>
#include <_date/clip_STOT.c>
#include <_date/_clip_ttoc.c>
#include <_date/clip_TTOC.c>
#include <_date/clip_TTOS.c>
