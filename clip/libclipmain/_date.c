/*
	Copyright (C) 2001  ITK
	Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/

#include <string.h>
#include "ci_clip.h"

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

#include "ci_error.ch"
#include "ci_hashcode.h"

#ifdef OS_CYGWIN
#include <w32api/windows.h>
#endif

int
clip_STOD(ClipMachine * ClipMachineMemory)
{
   long      d, m, y;

   char     *str = _clip_parc(ClipMachineMemory, 1);

   if (str == NULL)
    {
       _clip_retdj(ClipMachineMemory, 0);
       return 0;
      /*
         return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STOD");
       */
    }
   if (str[0] == ' ' || str[0] == '\t')
    {
       _clip_retdj(ClipMachineMemory, 0);
       return 0;
    }
   y = atol(str) / 10000;
   m = atol(str + 4) / 100;
   d = atol(str + 6);
   _clip_retdj(ClipMachineMemory, _clip_jdate(d, m, y));
   return 0;
}

int
clip_CTOD(ClipMachine * ClipMachineMemory)
{
   char     *str = _clip_parc(ClipMachineMemory, 1);

   char     *format = _clip_parc(ClipMachineMemory, 2);

   if (str == NULL)
    {
       _clip_retdj(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CTOD");
    }
   if (format == NULL)
      format = ClipMachineMemory->date_format;
   _clip_retdj(ClipMachineMemory, _clip_str_to_date(str, format, ClipMachineMemory->epoch));
   return 0;
}

struct tm *
_clip_sysdate()
{
  //#if !defined( __ZTC__ ) || !defined(__MSDOS__)
   struct tm *sysTime;

   time_t    t = time(0);

   sysTime = malloc(sizeof(struct tm));
   memcpy(sysTime, localtime(&t), sizeof(struct tm));

   return sysTime;
  /*
     #else
     time_t t=time(0);
     struct tm sysTime;
     REGS regs;
     regs.h.ah=0x2a;
     intdos(&regs, &regs);
     sysTime->tm_year=regs.x.cx-1900;
     sysTime->tm_wday=regs.h.al;
     sysTime->tm_mon=regs.h.dh-1;
     sysTime->tm_mday=regs.h.dl;
     regs.h.ah=0x2c;
     intdos(&regs, &regs);
     sysTime->tm_hour=regs.h.ch;
     sysTime->tm_min=regs.h.cl;
     sysTime->tm_sec=regs.h.dh;
     sysTime->tm_yday=0;
     sysTime->tm_isdst=-1;
     mktime(sysTime);
     return &sysTime;
     #endif
   */
}

int
clip_DATE(ClipMachine * ClipMachineMemory)
{
   int       yy, mm, dd;

#ifdef _WIN32
   SYSTEMTIME st;

   GetLocalTime(&st);
   yy = st.wYear;
   mm = st.wMonth;
   dd = st.wDay;
#else
   struct tm *sysTime;

   sysTime = _clip_sysdate();
   yy = sysTime->tm_year + 1900;
   mm = sysTime->tm_mon + 1;
   dd = sysTime->tm_mday;
   free(sysTime);
#endif

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      yy = _clip_parni(ClipMachineMemory, 1);
   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      mm = _clip_parni(ClipMachineMemory, 2);
   if (_clip_parinfo(ClipMachineMemory, 3) == NUMERIC_type_of_ClipVarType)
      dd = _clip_parni(ClipMachineMemory, 3);

   _clip_retdc(ClipMachineMemory, yy, mm, dd);
   return 0;
}

int
clip_DTOC(ClipMachine * ClipMachineMemory)
{
   long      d = _clip_pardj(ClipMachineMemory, 1);

   int       flag1 = 0, flag2 = 0, flag3 = 0;

   int       fox = 0, i, len;

   int       pcount = _clip_parinfo(ClipMachineMemory, 0);

   char     *format = _clip_parcl(ClipMachineMemory, 2, &len);

   char     *dtoc = NULL, ch;

   if (pcount == 1)
      format = ClipMachineMemory->date_format;
   else
    {
       if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
	{
	   fox = 1;
	   for (i = 0; i < len; i++)
	    {
	       ch = format[i];
	       switch (ch)
		{
		case 'Y':
		case 'y':
		   flag1++;
		   break;
		case 'M':
		case 'm':
		   flag2++;
		   break;
		case 'D':
		case 'd':
		   flag3++;
		   break;
		}
	    }
	   if (flag1 && flag2 && flag3)
	      fox = 0;
	}
       else
	  fox = 1;

    }

   if (fox)
      dtoc = _clip_date_to_str(d, "YYYYMMDD");
   else
      dtoc = _clip_date_to_str(d, format);

   _clip_retcn_m(ClipMachineMemory, dtoc, strlen(dtoc));
   return 0;
}

int
clip_DAY(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   if (_clip_pardj(ClipMachineMemory, 1) == 0)
    {
       _clip_retndp(ClipMachineMemory, 0, 3, 0);
       return 0;
    }

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   _clip_retndp(ClipMachineMemory, dd, 3, 0);
   return 0;
}

int
clip_DOW(ClipMachine * ClipMachineMemory)
{
   int       week;

   long      d = _clip_pardj(ClipMachineMemory, 1);

   char     *buf = _clip_fetch_item(ClipMachineMemory, HASH_first_day_week);

   week = (d + (*buf)) % 7 + 1;
   if (d == 0)
      _clip_retndp(ClipMachineMemory, 0, 1, 0);
   else
      _clip_retndp(ClipMachineMemory, week, 1, 0);
   return 0;
}

int
clip_DTOS(ClipMachine * ClipMachineMemory)
{
   char     *ret;

   long      d = _clip_pardj(ClipMachineMemory, 1);

   ret = malloc(9);
   _clip_datetostr(d, ret);
   _clip_retcn_m(ClipMachineMemory, ret, 8);
   return 0;
}

int
clip_MONTH(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   if (_clip_pardj(ClipMachineMemory, 1) == 0)
    {
       _clip_retndp(ClipMachineMemory, 0, 3, 0);
       return 0;
    }

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   _clip_retndp(ClipMachineMemory, mm, 3, 0);
   return 0;
}

static double
_clip_seconds()
{
   double    ret;

#if !defined( __ZTC__ ) || !defined(__MSDOS__)
   struct timeval tv;

   struct timezone tz;

   gettimeofday(&tv, &tz);
#ifdef _WIN32
   ret = (tv.tv_sec) % 86400 + (double) tv.tv_usec / 1000000;
#else
   ret = (tv.tv_sec - tz.tz_minuteswest * 60) % 86400 + (double) tv.tv_usec / 1000000;
#endif
#else
   time_t    t = time(0);

   struct tm sysTime;

   struct timeval tv;

   {
      REGS      regs;

      regs.h.ah = 0x2a;
      intdos(&regs, &regs);
      sysTime.tm_year = regs.x.cx - 1900;
      sysTime.tm_wday = regs.h.al;
      sysTime.tm_mon = regs.h.dh - 1;
      sysTime.tm_mday = regs.h.dl;
      regs.h.ah = 0x2c;
      intdos(&regs, &regs);
      sysTime.tm_hour = regs.h.ch;
      sysTime.tm_min = regs.h.cl;
      sysTime.tm_sec = regs.h.dh;
      sysTime.tm_yday = 0;
      sysTime.tm_isdst = -1;
      mktime(&sysTime);
   }
   gettimeofday(&tv, 0);
   ret = (sysTime.tm_hour * 3600 + sysTime.tm_min * 60 + sysTime.tm_sec) * 1000;
   ret = (ret + tv.tv_usec / 1000) / 1000;
#endif
   return ret;
}

int
clip_SECONDS(ClipMachine * ClipMachineMemory)
{
   _clip_retndp(ClipMachineMemory, _clip_seconds(), 10, 2);
   return 0;
}

int
clip_SECOND(ClipMachine * ClipMachineMemory)
{
   _clip_retndp(ClipMachineMemory, _clip_seconds(), 10, 2);
   return 0;
}

static int
_clip_timezone()
{
   int       ret;

#if !defined( __ZTC__ ) || !defined(__MSDOS__)
   struct timeval tv;

   struct timezone tz;

   gettimeofday(&tv, &tz);
   ret = tz.tz_minuteswest;
#else
   ret = 0;
#endif
   return ret;
}

int
clip_TIMEZONE(ClipMachine * ClipMachineMemory)
{
   _clip_retndp(ClipMachineMemory, _clip_timezone(), 10, 0);
   return 0;
}

int
clip_SECONDSCPU(ClipMachine * ClipMachineMemory)
{
   int       n = _clip_parni(ClipMachineMemory, 1);

   double    d = 0;

   struct tms tm;

   times(&tm);

   if ((n < 1 || n > 3) && (n < 11 || n > 13))
      n = 3;

   if (n > 10)
    {
       n -= 10;
       if (n & 1)
	  d += tm.tms_cutime;
       if (n & 2)
	  d += tm.tms_cstime;
    }
   if (n & 1)
      d += tm.tms_utime;
   if (n & 2)
      d += tm.tms_stime;

   d /= sysconf(_SC_CLK_TCK);
   _clip_retndp(ClipMachineMemory, d, 10, 2);
   return 0;
}

int
clip_TIME(ClipMachine * ClipMachineMemory)
{
   char     *ret = (char *) malloc(9);

#ifdef _WIN32
   SYSTEMTIME st;

   GetLocalTime(&st);
   snprintf(ret, 9, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
#else
   time_t    t = time(0);

#if 0
   struct tm sysTime;

   sysTime = *(localtime(&t));
   snprintf(ret, 9, "%02d:%02d:%02d", sysTime.tm_hour, sysTime.tm_min, sysTime.tm_sec);
#else
   struct tm *sysTime;

   sysTime = localtime(&t);
   snprintf(ret, 9, "%02d:%02d:%02d", sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);
#endif
#endif
   _clip_retcn_m(ClipMachineMemory, ret, 8);

   return 0;
}

int
clip_YEAR(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   if (_clip_pardj(ClipMachineMemory, 1) == 0)
    {
       _clip_retndp(ClipMachineMemory, 0, 5, 0);
       return 0;
    }

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   _clip_retndp(ClipMachineMemory, yy, 5, 0);
   return 0;
}

int
clip_ADDMONTH(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww, old_mm, i;

   long      jdate;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   int       nummon = _clip_parni(ClipMachineMemory, 1);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       _clip_retdj(ClipMachineMemory, 0);
       return 0;
    }
   if (numpar == 1)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
       nummon = 1;
    }
   if (numpar > 1)
      nummon = _clip_parni(ClipMachineMemory, 2);
   old_mm = mm + yy * 12;
   mm += nummon;

   if (mm > 12)
    {
       yy += (int) mm / 12;
       mm = mm - ((int) mm / 12) * 12;
    }

   if (nummon != 0)
    {
       for (i = 4; i != 0; i--)
	{
	   jdate = _clip_jdate(dd, mm, yy);
	   _clip_cdate(jdate, &dd, &mm, &yy, &ww);
	   if (((mm + yy * 12) - old_mm) == nummon)
	      break;
	   dd--;
	}
    }
   _clip_retdc(ClipMachineMemory, yy, mm, dd);
   return 0;
}

int
clip_ADDYEAR(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   int       numyear = _clip_parni(ClipMachineMemory, 1);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       _clip_retdj(ClipMachineMemory, 0);
       return 0;
    }
   if (numpar == 1)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1901;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   if (numpar > 1)
      numyear = _clip_parni(ClipMachineMemory, 2);
   yy += numyear;
   _clip_retdc(ClipMachineMemory, yy, mm, dd);
   return 0;
}

int
clip_BOM(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   _clip_retdc(ClipMachineMemory, yy, mm, 1);
   return 0;
}

int
clip_BOQ(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   _clip_retdc(ClipMachineMemory, yy, (mm / 4) * 3 + 1, 1);
   return 0;
}

int
clip_BOY(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   _clip_retdc(ClipMachineMemory, yy, 1, 1);
   return 0;
}

int
clip_DOY(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   struct tm *sysTime;

   long      d1, d2;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   d1 = _clip_jdate(1, 1, yy);
   d2 = _clip_jdate(dd, mm, yy);
   _clip_retndp(ClipMachineMemory, d2 - d1, 3, 0);
   return 0;
}

int
clip_EOM(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww, mm1;

   struct tm *sysTime;

   long      d;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   d = _clip_jdate(1, mm + 1, yy);
   _clip_cdate(d, &dd, &mm1, &yy, &ww);
   while (mm != mm1)
    {
       d--;
       _clip_cdate(d, &dd, &mm1, &yy, &ww);
    }
   _clip_retdc(ClipMachineMemory, yy, mm, dd);
   return 0;
}

int
clip_EOQ(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww, mm1;

   long      d;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   mm = (mm / 4 + 1) * 3;
   d = _clip_jdate(1, mm + 1, yy);
   _clip_cdate(d, &dd, &mm1, &yy, &ww);
   while (mm != mm1)
    {
       d--;
       _clip_cdate(d, &dd, &mm1, &yy, &ww);
    }
   _clip_retdc(ClipMachineMemory, yy, mm, dd);
   return 0;
}

int
clip_EOY(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   _clip_retdc(ClipMachineMemory, yy, 12, 31);
   return 0;
}

int
clip_ISLEAP(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   long      d1, d2;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   d1 = _clip_jdate(1, 1, yy);
   d2 = _clip_jdate(31, 12, yy);
   _clip_retl(ClipMachineMemory, (d2 - d1) == 365);
   return 0;
}

int
clip_LASTDAYOM(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww, mm1;

   struct tm *sysTime;

   long      d;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   mm1 = _clip_parni(ClipMachineMemory, 1);
   if (numpar == 0 || mm1 != 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   if (mm1 != 0)
      mm = mm1;
   d = _clip_jdate(1, mm + 1, yy);
   _clip_cdate(d, &dd, &mm1, &yy, &ww);
   while (mm != mm1)
    {
       d--;
       _clip_cdate(d, &dd, &mm1, &yy, &ww);
    }
   _clip_retndp(ClipMachineMemory, dd, 2, 0);
   return 0;
}

int
clip_QUARTER(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   _clip_retndp(ClipMachineMemory, mm / 4 + 1, 2, 0);
   return 0;
}

int
clip_SECTOTIME(ClipMachine * ClipMachineMemory)
{
   int       hour, min, sec, msec, len;

   char     *ret;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   int       flag = _clip_parl(ClipMachineMemory, 2);

   len = 32;			/*flag ? 11 : 8; */
   ret = malloc(len + 1);
   sec = d;
   msec = (d - sec) * 100;
   min = sec / 60;
   hour = min / 60;
   sec = sec % 60;
   min = min % 60;
   if (flag)
      snprintf(ret, len, "%02d:%02d:%02d:%02d", hour, min, sec, msec);
   else
      snprintf(ret, len, "%02d:%02d:%02d", hour, min, sec);
   len = strlen(ret);
   _clip_retcn_m(ClipMachineMemory, ret, len);
   return 0;
}

int
clip_TIMETOSEC(ClipMachine * ClipMachineMemory)
{
   int       l, hour = 0, min = 0, sec = 0, msec = 0;

   double    ret;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL)
    {
       _clip_retndp(ClipMachineMemory, _clip_seconds(), 10, 2);
       return 0;
    }
   sscanf(str, "%02d:%02d:%02d:%02d", &hour, &min, &sec, &msec);
   ret = hour * 3600 + min * 60 + sec;
   ret = (ret * 100 + msec) / 100;
   _clip_retndp(ClipMachineMemory, ret, 10, 2);
   return 0;
}

int
clip_VALIDTIME(ClipMachine * ClipMachineMemory)
{
   int       l, hour = 0, min = 0, sec = 0, msec = 0, ret = 1;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL)
    {
       _clip_retl(ClipMachineMemory, 0);
       return 0;
    }
   sscanf(str, "%02d:%02d:%02d:%02d", &hour, &min, &sec, &msec);
   if (hour < 0 || hour > 23)
      ret = 0;
   if (min < 0 || min > 59)
      ret = 0;
   if (sec < 0 || sec > 59)
      ret = 0;
   if (msec < 0 || msec > 99)
      ret = 0;
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}

int
clip_TIMEVALID(ClipMachine * ClipMachineMemory)
{
   int       l;

   int       hour = 0, min = 0, sec = 0, msec = 0, ret = 1;
   int       counts[] = { 0, 0, 0, 0 };
   int       count = 0;

   char     *s, *e, *str = _clip_parcl(ClipMachineMemory, 1, &l);

   _clip_retl(ClipMachineMemory, 0);
   if (str == NULL)
      return 0;

   for (s = str, e = str + l; s <= e; s++)
    {
       if (*s == ':' || *s == 0 || (*s >= '0' && *s <= '9'))
	  counts[count]++;
       if (*s == ':' || *s == 0)
	{
	  //             printf("\ns=%s,count=%d,c1=%d,c2=%d,c3=%d,c4=%d\n",str,count,counts[0],counts[1],counts[2],counts[3]);
	   if (counts[count] != 0 && counts[count] != 3)
	    {
	       ret = 0;
	       break;
	    }
	   if (*s != 0)
	      count++;
	}
       if (count > 3)
	{
	   ret = 0;
	   break;
	}

    }
   if (ret != 0)
    {
       sscanf(str, "%02d:%02d:%02d:%02d", &hour, &min, &sec, &msec);
       if (hour < 0 || hour > 23)
	  ret = 0;
       if (min < 0 || min > 59)
	  ret = 0;
       if (sec < 0 || sec > 59)
	  ret = 0;
       if (msec < 0 || msec > 99)
	  ret = 0;
    }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}

int
clip_WEEK(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww, week1, week2, ret;

   long      d1, d2;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   int       weekday = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_first_day_week));

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   d1 = _clip_jdate(1, 1, yy);
   week1 = (d1 + weekday) % 7 + 1;
   d2 = _clip_jdate(dd, mm, yy);
   week2 = (d2 + weekday) % 7 + 1;
   ret = (week2 < week1);
   ret = ((d2 - d1) / 7) + ret + 1;
   _clip_retndp(ClipMachineMemory, ret, 2, 0);
   return 0;
}

int
clip_WOM(ClipMachine * ClipMachineMemory)
{
   int       dd, mm, yy, ww, week1, week2, ret;

   long      d1, d2;

   struct tm *sysTime;

   int       numpar = _clip_parinfo(ClipMachineMemory, 0);

   int       weekday = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_first_day_week));

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   if (numpar == 0)
    {
       sysTime = _clip_sysdate();
       yy = sysTime->tm_year + 1900;
       mm = sysTime->tm_mon + 1;
       dd = sysTime->tm_mday;
       free(sysTime);
    }
   d1 = _clip_jdate(1, mm, yy);
   week1 = (d1 + weekday) % 7 + 1;
   d2 = _clip_jdate(1, mm + 1, yy) - 1;
   week2 = (d2 + weekday) % 7 + 1;
   ret = (week2 < week1);
   ret = ((d2 - d1) / 7) + ret + 1;
   _clip_retndp(ClipMachineMemory, ret, 2, 0);
   return 0;
}

/****************************************/
/* date & time class support */
typedef struct
{
   int       sign;		/* check data of _C_ITEM_TYPE_DATETIME */
   int       year;
   int       month;
   int       day;
   int       hour;
   int       min;
   int       sec;
   int       msec;
   char      null;
}
ClipDateTime;

int
_clip_dt_normalize(ClipDateTime * dt)
{
   int       ii, jj, ww;

   long      d;

   ii = dt->msec % 1000;
   jj = dt->msec / 1000;
   if (ii < 0)
    {
       jj--;
       ii = 1000 + ii;
    }
   dt->msec = ii;
   dt->sec += jj;

   ii = dt->sec % 60;
   jj = dt->sec / 60;
   if (ii < 0)
    {
       jj--;
       ii = 60 + ii;
    }
   dt->sec = ii;
   dt->min += jj;

   ii = dt->min % 60;
   jj = dt->min / 60;
   if (ii < 0)
    {
       jj--;
       ii = 60 + ii;
    }
   dt->min = ii;
   dt->hour += jj;

   ii = dt->hour % 24;
   jj = dt->hour / 24;
   if (ii < 0)
    {
       jj--;
       ii = 24 + ii;
    }
   dt->hour = ii;
   dt->day += jj;

   d = _clip_jdate(dt->day, dt->month, dt->year);
   _clip_cdate(d, &(dt->day), &(dt->month), &(dt->year), &ww);
   return 0;
}

int
clip_DT_CREATE(ClipMachine * ClipMachineMemory)
{
   int       ww;

   ClipDateTime *dt;

   dt = malloc(sizeof(ClipDateTime));
   memset(dt, 0, sizeof(ClipDateTime));
   dt->sign = _C_ITEM_TYPE_DATETIME;

   if (_clip_parinfo(ClipMachineMemory, 1) == DATE_type_of_ClipVarType)
      _clip_pardc(ClipMachineMemory, 1, &(dt->year), &(dt->month), &(dt->day), &ww);

   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
    {
       char     *tmp = _clip_parc(ClipMachineMemory, 1);

       dt->year = atol(tmp);
       for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
       if (*tmp != 0)
	  dt->month = atol(tmp + 1);
       for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
       if (*tmp != 0)
	  dt->day = atol(tmp + 1);
    }
   dt->hour = _clip_parni(ClipMachineMemory, 2);
   dt->min = _clip_parni(ClipMachineMemory, 3);
   dt->sec = _clip_parni(ClipMachineMemory, 4);
   dt->msec = _clip_parni(ClipMachineMemory, 5);

   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
    {
       char     *tmp = _clip_parc(ClipMachineMemory, 2);

       dt->hour = atol(tmp);
       for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
       if (*tmp != 0)
	  dt->min = atol(tmp + 1);
       for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
       if (*tmp != 0)
	  dt->sec = atol(tmp + 1);
       for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
       if (*tmp != 0)
	  dt->msec = atol(tmp + 1);
    }

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
    {
       dt->year = _clip_parni(ClipMachineMemory, 1);
       dt->month = _clip_parni(ClipMachineMemory, 2);
       dt->day = _clip_parni(ClipMachineMemory, 3);
       dt->hour = _clip_parni(ClipMachineMemory, 4);
       dt->min = _clip_parni(ClipMachineMemory, 5);
       dt->sec = _clip_parni(ClipMachineMemory, 6);
       dt->msec = _clip_parni(ClipMachineMemory, 7);
    }
  /*
     _clip_dt_normalize(dt);
   */
   _clip_retcn_m(ClipMachineMemory, (char *) dt, sizeof(ClipDateTime) - 1);
   return 0;
}

int
clip_DT_NORMALIZE(ClipMachine * ClipMachineMemory)
{
   int       len;

   ClipDateTime *dt;

   ClipDateTime *dt2;

   char     *tmp = _clip_parcl(ClipMachineMemory, 1, &len);

   if (tmp == NULL || len != sizeof(ClipDateTime) - 1 || *tmp != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_NORMALIZE");

   dt = (ClipDateTime *) tmp;

   _clip_dt_normalize(dt);

   dt2 = malloc(sizeof(ClipDateTime));
   memcpy(dt2, dt, sizeof(ClipDateTime));

   _clip_retcn_m(ClipMachineMemory, (char *) dt2, sizeof(ClipDateTime) - 1);
   return 0;
}

int
clip_DT_CMP(ClipMachine * ClipMachineMemory)
{
   int       ret = 0, len1, len2;

   char     *_dt1, *_dt2;

   ClipDateTime *dt1, *dt2;

   _dt1 = _clip_parcl(ClipMachineMemory, 1, &len1);
   if (_dt1 == NULL || len1 != sizeof(ClipDateTime) - 1 || *_dt1 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_CMP");
   _dt2 = _clip_parcl(ClipMachineMemory, 2, &len2);
   if (_dt2 == NULL || len2 != sizeof(ClipDateTime) - 1 || *_dt2 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_CMP");

   dt1 = (ClipDateTime *) _dt1;
   dt2 = (ClipDateTime *) _dt2;
   _clip_dt_normalize(dt1);
   _clip_dt_normalize(dt2);

   if (ret == 0)
      ret = dt1->year - dt2->year;
   if (ret == 0)
      ret = dt1->month - dt2->month;
   if (ret == 0)
      ret = dt1->day - dt2->day;
   if (ret == 0)
      ret = dt1->hour - dt2->hour;
   if (ret == 0)
      ret = dt1->min - dt2->min;
   if (ret == 0)
      ret = dt1->sec - dt2->sec;
   if (ret == 0)
      ret = dt1->msec - dt2->msec;

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}

int
clip_DT_DATE(ClipMachine * ClipMachineMemory)
{
   int       len;

   ClipDateTime *dt, *dt2;

   char     *tmp = _clip_parcl(ClipMachineMemory, 1, &len);

   if (tmp == NULL || len != sizeof(ClipDateTime) - 1 || *tmp != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_DATE");

   dt = (ClipDateTime *) tmp;
   dt2 = malloc(sizeof(ClipDateTime));
   memcpy(dt2, dt, sizeof(ClipDateTime));
   _clip_dt_normalize(dt2);
   _clip_retdc(ClipMachineMemory, dt2->year, dt2->month, dt2->day);
   free(dt2);
   return 0;
}

int
clip_DT_TIME(ClipMachine * ClipMachineMemory)
{
   int       len;

   ClipDateTime *dt, *dt2;

   char      buf[32];

   char     *tmp = _clip_parcl(ClipMachineMemory, 1, &len);

   if (tmp == NULL || len != sizeof(ClipDateTime) - 1 || *tmp != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_TIME");

   dt = (ClipDateTime *) tmp;
   dt2 = malloc(sizeof(ClipDateTime));
   memcpy(dt2, dt, sizeof(ClipDateTime));
   _clip_dt_normalize(dt2);
   snprintf(buf, 31, "%02d:%02d:%02d:%02d", dt2->hour, dt2->min, dt2->sec, dt2->msec);
   free(dt2);
   _clip_retc(ClipMachineMemory, buf);
   return 0;
}

int
_clip_dt_info(ClipMachine * ClipMachineMemory, int info)
{
   int       len, is_dt = 1;

   ClipDateTime *dt;

   char     *tmp = _clip_parcl(ClipMachineMemory, 1, &len);

   if (tmp == NULL || len != sizeof(ClipDateTime) - 1 || *tmp != _C_ITEM_TYPE_DATETIME)
      is_dt = 0;

   if (info == 0)
    {
       _clip_retl(ClipMachineMemory, is_dt);
       return 0;
    }
   if (!is_dt)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_INFO");

   dt = (ClipDateTime *) tmp;
   switch (info)
    {
    case 1:
       _clip_retni(ClipMachineMemory, dt->year);
       break;
    case 2:
       _clip_retni(ClipMachineMemory, dt->month);
       break;
    case 3:
       _clip_retni(ClipMachineMemory, dt->day);
       break;
    case 4:
       _clip_retni(ClipMachineMemory, dt->hour);
       break;
    case 5:
       _clip_retni(ClipMachineMemory, dt->min);
       break;
    case 6:
       _clip_retni(ClipMachineMemory, dt->sec);
       break;
    case 7:
       _clip_retni(ClipMachineMemory, dt->msec);
       break;
    default:
       _clip_retni(ClipMachineMemory, 0);
       break;
    }
   return 0;
}

int
clip_DT_ISDTDATA(ClipMachine * ClipMachineMemory)
{
   _clip_dt_info(ClipMachineMemory, 0);
   return 0;
}

int
clip_DT_YEAR(ClipMachine * ClipMachineMemory)
{
   _clip_dt_info(ClipMachineMemory, 1);
   return 0;
}

int
clip_DT_MONTH(ClipMachine * ClipMachineMemory)
{
   _clip_dt_info(ClipMachineMemory, 2);
   return 0;
}

int
clip_DT_DAY(ClipMachine * ClipMachineMemory)
{
   _clip_dt_info(ClipMachineMemory, 3);
   return 0;
}

int
clip_DT_HOUR(ClipMachine * ClipMachineMemory)
{
   _clip_dt_info(ClipMachineMemory, 4);
   return 0;
}

int
clip_DT_MIN(ClipMachine * ClipMachineMemory)
{
   _clip_dt_info(ClipMachineMemory, 5);
   return 0;
}

int
clip_DT_SEC(ClipMachine * ClipMachineMemory)
{
   _clip_dt_info(ClipMachineMemory, 6);
   return 0;
}

int
clip_DT_MSEC(ClipMachine * ClipMachineMemory)
{
   _clip_dt_info(ClipMachineMemory, 7);
   return 0;
}

int
clip_DT_ADD(ClipMachine * ClipMachineMemory)
{
   int       len1, len2;

   ClipDateTime *dt1, *dt2, *dt;

   char     *tmp1 = _clip_parcl(ClipMachineMemory, 1, &len1);

   char     *tmp2 = _clip_parcl(ClipMachineMemory, 2, &len2);

   if (tmp1 == NULL || len1 != sizeof(ClipDateTime) - 1 || *tmp1 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_ADD");
   if (tmp2 == NULL || len2 != sizeof(ClipDateTime) - 1 || *tmp2 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_ADD");

   dt1 = (ClipDateTime *) tmp1;
   dt2 = (ClipDateTime *) tmp2;

   dt = malloc(sizeof(ClipDateTime));
   dt->sign = _C_ITEM_TYPE_DATETIME;
   dt->year = dt1->year + dt2->year;
   dt->month = dt1->month + dt2->month;
   dt->day = dt1->day + dt2->day;
   dt->hour = dt1->hour + dt2->hour;
   dt->min = dt1->min + dt2->min;
   dt->sec = dt1->sec + dt2->sec;
   dt->msec = dt1->msec + dt2->msec;

  /*
     _clip_dt_normalize(dt);
   */

   _clip_retcn_m(ClipMachineMemory, (char *) dt, sizeof(ClipDateTime) - 1);
   return 0;
}

int
clip_DT_SUB(ClipMachine * ClipMachineMemory)
{
   int       len1, len2;

   ClipDateTime *dt1, *dt2, *dt;

   char     *tmp1 = _clip_parcl(ClipMachineMemory, 1, &len1);

   char     *tmp2 = _clip_parcl(ClipMachineMemory, 2, &len2);

   if (tmp1 == NULL || len1 != sizeof(ClipDateTime) - 1 || *tmp1 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_SUB");
   if (tmp2 == NULL || len2 != sizeof(ClipDateTime) - 1 || *tmp2 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_SUB");

   dt1 = (ClipDateTime *) tmp1;
   dt2 = (ClipDateTime *) tmp2;

   dt = malloc(sizeof(ClipDateTime));
   dt->sign = _C_ITEM_TYPE_DATETIME;
   dt->year = dt1->year - dt2->year;
   dt->month = dt1->month - dt2->month;
   dt->day = dt1->day - dt2->day;
   dt->hour = dt1->hour - dt2->hour;
   dt->min = dt1->min - dt2->min;
   dt->sec = dt1->sec - dt2->sec;
   dt->msec = dt1->msec - dt2->msec;
  /*
     _clip_dt_normalize(dt);
   */

   _clip_retcn_m(ClipMachineMemory, (char *) dt, sizeof(ClipDateTime) - 1);
   return 0;
}

int
clip_DTOJ(ClipMachine * ClipMachineMemory)
{
   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
    {
       struct tm *sysTime;

       sysTime = _clip_sysdate();
       _clip_retndp(ClipMachineMemory, _clip_jdate(sysTime->tm_mday, sysTime->tm_mon + 1, sysTime->tm_year + 1900), 7, 0);
       free(sysTime);
       return 0;
    }

   _clip_retndp(ClipMachineMemory, _clip_pardj(ClipMachineMemory, 1), 7, 0);
   return 0;
}

int
clip_JTOD(ClipMachine * ClipMachineMemory)
{
   _clip_retdj(ClipMachineMemory, _clip_parnl(ClipMachineMemory, 1));
   return 0;
}

/* Visual FoxPro DateTime */

long
_clip_str_to_time(char *str)
{
   int       len, am = 0, pm = 0;

   char     *t = str, *e, *ms = 0, *ss = 0, *ds = 0;

   long      r;

   int       h = 0, m = 0, s = 0, d = 0;

   if (!str)
      return 0;

   while (*t == ' ')
      t++;
   ms = strchr(t, ':');
   if (ms)
    {
       ss = strchr(ms + 1, ':');
       if (ss)
	  ds = strchr(ss + 1, ':');
    }
   len = strlen(t);
   e = t + len - 1;
   while (*e == ' ')
    {
       e--;
       len--;
    }

   h = atoi(t);
   if (ms)
      m = atoi(ms + 1);
   if (ss)
      s = atoi(ss + 1);
   if (ds)
      d = atoi(ds + 1);

   if (toupper(*e) == 'M')
    {
       if (toupper(*(e - 1)) == 'A')
	  am = 1;
       else if (toupper(*(e - 1)) == 'P')
	  pm = 1;
       else
	  return -1;
    }
   if ((am || pm) && (h > 12))
      return -1;

   if (h == 12 && (am || pm))
      h = 0;
   if (pm)
      h += 12;
   if (h == 24)
      h = 0;

   if (h < 0 || h > 23)
      return -1;
   if (m < 0 || m > 59)
      return -1;
   if (s < 0 || s > 59)
      return -1;
   if (d < 0 || d > 99)
      return -1;

   r = h * 60 * 60 * 1000 + m * 60 * 1000 + s * 1000 + d * 10;
   return r;
}

int
_clip_ctot(ClipMachine * ClipMachineMemory, char *ctime, long *date, long *time, char *dateformat)
{
   char     *s = ctime;

   *date = 0;
   *time = 0;

   if (!ctime)
      return 1;

   while (*s == ' ')
      s++;
//      if(strlen(s) < 5) /* 11:12 */
//              return 1;

   if (s[2] == ':' || strlen(s) < 5)
    {
       *date = 0 /*2415019 */ ;
       *time = _clip_str_to_time(s);
       if (*time == -1)
	  return 1;
    }
   else
    {
       char     *t = strchr(s, ' ');

       *date = _clip_str_to_date(s, dateformat, ClipMachineMemory->epoch);
       if (*date == 0)
	  return 1;
       *time = _clip_str_to_time(t);
       if (*time == -1)
	  return 1;
    }
   return 0;
}

int
clip_CTOT(ClipMachine * ClipMachineMemory)
{
   char     *ctime = _clip_parc(ClipMachineMemory, 1);

   long      date, time;

   int       er;

   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CTOT");

   er = _clip_ctot(ClipMachineMemory, ctime, &date, &time, ClipMachineMemory->date_format);
   if (er)
    {
       _clip_retdtj(ClipMachineMemory, 0, 0);
       return 0;
    }
   _clip_retdtj(ClipMachineMemory, date, time);
   return er;
}

int
clip_STOT(ClipMachine * ClipMachineMemory)
{
   char     *ctime = _clip_parc(ClipMachineMemory, 1);

   long      date, time;

   int       er;

   er = _clip_ctot(ClipMachineMemory, ctime, &date, &time, "yyyy-mm-dd");
   if (er)
    {
       _clip_retdtj(ClipMachineMemory, 0, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CTOT");
    }
   _clip_retdtj(ClipMachineMemory, date, time);
   return er;
}

char     *
_clip_ttoc(ClipMachine * ClipMachineMemory, long julian, long time, int *retlen, char *date_format, int hours, int seconds)
{
   char     *r = _clip_date_to_str(julian, date_format);

   int       len = strlen(r);

   int       h, m, s;

   long      t;

   int       pm = 0;

   r = realloc(r, len + 13);
   r[len] = ' ';
   h = time / (60 * 60 * 1000);
   t = time % (60 * 60 * 1000);
   m = t / (60 * 1000);
   t = t % (60 * 1000);
   s = t / 1000;
   if (!hours)
    {
       if (h >= 12)
	  pm = 1;
       if (h > 12)
	  h -= 12;
    }

   if (seconds)
    {
       if (h == 0 && m == 0 && s == 0)
	  snprintf(r + len + 1, 11, "  :  :  AM");
       else if (hours)
	  snprintf(r + len + 1, 9, "%02d:%02d:%02d", h, m, s);
       else if (pm)
	  snprintf(r + len + 1, 11, "%02d:%02d:%02dPM", h, m, s);
       else
	  snprintf(r + len + 1, 11, "%02d:%02d:%02dPM", h, m, s);
    }
   else
    {
       if (h == 0 && m == 0)
	  snprintf(r + len + 1, 8, "  :  AM");
       else if (hours)
	  snprintf(r + len + 1, 6, "%02d:%02d", h, m);
       else if (pm)
	  snprintf(r + len + 1, 8, "%02d:%02dPM", h, m);
       else
	  snprintf(r + len + 1, 8, "%02d:%02dAM", h, m);
    }
   if (retlen)
      *retlen = len + 6 + (seconds ? 3 : 0) + (hours ? 0 : 2);
   return r;
}

int
clip_TTOC(ClipMachine * ClipMachineMemory)
{
   ClipVar  *r = RETPTR(ClipMachineMemory);

   char     *date_format = _clip_parc(ClipMachineMemory, 2);

   long      time = 0;

   long      julian;

   if (_clip_parinfo(ClipMachineMemory, 1) == DATE_type_of_ClipVarType)
      julian = _clip_pardj(ClipMachineMemory, 1);
   else if (_clip_parinfo(ClipMachineMemory, 1) == DATETIME_type_of_ClipVarType)
      julian = _clip_pardtj(ClipMachineMemory, 1, &time);
   else
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "TTOC");

   if (date_format == NULL || *date_format == 0)
      date_format = ClipMachineMemory->date_format;

   memset(r, 0, sizeof(ClipVar));
   r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
    _clip_ttoc(ClipMachineMemory, julian, time, &r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf,
	       date_format, ClipMachineMemory->hours, ClipMachineMemory->seconds);
   return 0;
}

int
clip_TTOS(ClipMachine * ClipMachineMemory)
{
   ClipVar  *r = RETPTR(ClipMachineMemory);

   long      time = 0;

   long      julian;

   if (_clip_parinfo(ClipMachineMemory, 1) == DATE_type_of_ClipVarType)
      julian = _clip_pardj(ClipMachineMemory, 1);
   else if (_clip_parinfo(ClipMachineMemory, 1) == DATETIME_type_of_ClipVarType)
      julian = _clip_pardtj(ClipMachineMemory, 1, &time);
   else
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "TTOS");

   memset(r, 0, sizeof(ClipVar));
   r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
    _clip_ttoc(ClipMachineMemory, julian, time, &r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf,
	       "yyyy-mm-dd", 1, 1);
   return 0;
}
