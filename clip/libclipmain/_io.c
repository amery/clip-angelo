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
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <locale.h>
#include <signal.h>

#include "ci_clip.h"
#include "../libclipscreen/ci_screen.h"
#include "ci_hashcode.h"
#include "ci_error.ch"
#include "ci_ctwin.ch"
#include "ci_config.ch"
#include "ci_rational.h"
#include "ci_coll.h"

#ifdef USE_TASKS
#include "../libcliptask/ci_task.h"
#endif

ClipModule IO_module = {
   "_io",
   0,
   0,
   0,
   0,
   0,
   0,
   0
};

#define DEV_SCR 0
#define DEV_DEV 1
#define DEV_STD 2
#define DEV_ERR 3
#define DEV_DEVS 4
#define DEV_LOG 5
#define DEV_DBG 6
/*
   write function TRANSFORM as clipper+prg module
 */
/*
   #define CLIP_TRANSFORM_C
 */

static void out_any(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int dev);

static void out_scr(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int wrap);

static void out_dev(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int wrap);

static void out_std(ClipMachine * ClipMachineMemory, char *buf, int n, int attr);

static void out_err(ClipMachine * ClipMachineMemory, char *buf, int n, int attr);

static void out_dbg(ClipMachine * ClipMachineMemory, char *buf, int n);

static void print_var(ClipMachine * ClipMachineMemory, ClipVar * vp, int attr, int dev, int level);

#ifdef CLIP_TRANSFORM_C
static void transform(ClipMachine * ClipMachineMemory, ClipVar * vp, char *pict);

#endif
static void sync_mp(ClipMachine * ClipMachineMemory);

static void clip_region(ClipMachine * ClipMachineMemory, int *top, int *left, int *bottom, int *right, int full, int wnum);

static void wind_region(ClipMachine * ClipMachineMemory, int *ptop, int *pleft, int *pbottom, int *pright, int full);

extern FILE *logg;

extern int log_level;

static void dialog_init(ClipMachine * ClipMachineMemory);

const static int _clip_deck[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
   1000000000
};

const static double _clip_deck1[] = { 0.5f, 0.05f, 0.005f, 0.0005f, 0.00005f, 0.000005f, 0.0000005f,
   0.00000005f, 0.000000005f
};

void
_clip_dtos(double d, char *buf, int buflen, int *dp)
{
   int       dl;

   char     *s;

   snprintf(buf, buflen, "%.0f", d);
   dl = strlen(buf);
   if (dp)
      *dp = dl;
   snprintf(buf, buflen, "%.*f", buflen - dl - 1, d);

   for (s = buf + strlen(buf); s > buf; --s)
      if ((*s) == ',')
	 (*s) = '.';
   for (s = buf + strlen(buf); s > buf; --s)
      if (s[-1] != '0')
	 break;
   if (s > buf && s[-1] == '.')
      --s;
   *s = 0;
}

int
_clip_colorSelect(ClipMachine * ClipMachineMemory)
{
   int       ret;

   switch (ClipMachineMemory->colorSelect)
    {
    case 0:
       ret = ClipMachineMemory->attr.standard_of_ClipAttr;
       break;
    case 1:
       ret = ClipMachineMemory->attr.enhanced_of_ClipAttr;
       break;
    case 2:
       ret = ClipMachineMemory->attr.border_of_ClipAttr;
       break;
    case 3:
       ret = ClipMachineMemory->attr.background_of_ClipAttr;
       break;
    case 4:
       ret = ClipMachineMemory->attr.unselected_of_ClipAttr;
       break;
    case 5:
       ret = ClipMachineMemory->attr.u1_of_ClipAttr;
       break;
    case 6:
       ret = ClipMachineMemory->attr.u2_of_ClipAttr;
       break;
    case 7:
       ret = ClipMachineMemory->attr.u3_of_ClipAttr;
       break;
    case 8:
       ret = ClipMachineMemory->attr.u4_of_ClipAttr;
       break;
    case 9:
       ret = ClipMachineMemory->attr.u5_of_ClipAttr;
       break;
    default:
       ret = ClipMachineMemory->attr.standard_of_ClipAttr;
       break;
    }
   return ret;
}

int
_clip_sprintf(ClipMachine * ClipMachineMemory, const char *fmt, ...)
{
   va_list   ap;

   int       ret;

   va_start(ap, fmt);
   ret = _clip_vsprintf(ClipMachineMemory, fmt, ap);

   va_end(ap);

   return ret;
}

int
_clip_vsprintf(ClipMachine * ClipMachineMemory, const char *fmt, void *ap)
{
   for (;;)
    {
       int       n = vsnprintf(ClipMachineMemory->buf, ClipMachineMemory->buflen, fmt, ap);

       if (n < ClipMachineMemory->buflen)
	  return n;
       ClipMachineMemory->buflen = ClipMachineMemory->buflen * 2;
       ClipMachineMemory->buf = (char *) realloc(ClipMachineMemory->buf, ClipMachineMemory->buflen);
    }
}

static void
out_dev(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int wrap)
{
   if (ClipMachineMemory->flags & DEVICE_FLAG)
      goto prn;

   if (ClipMachineMemory->fullscreen)
    {
       if (wrap == 0 || (ClipMachineMemory->flags & CONSOLE_FLAG))
	  out_scr(ClipMachineMemory, buf, n, attr, wrap);
    }
   else if ((ClipMachineMemory->flags & CONSOLE_FLAG) || ClipMachineMemory->out != stdout)
    {
       int       i;

       if (ClipMachineMemory->obuf)
	{
	   int       i;

	   for (i = 0; i < n; i++)
	      putByte_Buf(ClipMachineMemory->obuf, _clip_outtbl[(unsigned char) buf[i]]);
	   return;
	}

       for (i = 0; i < n; i++)
	  fputc(_clip_outtbl[(unsigned char) buf[i]], (FILE *) ClipMachineMemory->out);
    }

   if (wrap == 1 && (ClipMachineMemory->flags & ALTERNATE_FLAG))
    {
       FILE     *alt = (FILE *) ClipMachineMemory->alt;

       if (alt)
	{
	   int       i;

	   for (i = 0; i < n; i++)
	      fputc(_clip_outtbl[(unsigned char) buf[i]], alt);
	}
    }

   if (wrap == 1 && (ClipMachineMemory->flags & EXTRA_FLAG))
    {
       FILE     *extra = (FILE *) ClipMachineMemory->extra;

       if (extra)
	{
	   int       i;

	   for (i = 0; i < n; i++)
	      fputc(_clip_outtbl[(unsigned char) buf[i]], extra);
	}
    }

   if ((ClipMachineMemory->flags & PRINTER_FLAG) && wrap)
    {
       FILE     *printer;

       int       i;

       char     *s;

     prn:
       if (ClipMachineMemory->pbuf)
	{
	   putBuf_Buf(ClipMachineMemory->pbuf, buf, n);
	}
       else
	{
	   printer = (FILE *) ClipMachineMemory->printer;
	   if (printer)
	    {
	       for (i = 0, s = buf; i < n; ++i, ++s)
		{
		   if (*s == '\n')
		    {
		       ClipMachineMemory->pcol = 0;
		       ClipMachineMemory->prow++;
		    }
		   else
		    {
		       if (*s != '\r')
			  ++ClipMachineMemory->pcol;
		    }
		}
	       for (i = 0; i < ClipMachineMemory->margin; ++i)
		  fputc(' ', printer);
	       for (i = 0; i < n; i++)
		  fputc(ClipMachineMemory->prntbl[(ClipMachineMemory->prn_xlat[(unsigned char) (buf[i])])], printer);
	    }
	}
    }
}

static void
out_std(ClipMachine * ClipMachineMemory, char *buf, int n, int attr)
{
   int       i;

   if (ClipMachineMemory->obuf)
    {
       int       i;

       for (i = 0; i < n; i++)
	  putByte_Buf(ClipMachineMemory->obuf, _clip_outtbl[(unsigned char) buf[i]]);
       return;
    }

   for (i = 0; i < n; i++)
      fputc(_clip_outtbl[(unsigned char) buf[i]], stdout);
}

static void
out_err(ClipMachine * ClipMachineMemory, char *buf, int n, int attr)
{
   int       i;

   if (ClipMachineMemory->ebuf)
    {
       int       i;

       for (i = 0; i < n; i++)
	  putByte_Buf(ClipMachineMemory->ebuf, _clip_outtbl[(unsigned char) buf[i]]);
       return;
    }

   for (i = 0; i < n; i++)
      fputc(_clip_outtbl[(unsigned char) buf[i]], stderr);
}

static void
out_dbg(ClipMachine * ClipMachineMemory, char *buf, int n)
{
   if (_clip_dbg_out)
    {
       int       i;

       for (i = 0; i < n; i++)
	  fputc(_clip_outtbl[(unsigned char) buf[i]], _clip_dbg_out);
    }
}

static void
out_log(ClipMachine * ClipMachineMemory, char *buf, int n, int attr)
{
   _clip_out_log(buf, n);
}

static void
out_any(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int dev)
{

   switch (dev)
    {
    case DEV_DEV:
       out_dev(ClipMachineMemory, buf, n, attr, 0);
       break;
    case DEV_DEVS:
       out_dev(ClipMachineMemory, buf, n, attr, 1);
       break;
    case DEV_SCR:
       out_scr(ClipMachineMemory, buf, n, attr, 0);
       break;
    case DEV_STD:
       out_std(ClipMachineMemory, buf, n, attr);
       break;
    case DEV_ERR:
       out_err(ClipMachineMemory, buf, n, attr);
       break;
    case DEV_LOG:
       out_log(ClipMachineMemory, buf, n, attr);
       break;
    case DEV_DBG:
       out_dbg(ClipMachineMemory, buf, n);
       break;
    }
}

int
_clip_printf(ClipMachine * ClipMachineMemory, const char *fmt, ...)
{
   int       n;

   va_list   ap;

   va_start(ap, fmt);
   n = _clip_vsprintf(ClipMachineMemory, fmt, ap);
   out_dev(ClipMachineMemory, ClipMachineMemory->buf, n, _clip_colorSelect(ClipMachineMemory), 1);
   va_end(ap);

   return n;
}

void
_clip_print_dbg(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   const char *nm = _clip_type_name(vp);

   if (_clip_dbg_out)
      fprintf(_clip_dbg_out, "%s; ", nm);
   print_var(ClipMachineMemory, vp, 0, DEV_DBG, 0);
}

static void
print_var(ClipMachine * ClipMachineMemory, ClipVar * vp, int attr, int dev, int level)
{
   if (level >= ClipMachineMemory->print_max_deep)
    {
       char      b[64];

       int       l;

       sprintf(b, "Max print level reached: %d ", level);
       l = strlen(b);
       if (dev != DEV_LOG)
	  out_dev(ClipMachineMemory, b, l, attr, dev);
       _clip_out_log(b, l);
       return;
    }

   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case UNDEF_type_of_ClipVarType:
       out_any(ClipMachineMemory, "NIL", 3, attr, dev);
       break;
    case NUMERIC_type_of_ClipVarType:
       {
	  if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	   {
	      char     *buf;

	      if (vp->ClipType_t_of_ClipVar.dec_of_ClipType)
		 buf =
		  rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10,
				    vp->ClipType_t_of_ClipVar.dec_of_ClipType, 0);
	      else
		 buf =
		  rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10,
				    ClipMachineMemory->decimals, 0);
	      out_any(ClipMachineMemory, buf, strlen(buf), attr, dev);
	      free(buf);
	   }
	  else
	   {
	      char      buf[48];

	      char     *s;

	      if (vp->ClipType_t_of_ClipVar.len_of_ClipType < sizeof(buf))
		 snprintf(buf, sizeof(buf), "%*.*f", vp->ClipType_t_of_ClipVar.len_of_ClipType,
			  vp->ClipType_t_of_ClipVar.dec_of_ClipType, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	      else
		 _clip_dtos(vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, buf, sizeof(buf), 0);
	      for (s = buf + strlen(buf); s > buf; --s)
		 if ((*s) == ',')
		    (*s) = '.';
	      out_any(ClipMachineMemory, buf, strlen(buf), attr, dev);
	   }
       }
       break;
    case CHARACTER_type_of_ClipVarType:
       out_any(ClipMachineMemory, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, attr, dev);
       break;
    case LOGICAL_type_of_ClipVarType:
       if (vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	  out_any(ClipMachineMemory, ".T.", 3, attr, dev);
       else
	  out_any(ClipMachineMemory, ".F.", 3, attr, dev);
       break;
    case DATE_type_of_ClipVarType:
       {
	  char     *s = _clip_date_to_str(vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, ClipMachineMemory->date_format);

	  out_any(ClipMachineMemory, s, strlen(s), attr, dev);
	  free(s);
       }
       break;
    case DATETIME_type_of_ClipVarType:
       {
	  int       len;

	  char     *s =
	   _clip_ttoc(ClipMachineMemory, vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar,
		      vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar, &len,
		      ClipMachineMemory->date_format, ClipMachineMemory->hours,
		      ClipMachineMemory->seconds);

	  out_any(ClipMachineMemory, s, len, attr, dev);
	  free(s);
       }
       break;
    case OBJECT_type_of_ClipVarType:
       vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->print_of_ClipObjRtti(ClipMachineMemory,
									    vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar,
									    vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar,
									    &ClipMachineMemory->buf,
									    &ClipMachineMemory->buflen);
       out_any(ClipMachineMemory, ClipMachineMemory->buf, ClipMachineMemory->buflen, attr, dev);
       break;
    case PCODE_type_of_ClipVarType:
    case CCODE_type_of_ClipVarType:
       out_any(ClipMachineMemory, "CODE", 4, attr, dev);
       break;
    case ARRAY_type_of_ClipVarType:
       {
	  int       i;

	  out_any(ClipMachineMemory, "{", 1, attr, dev);
	  for (i = 0; i < vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; ++i)
	   {
	      if (i)
		 out_any(ClipMachineMemory, ", ", 2, attr, dev);
	      print_var(ClipMachineMemory, vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i, attr, dev, level + 1);
	   }
	  out_any(ClipMachineMemory, "}", 1, attr, dev);
       }
       break;
    case MAP_type_of_ClipVarType:
       {
	  int       i;

	  out_any(ClipMachineMemory, "{", 1, attr, dev);
	  for (i = 0; i < vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar; ++i)
	   {
	      char      buf[64];

	      int       l;

	      if (i)
		 out_any(ClipMachineMemory, ", ", 2, attr, dev);
	      _clip_hash_name(ClipMachineMemory, vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].no_of_ClipVarEl,
			      buf, sizeof(buf));
	      l = strlen(buf);
	      snprintf(buf + l, sizeof(buf) - l, ":");
	      l += 1;
	      out_any(ClipMachineMemory, buf, l, attr, dev);
	      print_var(ClipMachineMemory,
			&vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl, attr, dev,
			level + 1);
	   }
	  out_any(ClipMachineMemory, "}", 1, attr, dev);
       }
       break;
    default:
       break;
    }
}

int
clip_QQOUT(ClipMachine * ClipMachineMemory)
{
   int       i;

   for (i = 1; i <= ClipMachineMemory->argc; ++i)
    {
       ClipVar  *vp = _clip_par(ClipMachineMemory, i);

       if (i > 1)
	  out_any(ClipMachineMemory, " ", 1, _clip_colorSelect(ClipMachineMemory), DEV_DEVS);
       if (!vp)
	  continue;
       print_var(ClipMachineMemory, vp, _clip_colorSelect(ClipMachineMemory), DEV_DEVS, 0);
    }
   sync_mp(ClipMachineMemory);

   return 0;
}

int
clip_QOUT(ClipMachine * ClipMachineMemory)
{
   char     *pEol = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("PRINTER_EOL"));

   if (pEol == NULL || *pEol == 0)
      out_any(ClipMachineMemory, "\n", 1, _clip_colorSelect(ClipMachineMemory), DEV_DEVS);
   else
      out_any(ClipMachineMemory, pEol, strlen(pEol), _clip_colorSelect(ClipMachineMemory), DEV_DEVS);
   clip_QQOUT(ClipMachineMemory);
   sync_mp(ClipMachineMemory);
   return 0;
}

int
clip_OUTSTD(ClipMachine * ClipMachineMemory)
{
   int       i;

   for (i = 1; i <= ClipMachineMemory->argc; ++i)
    {
       ClipVar  *vp = _clip_par(ClipMachineMemory, i);

       if (i > 1)
	  out_any(ClipMachineMemory, " ", 1, _clip_colorSelect(ClipMachineMemory), DEV_STD);
       print_var(ClipMachineMemory, vp, _clip_colorSelect(ClipMachineMemory), DEV_STD, 0);
    }
   return 0;
}

int
clip_OUTERR(ClipMachine * ClipMachineMemory)
{
   int       i;

   for (i = 1; i <= ClipMachineMemory->argc; ++i)
    {
       ClipVar  *vp = _clip_par(ClipMachineMemory, i);

       if (i > 1)
	  out_any(ClipMachineMemory, " ", 1, _clip_colorSelect(ClipMachineMemory), DEV_ERR);
       print_var(ClipMachineMemory, vp, _clip_colorSelect(ClipMachineMemory), DEV_ERR, 0);
    }
   return 0;
}

int
clip_OUTLOG(ClipMachine * ClipMachineMemory)
{
   char      buf[11];

   int       i;

   time_t    tt;

   struct tm *tp;

   int       level = _clip_parni(ClipMachineMemory, 1);

   if (level > log_level)
      return 0;

   tt = time(0);
   tp = localtime(&tt);
   snprintf(buf, 11, "%02d:%02d:%02d: ", tp->tm_hour, tp->tm_min, tp->tm_sec);
   _clip_out_log(buf, 10);
   buf[0] = ',';
   buf[1] = 0;
   for (i = 1; i <= ClipMachineMemory->argc; ++i)
    {
       ClipVar  *vp = _clip_par(ClipMachineMemory, i);

       if (i > 1)
	  out_any(ClipMachineMemory, " ", 1, _clip_colorSelect(ClipMachineMemory), DEV_LOG);
       print_var(ClipMachineMemory, vp, _clip_colorSelect(ClipMachineMemory), DEV_LOG, 0);
       if (i < ClipMachineMemory->argc)
	  _clip_out_log(buf, 1);
    }
   buf[0] = '\n';

   _clip_out_log(buf, 1);
   _clip_flush_log();
   return 0;
}

int
clip_SLEEP(ClipMachine * ClipMachineMemory)
{
   double    d = 60000;

   long      ld;

#ifndef USE_TASKS
   struct timeval tv;
#endif

   if (ClipMachineMemory->argc > 0)
      d = _clip_parnd(ClipMachineMemory, 1);

   ld = d;

#ifdef USE_TASKS
   Task_sleep(d * 1000);
#else

   tv.tv_sec = ld;
   tv.tv_usec = (d - (double) ld) * 1000000.0;

   select(0, 0, 0, 0, &tv);
#endif
   return 0;
}

/*
int
clip_WAITPERIOD(ClipMachine * ClipMachineMemory)
{
	int per = _clip_parni(ClipMachineMemory, 1);
	struct timeval tv;

	tv.tv_sec = per / 100;
	tv.tv_usec = (per % 100) * 10000;
	select(0, 0, 0, 0, &tv);
	_clip_retl(ClipMachineMemory, 1);
	return 0;
}
*/

int
clip_MILLISEC(ClipMachine * ClipMachineMemory)
{
   int       per = _clip_parni(ClipMachineMemory, 1);

   struct timeval tv;

   tv.tv_sec = per / 1000;
   tv.tv_usec = (per % 1000) * 1000;
   select(0, 0, 0, 0, &tv);
   return 0;
}

int
clip_NDATE(ClipMachine * ClipMachineMemory)
{
   int       dd = _clip_parni(ClipMachineMemory, 1);

   int       mm = _clip_parni(ClipMachineMemory, 2);

   int       yy = _clip_parni(ClipMachineMemory, 3);

   ClipVar  *rp = RETPTR(ClipMachineMemory);

   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _clip_jdate(dd, mm, yy);

   return 0;
}

char     *
_clip_strFunc(ClipMachine * ClipMachineMemory, ClipVar * v, int len, int dec, int pad)
{
   char     *buf = NULL;

   int       lend = 10, decd;

   char     *a, *b;

   lend = v->ClipType_t_of_ClipVar.len_of_ClipType;
   decd = v->ClipType_t_of_ClipVar.dec_of_ClipType;
   if (lend <= 0)
      lend = 10;
   if (len != -999)
    {
       if (len)
	  lend = len;
       if (dec >= 0)
	  decd = dec;
       else
	  decd = 0;
    }
   else
    {
       if (ClipMachineMemory->flags & FIXED_FLAG)
	{
	   if (decd == 0)
	      lend += ClipMachineMemory->decimals + 1;
	   else
	      lend += (ClipMachineMemory->decimals - decd);
	   decd = ClipMachineMemory->decimals;
	}
    }
   if (v->ClipType_t_of_ClipVar.memo_of_ClipType)
    {
       int       _lend;

       char     *_buf;

       _buf = rational_toString(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, decd, 0);
       _lend = strlen(_buf);
       if (_lend < lend)
	{
	   buf = malloc(lend + 1);
	   memset(buf, ' ', lend);
	   memcpy(buf + (lend - _lend), _buf, _lend);
	   free(_buf);
	}
       else
	{
	   buf = _buf;
	   lend = _lend;
	}
    }
   else
    {
       buf = malloc(lend + 1);
       _clip_dtostr(buf, lend, decd, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 0);
    }
   buf[lend] = 0;
  /* padr() ==2 & padc() ==3 need left orientation */
  /* padl() ==1 & str() == 0 need right orientation */
   if (pad < 2)
      return buf;
  /* formatiing to left */
   for (a = buf, b = buf; *b; b++)
    {
       if (*b != ' ')
	{
	   *a = *b;
	   a++;
	}
    }
   *a = 0;
   return buf;
}

int
clip_STR(ClipMachine * ClipMachineMemory)
{
   ClipVar  *v = _clip_par(ClipMachineMemory, 1);

   int       len = _clip_parni(ClipMachineMemory, 2);

   int       dec = _clip_parni(ClipMachineMemory, 3);

   char     *buf;

   _clip_retc(ClipMachineMemory, "");
   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STR");

   if (len < 0)
      return 0;

   if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
      len = -999;
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      dec = -999;

   buf = _clip_strFunc(ClipMachineMemory, v, len, dec, 0);
   _clip_retcn_m(ClipMachineMemory, buf, strlen(buf));
   return 0;
}

int
clip_RSTR(ClipMachine * ClipMachineMemory)
{
   int       lend = 10, decd = 2;

   char      buf[32], *s;

   ClipVar  *v = _clip_par(ClipMachineMemory, 1);

   int       len = _clip_parni(ClipMachineMemory, 2);

   int       dec = _clip_parni(ClipMachineMemory, 3);

   _clip_parp(ClipMachineMemory, 1, &lend, &decd);

   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType
       && !v->ClipType_t_of_ClipVar.memo_of_ClipType)
    {
       if (len)
	  snprintf(buf, sizeof(buf), "%*.*f", len, dec, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
       else
	  snprintf(buf, sizeof(buf), "%*.*f", lend, decd, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
       _clip_retc(ClipMachineMemory, buf);
    }
   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType
       && v->ClipType_t_of_ClipVar.memo_of_ClipType)
    {
       s = rational_toString2(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
       _clip_retcn_m(ClipMachineMemory, s, strlen(s));
    }
   return 0;
}

int
clip_STRZERO(ClipMachine * ClipMachineMemory)
{
   int       lend = 10, decd = 2;

   char     *s;

   ClipVar  *v = _clip_par(ClipMachineMemory, 1);

   int       len = _clip_parni(ClipMachineMemory, 2);

   int       dec = _clip_parni(ClipMachineMemory, 3);

   char     *buf;

   _clip_parp(ClipMachineMemory, 1, &lend, &decd);

   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType
       && !v->ClipType_t_of_ClipVar.memo_of_ClipType)
    {
       if (!len && !lend)
	{
	   char      b[32];

	   snprintf(b, sizeof(b), "%d", (int) v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	   _clip_retc(ClipMachineMemory, b);
	   return 0;
	}
       buf = malloc((len ? len : lend) + 1);

#if 1
      /* uri added */
       _clip_dtostr(buf, len ? len : lend,
		    _clip_parinfo(ClipMachineMemory,
				  3) != UNDEF_type_of_ClipVarType ? dec : decd,
		    v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
#else
       _clip_dtostr(buf, len ? len : lend, len ? dec : decd, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
#endif
       _clip_retcn_m(ClipMachineMemory, buf, len ? len : lend);
    }
   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType
       && v->ClipType_t_of_ClipVar.memo_of_ClipType)
    {
       if (len)
	  s = rational_toString(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, dec, 0);
       else
	{
	   if (lend)
	      s = rational_toString(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, decd, 0);
	   else
	      s =
	       rational_toString(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, ClipMachineMemory->decimals,
				 0);
	}
       _clip_retcn_m(ClipMachineMemory, s, strlen(s));
    }
   return 0;
}

#ifdef CLIP_TRANSFORM_C
static int
is_num(int c)
{
   switch (c)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '+':
    case '-':
       return 1;
    default:
       return 0;
    }
}

static int
do_templ(char *str, char *templ, char *buf, int buflen, int ins, int left, int no_zero)
{
   char     *t, *te, *s, *se, *d, *e, *p;

   int       dot = 0;

   if (!templ)
    {
       int       ret, l = strlen(str);

       if (l >= buflen)
	  ret = 0;
       else
	  ret = 1;

       strncpy(buf, str, buflen);
       return ret;
    }

   for (t = templ, s = str, d = buf, e = buf + buflen - 1; *t && d < e;)
    {
       int       f_type, t_len, s_len, i, lead;

      /* find template and source field */
       lead = 0;
     next:
       switch (*t)
	{
	case 0:
	   break;
	case '$':
	case '*':
	   lead = *t;
	   ++t;
	   goto next;
	case '9':
	case '#':
	case 'N':
	   for (t_len = 1, f_type = *t, te = t + t_len; *te == f_type; ++te, ++t_len)
	      ;
	   while (*s && !is_num(*s))
	      ++s;
	   for (s_len = 0, se = s; is_num(*se); ++se, ++s_len)
	      ;
	   if (s_len <= t_len)
	    {
	       if (!dot && !left)
		  for (i = t_len - s_len; i && d < e; --i, ++d)
		     *d = ' ';
	       if (lead)
		  *d++ = lead;
	       if (no_zero && s_len == 1 && *s == '0')
		  *d++ = ' ';
	       else
		  for (i = 0; i < s_len && d < e; ++d, ++s, ++i)
		     *d = *s;
	       if (dot && !no_zero)
		  for (i = t_len - s_len; i && d < e; --i, ++d)
		     *d = '0';
	       else if (left)
		  for (i = t_len - s_len; i && d < e; --i, ++d)
		     *d = ' ';
	       t = te;
	       s = se;
	       *d = 0;
	       dot = 0;
	       continue;
	    }
	   else
	     /* overflow */
	    {
	       for (i = 0; i < t_len && d < e; ++i, ++d, ++s)
		  *d = *s;
	       *d = 0;
	       s = se;
	       t = te;
	       continue;
	    }
	   break;
	case 'A':
	   *d = *s;
	   break;
	case '.':
	   p = strchr(s, '.');
	   if (p)
	    {
	       s = p;
	       *d = *s;
	    }
	   else
	    {
	       *d = '.';
	       if (*s)
		  --s;
	    }
	   dot++;
	   break;
	case ',':
	   p = strchr(s, ',');
	   if (p)
	    {
	       s = p;
	       *d = *s;
	    }
	   else
	    {
	       *d = ',';
	       if (*s)
		  --s;
	    }
	   break;
	case '!':
	   *d = toupper(*s);
	   break;
	case 'Y':
	   switch (toupper(*s))
	    {
	    case 'T':
	    case 'Y':
	       *d = 'Y';
	       break;
	    case 'F':
	    case 'N':
	       *d = 'N';
	       break;
	    default:
	       *d = *s;
	       break;
	    }
	   break;
	case 'L':
	   switch (toupper(*s))
	    {
	    case 'T':
	    case 'Y':
	       *d = 'T';
	       break;
	    case 'F':
	    case 'N':
	       *d = 'F';
	       break;
	    default:
	       *d = *s;
	       break;
	    }
	   break;
	default:
	   *d = *t;
	   if (ins && *s)
	      --s;
	   break;
	}
       if (*s)
	  ++s;
       ++d;
       ++t;
    }

  /* just copy rest */
   for (; *s && d < e; ++s, ++d)
      *d = *s;

   *d = 0;
   if (d == e)
      return 0;
   else
      return 1;
}

static void
transform(ClipMachine * ClipMachineMemory, ClipVar * vp, char *pict)
{
   char      dbuf[32];

   char     *templ = 0;

   int       funclen = 0;

   int       R_func = 0;

   int       B_func = 0;

   int       C_func = 0;

   int       D_func = 0;

   int       E_func = 0;

   int       X_func = 0;

   int       Z_func = 0;

   int       BR_func = 0;

   int       VO_func = 0;

   int       n = 0;

   if (!vp)
    {
       _clip_printf(ClipMachineMemory, "");
       return;
    }
   vp = _clip_unptr(vp);

   if (pict)
    {
       if (pict[0] == '@')
	{
	   for (templ = pict + 1; *templ; ++templ, ++funclen)
	      if (*templ == ' ')
	       {
		  ++templ;
		  break;
	       }
	   if (!*templ)
	      templ = 0;
	}
       else
	  templ = pict;
    }

   if (funclen)
    {
       char     *s, *e;

       for (s = pict + 1, e = s + funclen; *s && s < e; ++s)
	  switch (toupper(*s))
	   {
	   case 'R':
	      R_func = 1;
	      break;
	   case 'B':
	      B_func = 1;
	      break;
	   case 'C':
	      C_func = 1;
	      break;
	   case 'D':
	      D_func = 1;
	      break;
	   case 'E':
	      E_func = 1;
	      break;
	   case 'X':
	      X_func = 1;
	      break;
	   case 'Z':
	      Z_func = 1;
	      break;
	   case '(':
	   case ')':
	      BR_func = 1;
	      break;
	   case '!':
	      VO_func = 1;
	      break;
	   }

    }

   for (;;)
    {
       switch (vp->t.ClipVartype_type_of_ClipType)
	{
	case CHARACTER_type_of_ClipVarType:
	   n =
	    do_templ(vp->s.ClipBuf_str_of_ClipStrVar.buf, templ, ClipMachineMemory->buf,
		     ClipMachineMemory->buflen, R_func, B_func, Z_func);
	   break;
	case NUMERIC_t:
	   {
	      double    d;

	      if (vp->t.memo_of_ClipType)
		 d = rational_toDouble(vp->r.r);
	      else
		 d = vp->n.d;

	      _clip_dtos(d, dbuf, sizeof(dbuf), 0);
	      if (BR_func && d < 0)
	       {
		  d = -d;
		  _clip_dtos(d, dbuf, sizeof(dbuf), 0);
		  _clip_printf(ClipMachineMemory, "(%s)", dbuf);
		  strncpy(dbuf, ClipMachineMemory->buf, sizeof(dbuf));
	       }
	      else if (C_func && d > 0)
	       {
		  _clip_printf(ClipMachineMemory, "%s CR", dbuf);
		  strncpy(dbuf, ClipMachineMemory->buf, sizeof(dbuf));
	       }
	      else if (X_func && d < 0)
	       {
		  d = -d;
		  _clip_dtos(d, dbuf, sizeof(dbuf), 0);
		  _clip_printf(ClipMachineMemory, "%s DB", dbuf);
		  strncpy(dbuf, ClipMachineMemory->buf, sizeof(dbuf));
	       }

	      n = do_templ(dbuf, templ, ClipMachineMemory->buf, ClipMachineMemory->buflen, R_func, B_func, Z_func);
	   }
	   break;
	case LOGICAL_t:
	   if (vp->l.val)
	      strcpy(dbuf, "T");
	   else
	      strcpy(dbuf, "F");
	   n = do_templ(dbuf, templ, ClipMachineMemory->buf, ClipMachineMemory->buflen, R_func, B_func, Z_func);
	   break;
	case DATE_t:
	   {
	      long      julian = vp->d.julian;

	      char     *s;

	      int       l;

	      if (D_func)
		 templ = ClipMachineMemory->date_format;
	      else if (E_func)
		 templ = "dd/mm/yy";
	      s = _clip_date_to_str(julian, templ);
	      l = strlen(s);
	      if (l >= ClipMachineMemory->buflen)
		 n = 0;
	      else
	       {
		  n = 1;
		  strncpy(ClipMachineMemory->buf, s, ClipMachineMemory->buflen);
	       }
	      free(s);
	   }
	   break;
	case OBJECT_t:
	   _clip_printf(ClipMachineMemory, "OBJECT");
	   return;
	case ARRAY_t:
	   _clip_printf(ClipMachineMemory, "ARRAY");
	   return;
	case PCODE_t:
	case CCODE_t:
	   _clip_printf(ClipMachineMemory, "CODE");
	   return;
	default:
	   break;
	}
       if (n)
	  break;
       ClipMachineMemory->buflen *= 2;
       ClipMachineMemory->buf = (char *) realloc(ClipMachineMemory->buf, ClipMachineMemory->buflen);
    }
}

int
clip_TRANSFORM(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   char     *pict = _clip_parc(ClipMachineMemory, 2);

   transform(ClipMachineMemory, vp, pict);
   _clip_retcn(ClipMachineMemory, ClipMachineMemory->buf, strlen(ClipMachineMemory->buf));

   return 0;
}
#endif

int
_clip_dtostr(char *buf, int len, int dec, double d, int zero)
{
#if 1
   int       im = 0, ie;

   char     *s;

   double    de;

   int       neg = 0;

   int       cnt = 0;

   int       snp = 0;

   buf[len] = 0;

#ifdef CLIP_DEBUG
   printf("\n00dec=%d,len=%d", dec, len);
#endif
   if (dec == 0 ? dec >= len : dec >= len - 1)	/* (1) if not true dec + 1 < len */
      dec = 0;
  /*goto err;
     else
   */
   if (dec > 9)
      snp = 1;
   else
    {
       if (d < 0)
	{
	   d = -d;
	   neg = 1;
	}
       if (dec > 0)
	{
	   cnt = dec;
#ifdef ARCH_i386
	   (*(long long *) &d)++;
#endif
	   im = (int) (modf(d, &de) * _clip_deck[dec] + 0.5f);
	  /* chek if fractional part was rounded up */
	  /* commented out - 'ie += im' is faster
	     if (im >= _clip_deck[dec])
	     {
	     ++de;
	     im -= _clip_deck[dec];
	     }
	   */
	}
       else
	  modf(d + 0.5f, &de);

       if (de >= 1000000000)
	  snp = 1;
    }

#ifdef CLIP_DEBUG
   printf("\n01dec=%d,len=%d", dec, len);
#endif
   if (snp)
    {
#if 1				/* Uri added for str(-1234567890,10) */
       double    diff;

       snprintf(buf, len + 1, zero ? "%0*.*f" : "%*.*f", len, dec, neg ? -d : d);
       diff = fabs(fabs(atof(buf)) - d);
       if (diff > (d * MIN_NUMERIC_DIFF))
	  goto err;
      /*
         if (fabs(atof(buf)) != d)
         goto err;
       */
#else
       if (snprintf(buf, len + 1, zero ? "%0*.*f" : "%*.*f", len, dec, neg ? -d : d) < 0)
	  goto err;
#endif
       s = strchr(buf, ',');
       if (s)
	  *s = '.';
       return 0;
    }

#ifdef CLIP_DEBUG
   printf("\n02dec=%d,len=%d", dec, len);
#endif
   ie = (int) de;
   s = buf + len;

   if (cnt)
    {
       while (cnt--)
	{
	  /* (1) is fales, dec + 1 < len always so s > b */
	   *(--s) = '0' + im % 10;
	   im /= 10;
	}
       *(--s) = '.';
       ie += im;
      /* im > 0 if fractional part was rounded up
         the case of *.999 with dec == 2 (im == 100 in that case)
       */
    }

#ifdef CLIP_DEBUG
   printf("\n03dec=%d,len=%d", dec, len);
#endif
   while (1)
    {
       if (--s < buf)
	  goto err;
       *s = '0' + ie % 10;
       ie /= 10;
       if (!ie)
	  break;
    }

#ifdef CLIP_DEBUG
   printf("\n04dec=%d,len=%d", dec, len);
#endif
   if (zero)
    {
       if (neg ? s <= buf : s < buf)
	  goto err;
#ifdef CLIP_DEBUG
       printf("\n05dec=%d,len=%d", dec, len);
#endif
       while (s > buf)
	  *(--s) = '0';
       if (neg)
	  *s = '-';
    }
   else
    {
#ifdef CLIP_DEBUG
       printf("\n06dec=%d,len=%d", dec, len);
#endif
       if (neg)
	{
	   if (--s < buf)
	      goto err;
	   *s = '-';
	}
       while (s > buf)
	  *(--s) = ' ';
#ifdef CLIP_DEBUG
       printf("\n07dec=%d,len=%d", dec, len);
#endif
    }

   return 0;

 err:
   memset(buf, '*', len);
   if (dec > 0 && dec < len)
      buf[len - dec - 1] = '.';

   return 1;
}
#else
   int       im, ie;

   int       l;

   char     *s;

   double    de;

   int       neg = 0;

   int       cnt;

   buf[len] = 0;
   if (dec < 0 || dec > 9 || dec >= len - 1)
    {
       if (snprintf(buf, len + 1, "%*.*f", len, dec, d) < 0)
	  goto err;
       s = strchr(buf, ',');
       if (s)
	  *s = '.';
       return 0;
    }

   if (d < 0)
    {
       d = -d;
       neg = 1;
    }
   im = (int) (modf(d, &de) * _clip_deck[dec] + 0.5f);
   if (de >= 1000000000)
    {
       if (snprintf(buf, len + 1, "%*.*f", len, dec, neg ? -d : d) < 0)
	  goto err;
       s = strchr(buf, ',');
       if (s)
	  *s = '.';
       return 0;
    }
   ie = (int) de;
   s = buf + len;
   if (dec)
    {
       cnt = 0;
       while (cnt++ < dec)
	{
	   l = im % 10;
	   s--;
	   if (s < buf)
	      goto err;
	   *s = '0' + l;
	   im = im / 10;
	}
       *(--s) = '.';
    }
   ie += im;			/* the case of *.999 with dec == 2 (im == 100 in that case) */
   while (1)
    {
       l = ie % 10;
       s--;
       if (s < buf)
	  goto err;
       *s = '0' + l;
       ie = ie / 10;
       if (!ie)
	  break;
    }
   if (zero)
    {
       while (s > buf)
	  *(--s) = '0';
       if (neg)
	{
	   if (s < buf)
	      goto err;
	   *s = '-';
	}
    }
   else
    {
       if (neg)
	{
	   s--;
	   if (s < buf)
	      goto err;
	   *s = '-';
	}
       while (s > buf)
	  *(--s) = ' ';
    }
   return 0;
 err:
   memset(buf, '*', len);
   if (dec > 0 && dec < len)
      buf[len - dec - 1] = '.';
   return 1;
}
#endif

CLIP_DLLEXPORT double
_clip_strtod(char *str, int *decpos)
{
#if 1
   int       ie = 0;

   int       im = 0;

   double    d;

   char     *s = str, *e, *m;

   int       ecnt, mcnt, cnt;

   struct lconv *lc;

   int       neg = 0;

   while (*s == ' ')
      s++;
   if (*s == '+')
      s++;
   if (!(((*s >= '0') && (*s <= '9')) || (*s == '.') || (*s == ',') || (*s == '-') || (*s == '+')))
    {
       if (decpos)
	  *decpos = strlen(str);
       return 0;
    }

   if (*s == '-')
    {
       neg = 1;
       s++;
    }
   e = s;
   while (*s >= '0' && *s <= '9')
      s++;
   ecnt = s - e;

   m = s;
   if ((*m == '.') || (*m == ','))
    {
       if (decpos)
	  *decpos = (s == str) ? 1 : s - str;
       m++;
       while (*m >= '0' && *m <= '9')
	  m++;
       mcnt = m - s - 1;
    }
   else
    {
       mcnt = 0;
       if (decpos)
	  *decpos = 0;
    }

   if (ecnt > 9 || mcnt > 9)
    {
       if (mcnt > 0)
	{
	   lc = localeconv();
	   *s = *lc->decimal_point;
	   d = strtod(e, NULL);
	   *s = '.';
	}
       else
	  d = strtod(e, NULL);
    }
   else
    {
       for (m = s + 1, cnt = mcnt; cnt > 0; m++, cnt--)
	  im = im * 10 + (*m - '0');

       for (; ecnt > 0; e++, ecnt--)
	  ie = ie * 10 + (*e - '0');

       d = (double) ie;
       if (im > 0)
	  d += (double) (im) / _clip_deck[mcnt];
    }

   return neg ? -d : d;

#else
   int       ie = 0;

   int       im = 0;

   double    d;

   char     *s = str, *e;

   int       cnt, dcnt = 0;

   struct lconv *lc;

   int       neg = 0;

   while (*s == ' ')
      s++;
   if (!(((*s >= '0') && (*s <= '9')) || (*s == '.') || (*s == '-') || (*s == '+')))
    {
       if (decpos)
	  *decpos = strlen(str);
       return 0;
    }
   while ((*s >= '0' && *s <= '9') || *s == '-' || *s == '+')
      s++;

   e = s;
   while ((*e >= '0' && *e <= '9') || *e == '.')
      e++;
   if (*s != 0 && *s == '.')
    {
       if (decpos)
	  *decpos = s - str;
    }
   else
    {
       s = NULL;
       if (decpos)
	  *decpos = 0;
    }
   if (s)
    {
       if (s - str > 9 || e - s - 1 > 9)
	{
	   lc = localeconv();
	   *s = *lc->decimal_point;
	   if (decpos)
	      *decpos = s - str;
	   return strtod(str, NULL);
	}
    }
   else if (e - str > 9)
    {
       return strtod(str, NULL);
    }
   if (s)
    {
       for (dcnt = 0; (--e) > s; dcnt++)
	{
	   if (*e >= '0' && *e <= '9')
	      im += (*e - '0') * _clip_deck[dcnt];
	   else
	    {
	       if (*e == ' ')
		  dcnt--;
	       else
		  break;
	    }
	}
    }
   for (cnt = 0; (--e) >= str; cnt++)
    {
       if (*e == '-')
	{
	   neg = 1;
	   break;
	}
       if (*e >= '0' && *e <= '9')
	  ie += (*e - '0') * _clip_deck[cnt];
       else
	{
	   if (*e == ' ')
	      cnt--;
	   else
	      break;
	}
    }
   d = (double) ie;
   if (s)
      d += (double) (im) / _clip_deck[dcnt];
   if (neg)
      d = -d;
   return d;
#endif
}

int
clip_VAL(ClipMachine * ClipMachineMemory)
{
   int       l;

   char     *s = _clip_parcl(ClipMachineMemory, 1, &l);

   int       plen = _clip_parni(ClipMachineMemory, 2);

   int       pdec = _clip_parni(ClipMachineMemory, 3);

   int       i;

   if (!s)
    {
       _clip_retnd(ClipMachineMemory, 0);
       return 0;
    }
   if (ClipMachineMemory->flags & RATIONAL_FLAG)
    {
       for (i = l; i >= 0 && s[i] != '/' && s[i] != '.'; i--);
       if (i < 0)
	  i = 0;
       else
	  i = l - i - 1;
       _clip_retnr(ClipMachineMemory, rational_fromString(s), l, i);
    }
   else
    {
       char     *sp;

       int       dec = 0, len = 0;

       double    d;

       d = _clip_strtod(s, &dec);
       if (dec == 0)
	{
	   for (sp = s; *sp && (*sp == ' ' || *sp == '\t'); sp++)
	      ;
	}
       else
	  sp = s + dec;
       for (; *sp && (isdigit(*sp) || *sp == '.' || *sp == ',' || *sp == '-' || *sp == '+' || *sp == ' '); ++sp)
	  ;
       len = sp - s;
       if (dec)
	  dec = len - dec - 1;

       if (ClipMachineMemory->flags & FIXED_FLAG)
	  dec = ClipMachineMemory->decimals;
       if (dec < 0 || dec == len)
	  dec = 0;
       if (plen != 0)
	  len = plen;
       if (len < dec + 2)
	  len = dec + 2;
       if (_clip_parinfo(ClipMachineMemory, 0) == 3)
	  dec = pdec;
       _clip_retndp(ClipMachineMemory, d, len, dec);
    }

   return 0;
}

int
clip_FLOATVAL(ClipMachine * ClipMachineMemory)
{
   char     *s = _clip_parc(ClipMachineMemory, 1);

   int       len = _clip_parni(ClipMachineMemory, 2);

   int       dec = _clip_parni(ClipMachineMemory, 3);

   if (!s)
    {
       _clip_retnd(ClipMachineMemory, 0);
       return 0;
    }
   if (strchr(s, 'e') || strchr(s, 'E'))
    {
       struct lconv *lc = localeconv();

       char      c, *r;

       if (*lc->decimal_point == '.')
	  c = ',';
       else
	  c = '.';
       r = s;
       while ((r = strchr(r, c)))
	  *r = *lc->decimal_point;
       _clip_retndp(ClipMachineMemory, atof(s), len, dec);
       return 0;
    }
   return clip_VAL(ClipMachineMemory);
}

static void
eject(ClipMachine * ClipMachineMemory)
{
   FILE     *printer = (FILE *) ClipMachineMemory->printer;

   int       i;

   ClipMachineMemory->prow = 0;
   ClipMachineMemory->pcol = 0;

   if (!printer)
      return;

   fputc('\f', printer);
   fputc('\r', printer);

   for (i = 0; i < ClipMachineMemory->margin; ++i)
      fputc(' ', printer);
}

int
clip___EJECT(ClipMachine * ClipMachineMemory)
{
   eject(ClipMachineMemory);
   return 0;
}

int
clip_SETPOS(ClipMachine * ClipMachineMemory)
{
   int       y = _clip_parnl(ClipMachineMemory, 1);

   int       x = _clip_parnl(ClipMachineMemory, 2);

   int       scr = _clip_parl(ClipMachineMemory, 3);

   if (ClipMachineMemory->flags & DEVICE_FLAG)
    {
       int       i;

       FILE     *printer = (FILE *) ClipMachineMemory->printer;

       if (!printer)
	  goto norm;

       if (ClipMachineMemory->prow > y)
	  eject(ClipMachineMemory);

       if (ClipMachineMemory->prow < y)
	{
	   char     *pEol = (char *) _clip_fetch_item(ClipMachineMemory,
						      _clip_hashstr("PRINTER_EOL"));

	   while (ClipMachineMemory->prow < y)
	    {
	       ClipMachineMemory->pcol = 0;
	       ClipMachineMemory->prow++;
	       if (pEol == NULL || *pEol == 0)
		  fputc('\n', printer);
	       else
		  fputs(pEol, printer);

	    }
	   for (i = 0; i < ClipMachineMemory->margin; ++i)
	      fputc(' ', printer);
	}

       if (ClipMachineMemory->pcol > x)
	{
	   fputc('\r', printer);
	   ClipMachineMemory->pcol = 0;
	   for (i = 0; i < ClipMachineMemory->margin; ++i)
	      fputc(' ', printer);
	}

       while (ClipMachineMemory->pcol < x)
	{
	   fputc(' ', printer);
	   ClipMachineMemory->pcol++;
	}
    }

 norm:
   _clip_fullscreen(ClipMachineMemory);

   if (scr)
    {
       if (x < 0)
	  x = 0;
       if (x >= ClipMachineMemory->screen->base->Columns)
	  x = ClipMachineMemory->screen->base->Columns - 1;
       if (y < 0)
	  y = 0;
       if (y >= ClipMachineMemory->screen->base->Lines)
	  y = ClipMachineMemory->screen->base->Lines - 1;
    }
   else
      clip_region(ClipMachineMemory, &y, &x, 0, 0, 1, -1);

   ClipMachineMemory->screen->y = y;
   ClipMachineMemory->screen->x = x;
   sync_mp(ClipMachineMemory);

   return 0;
}

int
clip_SETCURSOR(ClipMachine * ClipMachineMemory)
{
   int       oldcursor;

   int       ret = 0;

   _clip_fullscreen(ClipMachineMemory);
   oldcursor = ClipMachineMemory->screen->cursor;

   switch (oldcursor)
    {
    case 0:
       ret = 1;
       break;
    case 1:
       ret = 0;
       break;
    case 2:
       ret = 3;
       break;
    }

   if (ClipMachineMemory->argc > 0)
    {
       int       cursor = _clip_parni(ClipMachineMemory, 1);

       int       scr_cursor;

       switch (cursor)
	{
	case 0:
	   scr_cursor = 1;
	   break;
	case 1:
	default:
	   scr_cursor = 0;
	   break;
	case 2:
	case 3:
	case 4:
	   scr_cursor = 2;
	   break;
	}

       ClipMachineMemory->screen->cursor = scr_cursor;

       sync_mp(ClipMachineMemory);
    }

   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_DEVPOS(ClipMachine * ClipMachineMemory)
{
   return clip_SETPOS(ClipMachineMemory);
}

static int
is_pgch(int ch)
{
   switch (ch)
    {
    case PGCH_HLINE:
    case PGCH_LLCORNER:
    case PGCH_LRCORNER:
    case PGCH_TTEE:
    case PGCH_RTEE:
    case PGCH_LTEE:
    case PGCH_BTEE:
    case PGCH_ULCORNER:
    case PGCH_URCORNER:
    case PGCH_VLINE:

    case PGCH_HLINE2:
    case PGCH_LLCORNER2:
    case PGCH_LRCORNER2:
    case PGCH_TTEE2:
    case PGCH_RTEE2:
    case PGCH_LTEE2:
    case PGCH_BTEE2:
    case PGCH_ULCORNER2:
    case PGCH_URCORNER2:
    case PGCH_VLINE2:

    case PGCH_PLUS:
    case PGCH_PLUS2:
    case PGCH_BLOCK:

    case PGCH_RARROW:
    case PGCH_LARROW:
       return 1;
    }

   return 0;
}

static void
out_scr(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int wrap)
{
   int       i;

   unsigned char *s, ch;

   Screen   *sp = ClipMachineMemory->screen;

   int       x = sp->x;

   int       y = sp->y;

   int       top = -1024, left = -1024, bottom = 1024, right = 1024;

   if (ClipMachineMemory->obuf)
    {
       int       i;

       for (i = 0; i < n; i++)
	  putByte_Buf(ClipMachineMemory->obuf, _clip_outtbl[(unsigned char) buf[i]]);
       return;
    }

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 1, -1);

   if (x > right || y > bottom || x < left || y < top)
      return;

   sp->touched[y] = 1;

   for (i = 0, s = (unsigned char *) buf; i < n; ++i, ++s)
    {
       switch ((ch = *s))
	{
	case '\n':
	   if (wrap)
	    {
	       ++y;
	       if (y <= bottom)
		  sp->touched[y] = 1;
	       x = left;
	    }
	   else
	      goto def;

	   break;
	case '\r':
	   if (wrap)
	    {
	       x = left;
	    }
	   else
	      goto def;
	   break;
	case '\b':
	   if (wrap)
	    {
	       --x;
	       if (x < left)
		{
		   --y;
		   x = right;
		   sp->touched[y] = 1;
		}
	    }
	   else
	      goto def;

	   break;
	case '\t':
	   if (wrap)
	    {
	       int       i = x;

	       x = (x / 8 + 1) * 8;
	       for (; i < x; ++i)
		{
		   if (i > right)
		      break;
		   sp->chars[y][i] = ' ';
		   sp->colors[y][i] = attr;
		   sp->attrs[y][i] = 0;
		}
	    }
	   else
	      goto def;

	   break;
	default:
	 def:
	   ch = ClipMachineMemory->term_xlat[ch];

	   if (ch < 32 && !is_pgch(ch))
	    {
	       sp->chars[y][x] = ch + 64;
	       sp->colors[y][x] = attr | 0x8;
	       sp->attrs[y][x] = 0;
	    }
	   else
	    {
	       sp->chars[y][x] = ch;
	       sp->colors[y][x] = attr;
	       sp->attrs[y][x] = 0;
	    }

	   ++x;
	   break;
	}

       if (wrap)
	{
	   if (x > right)
	    {
	       x = left;
	       y++;
	       if (y <= bottom)
		  sp->touched[y] = 1;
	    }
	   if (y > bottom)
	    {
	       top = 0;
	       clip_region(ClipMachineMemory, &top, 0, 0, 0, 0, -1);
	       while (y > bottom)
		{
		   scrollw_Screen(sp, top, left, bottom, right, 1, attr);
		   --y;
		}
	    }
	   if (y < top)
	    {
	       top = 0;
	       clip_region(ClipMachineMemory, &top, 0, 0, 0, 0, -1);
	       while (y < top)
		{
		   scrollw_Screen(sp, top, left, bottom, right, -1, attr);
		   ++y;
		}
	    }
	}
       else if (x > right)
	  break;
    }

  /*sp->touched[y] = 1; */
   sp->y = y;
   sp->x = x;
}

int
_clip_str2attr(char *str, int len)
{
   int       bg = 0, fg = 0, *ap = &fg, done = 0;

   char     *e;

   if (len == 2 && !strcmp(str, "-1"))
      return -1;

   for (e = str + len; !done && *str && str < e; ++str)
      switch (*str)
       {
       case ',':
	  done = 1;
	  break;
       case '/':
	  ap = &bg;
	  break;
       case 'U':
       case 'N':
       case 'X':
       case 'u':
       case 'n':
       case 'x':
       case ' ':
	  *ap |= COLOR_BLACK;
	  break;
       case 'B':
       case 'b':
	  *ap |= COLOR_BLUE;
	  break;
       case 'W':
       case 'w':
	  *ap |= COLOR_WHITE;
	  break;
       case 'G':
       case 'g':
	  *ap |= COLOR_GREEN;
	  break;
       case 'R':
       case 'r':
	  *ap |= COLOR_RED;
	  break;
       case '+':
	  fg |= COLOR_HI;
	  break;
       case 'I':
       case 'i':
	  *ap = ~*ap;
	  break;
       case '*':
	  bg |= COLOR_HI;
	  break;
       case '0':
       case '1':
       case '2':
       case '3':
       case '4':
       case '5':
       case '6':
       case '7':
       case '8':
       case '9':
	  *ap = strtol(str, &str, 10);
	  --str;
	  break;
       }
   return (((bg << 4) & 0xf0) | (fg & 0xf)) /*& 0x7f */ ;
}

int
_clip_attr2str(int attr, char *buf, int buflen, int num_format)
{
   int       l = 0;

   if (num_format)
    {
       snprintf(buf, buflen, "%d/%d", attr & 0xf, (attr >> 4) & 0xf);
       l = strlen(buf);
       return l;
    }

   if (attr & COLOR_HI)
    {
       buf[l] = '+';
       ++l;
    }

   if (l >= buflen)
      return l;

   if ((attr >> 4) & COLOR_HI)
    {
       buf[l] = '*';
       ++l;
    }

   if (l >= buflen)
      return l;

   switch (attr & 0x7)
    {
    case COLOR_BLACK:
       buf[l] = 'N';
       break;
    case COLOR_WHITE:
       buf[l] = 'W';
       break;
    case COLOR_BLUE:
    case COLOR_CYAN:
       buf[l] = 'B';
       break;
    case COLOR_GREEN:
    case COLOR_YELLOW:
       buf[l] = 'G';
       break;
    case COLOR_RED:
    case COLOR_MAGENTA:
       buf[l] = 'R';
       break;
    }

   ++l;
   if (l >= buflen)
      return l;

   switch (attr & 0x7)
    {
    case COLOR_CYAN:
       buf[l] = 'G';
       ++l;
       break;
    case COLOR_MAGENTA:
       buf[l] = 'B';
       ++l;
       break;
    case COLOR_YELLOW:
       buf[l] = 'R';
       ++l;
       break;
    }

   if (l >= buflen)
      return l;

   buf[l] = '/';
   ++l;

   if (l >= buflen)
      return l;

   switch ((attr >> 4) & 0x7)
    {
    case COLOR_BLACK:
       buf[l] = 'N';
       break;
    case COLOR_WHITE:
       buf[l] = 'W';
       break;
    case COLOR_BLUE:
    case COLOR_CYAN:
       buf[l] = 'B';
       break;
    case COLOR_GREEN:
    case COLOR_YELLOW:
       buf[l] = 'G';
       break;
    case COLOR_RED:
    case COLOR_MAGENTA:
       buf[l] = 'R';
       break;
    }

   ++l;
   if (l >= buflen)
      return l;

   switch ((attr >> 4) & 0x7)
    {
    case COLOR_CYAN:
       buf[l] = 'G';
       ++l;
       break;
    case COLOR_MAGENTA:
       buf[l] = 'B';
       ++l;
       break;
    case COLOR_YELLOW:
       buf[l] = 'R';
       ++l;
       break;
    }

   return l;
}

static int
get_color(ClipMachine * ClipMachineMemory, char *str)
{
   if (str)
    {
       int       l = strcspn(str, ",");

       return _clip_str2attr(str, l);
    }
   else
      return _clip_colorSelect(ClipMachineMemory);
}

void
_clip_fullscreen(ClipMachine * ClipMachineMemory)
{
   if (ClipMachineMemory->fullscreen)
      return;
   ClipMachineMemory->fullscreen = 1;
   _clip_init_tty(ClipMachineMemory);
   if (!ClipMachineMemory->screen)
    {
       char      msg[256];

       int       l;

       ClipFrame *fp;

       snprintf(msg, sizeof(msg), "request to switch into FULLSCREEN mode failed: %s\n", ClipMachineMemory->syserr);
       l = strlen(msg);
       out_log(ClipMachineMemory, msg, l, 0);
       out_err(ClipMachineMemory, msg, l, 0);
       for (fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
	{
	   _clip_logg(0, "trace: file '%s' line %d", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
	   fprintf(stderr, "trace: file '%s' line %d\n", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
	   fflush(stderr);
	}

       exit(2);
    }
  /*clear_Screen(ClipMachineMemory->screen); */
}

int
clip_DEVOUT(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);

   if (!vp)
      return 0;

   print_var(ClipMachineMemory, vp, get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2)), DEV_DEV, 0);
   sync_mp(ClipMachineMemory);

   return 0;
}

#ifdef CLIP_TRANSFORM_C
int
clip_DEVOUTPICT(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   char     *pict = _clip_parc(ClipMachineMemory, 2);

   int       attr;

   _clip_fullscreen(ClipMachineMemory);

   if (!vp)
      return 0;

   attr = get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 3));
   if (pict)
    {
       transform(ClipMachineMemory, vp, pict);
       out_dev(ClipMachineMemory, ClipMachineMemory->buf, strlen(ClipMachineMemory->buf), attr, 0);
    }
   else
      print_var(ClipMachineMemory, vp, attr, DEV_SCR, 0);

   sync_mp(ClipMachineMemory);

   return 0;
}
#endif

static void
sync_mp(ClipMachine * ClipMachineMemory)
{
   if (!ClipMachineMemory->update && !ClipMachineMemory->inkey)
    {
       if (ClipMachineMemory->fullscreen)
	  sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
       else if (ClipMachineMemory->flags1 & FLUSHOUT_FLAG)
	  fflush((FILE *) ClipMachineMemory->out);
    }
}

int
clip_DISPOUT(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);

   if (!vp)
      return 0;

   print_var(ClipMachineMemory, vp, get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2)), DEV_SCR, 0);
   sync_mp(ClipMachineMemory);

   return 0;
}

/* disppos(nRow,nCol) */
int
clip_DISPPOS(ClipMachine * ClipMachineMemory)
{
   int       y = _clip_parnl(ClipMachineMemory, 1);

   int       x = _clip_parnl(ClipMachineMemory, 2);

   _clip_fullscreen(ClipMachineMemory);

   clip_region(ClipMachineMemory, &y, &x, 0, 0, 1, -1);

   ClipMachineMemory->screen->y = y;
   ClipMachineMemory->screen->x = x;
   sync_mp(ClipMachineMemory);

   return 0;
}

/* dispoutat(nRow,nCol,var[,cColor]) */
int
clip_DISPOUTAT(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 3);

   clip_DISPPOS(ClipMachineMemory);

   _clip_fullscreen(ClipMachineMemory);

   if (!vp)
      return 0;

   print_var(ClipMachineMemory, vp, get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 4)), DEV_SCR, 0);
   sync_mp(ClipMachineMemory);

   return 0;
}

/*
   dispstr(nStr,nCol,cStr);
 */
int
clip_DISPSTR(ClipMachine * ClipMachineMemory)
{
   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   int       len;

   char     *str = _clip_parcl(ClipMachineMemory, 3, &len);

   int       bottom, right, i, k;

   Screen   *sp;

   if (!str)
      return 0;
   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   bottom = top;
   right = left + len - 1;

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);
   sp->touched[top] = 1;
   for (k = 0, i = left; i <= right; ++i, ++k)
      sp->chars[top][i] = str[k];

   sync_mp(ClipMachineMemory);

   return 0;
}

/* blinkattr([lEnable]) -> prevstate */
int
clip_BLINKATTR(ClipMachine * ClipMachineMemory)
{
   int       prevState = -1;

   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      prevState = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);
   setAttr_Screen(ClipMachineMemory->screen, &prevState, 0);

   _clip_retl(ClipMachineMemory, prevState);
   return 0;
}

/* boldattr([lEnable]) -> prevstate */
int
clip_BOLDATTR(ClipMachine * ClipMachineMemory)
{
   int       prevState = -1;

   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      prevState = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);
   setAttr_Screen(ClipMachineMemory->screen, 0, &prevState);

   _clip_retl(ClipMachineMemory, prevState);
   return 0;
}

/*
   dispattr(nTop,nLeft,nBottom,nRight,cColor|nColor)
 */
int
clip_DISPATTR(ClipMachine * ClipMachineMemory)
{
   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   int       bottom = _clip_parni(ClipMachineMemory, 3);

   int       right = _clip_parni(ClipMachineMemory, 4);

   int       i, j, attr;

   Screen   *sp;

   ClipVar  *vp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   vp = _clip_par(ClipMachineMemory, 5);
   if (vp)
    {
       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	  attr = _clip_parni(ClipMachineMemory, 5);
       else
	  attr = get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 5));
    }
   else
      attr = _clip_colorSelect(ClipMachineMemory);

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);
   for (i = top; i <= bottom; ++i)
    {
       sp->touched[i] = 1;
       for (j = left; j <= right; ++j)
	{
	  /*int pg = sp->colors[i][j] & PG_ATTR; */

	   sp->colors[i][j] = attr /*| pg */ ;
	}
    }

   sync_mp(ClipMachineMemory);
   return 0;
}

int
clip_FT_SHADOW(ClipMachine * ClipMachineMemory)
{
   return clip_DISPATTR(ClipMachineMemory);
}

int
clip_FT_SETATTR(ClipMachine * ClipMachineMemory)
{
   return clip_DISPATTR(ClipMachineMemory);
}

static void
disp_box(ClipMachine * ClipMachineMemory, int Top, int Left, int Bottom, int Right,
	 unsigned char *chars, int cl, int chars_n, char *color, int fill)
{
   Screen   *sp;

   int       attr = get_color(ClipMachineMemory, color);

   int       i, j;

   int       top, left, bottom, right;

   int       ulc, urc, hlt, hlb, llc, lrc, vll, vlr, mch = ' ', pg_char = 0;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   if (!chars)
      pg_char = 1;

   if (!chars && chars_n == 2)
    {
       ulc = PGCH_ULCORNER2;
       urc = PGCH_URCORNER2;
       hlt = PGCH_HLINE2;
       hlb = PGCH_HLINE2;
       llc = PGCH_LLCORNER2;
       lrc = PGCH_LRCORNER2;
       vll = PGCH_VLINE2;
       vlr = PGCH_VLINE2;
    }
   else
    {
       ulc = PGCH_ULCORNER;
       urc = PGCH_URCORNER;
       hlt = PGCH_HLINE;
       hlb = PGCH_HLINE;
       llc = PGCH_LLCORNER;
       lrc = PGCH_LRCORNER;
       vll = PGCH_VLINE;
       vlr = PGCH_VLINE;
    }

   top = Top;
   left = Left;
   bottom = Bottom;
   right = Right;

   wind_region(ClipMachineMemory, &Top, &Left, &Bottom, &Right, 0);
   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);

   if (chars && cl > 0)
    {
       ulc = mch;
       urc = mch;
       hlt = mch;
       hlb = mch;
       llc = mch;
       lrc = mch;
       vll = mch;
       vlr = mch;
       if (cl > 0)
	  ulc = chars[0];
       if (cl > 1)
	  hlt = chars[1];
       if (cl > 2)
	  urc = chars[2];
       if (cl > 3)
	  vlr = chars[3];
       if (cl > 4)
	  lrc = chars[4];
       if (cl > 5)
	  hlb = chars[5];
       if (cl > 6)
	  llc = chars[6];
       if (cl > 7)
	  vll = chars[7];
       if (cl > 8)
	{
	   mch = chars[8];
	   fill = 1;
	}
    }

   for (i = top; i <= bottom; ++i)
    {
       sp->touched[i] = 1;
       for (j = left; j <= right; ++j)
	{
	   int       ch;

	   if (top == bottom)
	      ch = hlt;
	   else if (left == right)
	      ch = vll;
	   else if (i == Top)
	    {
	       if (j == Left)
		  ch = ulc;
	       else if (j == Right)
		  ch = urc;
	       else
		  ch = hlt;
	    }
	   else if (i == Bottom)
	    {
	       if (j == Left)
		  ch = llc;
	       else if (j == Right)
		  ch = lrc;
	       else
		  ch = hlb;
	    }
	   else if (j == Left)
	      ch = vll;
	   else if (j == Right)
	      ch = vlr;
	   else if (fill)
	      ch = mch;
	   else
	      continue;

	   sp->chars[i][j] = ch;
	  /*if (pg_char)
	     sp->attrs[i][j] |= PG_ATTR; */
	   sp->colors[i][j] = attr;
	}
    }

   sync_mp(ClipMachineMemory);
}

int
clip_DISPBOXTERM(ClipMachine * ClipMachineMemory)
{
   int       Top = _clip_parni(ClipMachineMemory, 1);

   int       Left = _clip_parni(ClipMachineMemory, 2);

   int       Bottom = _clip_parni(ClipMachineMemory, 3);

   int       Right = _clip_parni(ClipMachineMemory, 4);

   int       cl = 0;

   unsigned char *chars = (unsigned char *) _clip_parcl(ClipMachineMemory, 5, &cl);

   int       chars_n = _clip_parni(ClipMachineMemory, 5);

   char     *color = _clip_parc(ClipMachineMemory, 6);

   if (cl > 0 || !chars)
      disp_box(ClipMachineMemory, Top, Left, Bottom, Right, chars, cl, chars_n, color, 0);
   else
      disp_box(ClipMachineMemory, Top, Left, Bottom, Right, (unsigned char *) ("         "), 9, chars_n, color, 0);

   return 0;
}

#ifdef USE_TASKS
int       clip_TASKSTART(ClipMachine * ClipMachineMemory);

int       clip_TASKSTOP(ClipMachine * ClipMachineMemory);
#endif

int
clip_DISPBEGIN(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   if (!ClipMachineMemory->update)
      clip_TASKSTOP(ClipMachineMemory);
#endif
   ClipMachineMemory->update++;
   return 0;
}

int
clip_DISPEND(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->update--;
   if (_clip_debuglevel)
      sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
   if (ClipMachineMemory->update <= 0)
    {
       ClipMachineMemory->update = 0;
       if (ClipMachineMemory->fullscreen)
	  sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
#ifdef USE_TASKS
       clip_TASKSTART(ClipMachineMemory);
#endif
    }
   return 0;
}

int
clip_DISPCOUNT(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, ClipMachineMemory->update);
   return 0;
}

int
clip_ROW(ClipMachine * ClipMachineMemory)
{
   int       r;

   int       scr = _clip_parl(ClipMachineMemory, 1);

   if (ClipMachineMemory->fullscreen)
    {
       if (scr)
	{
	   Screen   *sp = ClipMachineMemory->screen;

	   r = sp->y;
	}
       else
	{
	   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	   r = ClipMachineMemory->screen->y - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
	}
    }
   else
      r = 0;
   _clip_retnl(ClipMachineMemory, r);
   return 0;
}

int
clip_COL(ClipMachine * ClipMachineMemory)
{
   int       r;

   int       scr = _clip_parl(ClipMachineMemory, 1);

   if (ClipMachineMemory->fullscreen)
    {
       if (scr)
	{
	   Screen   *sp = ClipMachineMemory->screen;

	   r = sp->x;
	}
       else
	{
	   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	   r = ClipMachineMemory->screen->x - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
	}
    }
   else
      r = 0;
   _clip_retnl(ClipMachineMemory, r);
   return 0;
}

int
clip_PROW(ClipMachine * ClipMachineMemory)
{
   _clip_retnl(ClipMachineMemory, ClipMachineMemory->prow);
   return 0;
}

int
clip_PCOL(ClipMachine * ClipMachineMemory)
{
   _clip_retnl(ClipMachineMemory, ClipMachineMemory->pcol);
   return 0;
}

int
clip_SETPRC(ClipMachine * ClipMachineMemory)
{
   if (ClipMachineMemory->argc > 0)
      ClipMachineMemory->prow = _clip_parni(ClipMachineMemory, 1);
   if (ClipMachineMemory->argc > 1)
      ClipMachineMemory->pcol = _clip_parni(ClipMachineMemory, 2);
   return 0;
}

int
clip_MAXROW(ClipMachine * ClipMachineMemory)
{
   int       r;

   int       scr = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);
   if (ClipMachineMemory->fullscreen)
    {
       if (scr)
	{
	   Screen   *sp = ClipMachineMemory->screen;

	   r = sp->base->Lines - 1;
	}
       else
	{
	   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	   r = wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
	}
    }
   else
      r = 0;
   _clip_retnl(ClipMachineMemory, r);
   return 0;
}

int
clip_MAXCOL(ClipMachine * ClipMachineMemory)
{
   int       r;

   int       scr = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);
   if (ClipMachineMemory->fullscreen)
    {
       if (scr)
	{
	   Screen   *sp = ClipMachineMemory->screen;

	   r = sp->base->Columns - 1;
	}
       else
	{
	   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	   r = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
	}
    }
   else
      r = 0;
   _clip_retnl(ClipMachineMemory, r);
   return 0;
}

static int altkeys[] = {
   286,
   304,
   302,
   288,
   274,
   289,
   290,
   291,
   279,
   292,
   293,
   294,
   306,

   305,
   280,
   281,
   272,
   275,
   287,
   276,
   278,
   303,
   273,
   301,
   277,
   300,
};

static int altnums[] = {
   385,				/*   Alt-0  */
   376,				/*   Alt-1  */
   377,				/*   Alt-2  */
   378,				/*   Alt-3  */
   379,				/*   Alt-4  */
   380,				/*   Alt-5  */
   381,				/*   Alt-6  */
   382,				/*   Alt-7  */
   383,				/*   Alt-8  */
   384,				/*   Alt-9  */
};

static int
key2clip(unsigned long key, int mask)
{
   int       ckey = 0, flags;

   if (key & MOUSE_MASK)
    {
       int       type, buttons;

       if (!(mask & (_CLIP_INKEY_MOVE
		     | _CLIP_INKEY_LDOWN
		     | _CLIP_INKEY_LUP | _CLIP_INKEY_RDOWN | _CLIP_INKEY_RUP | _CLIP_INKEY_MDOWN | _CLIP_INKEY_MUP)))
	  return 0;

       type = MOUSE_TYPE(key);
       buttons = MOUSE_BUTTONS(key);

       if (type & MOUSE_TYPE_DOUBLE)
	{
	   if (type & MOUSE_TYPE_DOWN)
	      return 0;
	   if ((mask & (_CLIP_INKEY_LUP | _CLIP_INKEY_LDOWN)) && (buttons & MOUSE_BUTTONS_LEFT))
	      return 1006 /* K_LDBLCLK */ ;
	   else if ((mask & (_CLIP_INKEY_RUP | _CLIP_INKEY_RDOWN)) && (buttons & MOUSE_BUTTONS_RIGHT))
	      return 1007 /* K_RDBLCLK */ ;
	   else if ((mask & (_CLIP_INKEY_MUP | _CLIP_INKEY_MDOWN)) && (buttons & MOUSE_BUTTONS_MIDDLE))
	      return 1010 /* K_MDBLCLK */ ;
	   else
	      return 0;
	}

       switch (type & (MOUSE_TYPE_MOVE | MOUSE_TYPE_DRAG | MOUSE_TYPE_DOWN | MOUSE_TYPE_UP))
	{
	case MOUSE_TYPE_MOVE:
	case MOUSE_TYPE_DRAG:
	case MOUSE_TYPE_MOVE | MOUSE_TYPE_DRAG:
	   if ((mask & _CLIP_INKEY_MOVE))
	      return 1001 /* K_MOUSEMOVE */ ;
	   else
	      return 0;
	case MOUSE_TYPE_DOWN:
	   if ((mask & _CLIP_INKEY_LDOWN) && (buttons & MOUSE_BUTTONS_LEFT))
	      return 1002 /* K_LBUTTONDOWN */ ;
	   else if ((mask & _CLIP_INKEY_RDOWN) && (buttons & MOUSE_BUTTONS_RIGHT))
	      return 1004 /* K_RBUTTONDOWN */ ;
	   else if ((mask & _CLIP_INKEY_MDOWN) && (buttons & MOUSE_BUTTONS_MIDDLE))
	      return 1008 /* K_MBUTTONDOWN */ ;
	   else
	      return 0;
	case MOUSE_TYPE_UP:
	   if ((mask & _CLIP_INKEY_LUP) && (buttons & MOUSE_BUTTONS_LEFT))
	      return 1003 /* K_LBUTTONUP */ ;
	   else if ((mask & _CLIP_INKEY_RUP) && (buttons & MOUSE_BUTTONS_RIGHT))
	      return 1005 /* K_RBUTTONUP */ ;
	   else if ((mask & _CLIP_INKEY_MUP) && (buttons & MOUSE_BUTTONS_MIDDLE))
	      return 1009 /* K_MBUTTONUP */ ;
	   else
	      return 0;
	}
       return 0;
    }

   if (!(mask & _CLIP_INKEY_KEYBOARD))
      return 0;

   switch (key)
    {
    case KEY_LEFT:
       ckey = 19;
       break;
    case KEY_RIGHT:
       ckey = 4;
       break;
    case KEY_UP:
       ckey = 5;
       break;
    case KEY_DOWN:
       ckey = 24;
       break;
    case KEY_PGDN:
       ckey = 3;
       break;
    case KEY_PGUP:
       ckey = 18;
       break;
    case KEY_HOME:
       ckey = 1;
       break;
    case KEY_END:
       ckey = 6;
       break;
    case KEY_INS:
       ckey = 22;
       break;
    case KEY_BS:
    case 8:
    case 0x7f:
       ckey = 8;
       break;
    case KEY_DEL:
       ckey = 7;
       break;
    case KEY_ENTER:
       ckey = 13;
       break;
    case KEY_ESC:
       ckey = 27;
       break;

    case KEY_F1:
       ckey = 28;
       break;
    case KEY_F2:
       ckey = -1;
       break;
    case KEY_F3:
       ckey = -2;
       break;
    case KEY_F4:
       ckey = -3;
       break;
    case KEY_F5:
       ckey = -4;
       break;
    case KEY_F6:
       ckey = -5;
       break;
    case KEY_F7:
       ckey = -6;
       break;
    case KEY_F8:
       ckey = -7;
       break;
    case KEY_F9:
       ckey = -8;
       break;
    case KEY_F10:
       ckey = -9;
       break;

    case KEY_F11:
       ckey = -40;
       break;
    case KEY_F12:
       ckey = -41;
       break;

    case KEY_F13:
       ckey = -12;
       break;
    case KEY_F14:
       ckey = -13;
       break;
    case KEY_F15:
       ckey = -14;
       break;
    case KEY_F16:
       ckey = -15;
       break;
    case KEY_F17:
       ckey = -16;
       break;
    case KEY_F18:
       ckey = -17;
       break;
    case KEY_F19:
       ckey = -18;
       break;
    case KEY_F20:
       ckey = -19;
       break;

    case META1_MASK | KEY_F11:
       ckey = -46;
       break;
    case META1_MASK | KEY_F12:
       ckey = -47;
       break;
    case META2_MASK | KEY_F11:
       ckey = -44;
       break;
    case META2_MASK | KEY_F12:
       ckey = -45;
       break;
    case META1_MASK | META2_MASK | KEY_F11:
       ckey = -42;
       break;
    case META1_MASK | META2_MASK | KEY_F12:
       ckey = -43;
       break;

    case META1_MASK | KEY_UP:
       ckey = 408;
       break;
    case META1_MASK | KEY_DOWN:
       ckey = 416;
       break;
    case META1_MASK | KEY_LEFT:
       ckey = 411;
       break;
    case META1_MASK | KEY_RIGHT:
       ckey = 413;
       break;
    case META1_MASK | KEY_HOME:
       ckey = 407;
       break;
    case META1_MASK | KEY_END:
       ckey = 415;
       break;
    case META1_MASK | KEY_PGUP:
       ckey = 409;
       break;
    case META1_MASK | KEY_PGDN:
       ckey = 417;
       break;

    case META2_MASK | KEY_UP:
       ckey = 397;
       break;
    case META2_MASK | KEY_DOWN:
       ckey = 401;
       break;
    case META2_MASK | KEY_LEFT:
       ckey = 26;
       break;
    case META2_MASK | KEY_RIGHT:
       ckey = 2;
       break;
    case META2_MASK | KEY_HOME:
       ckey = 29;
       break;
    case META2_MASK | KEY_END:
       ckey = 23;
       break;
    case META2_MASK | KEY_PGUP:
       ckey = 31;
       break;
    case META2_MASK | KEY_PGDN:
       ckey = 30;
       break;

    case META1_MASK | '=':
       ckey = 387;
       break;

    case META1_MASK | '_':
       ckey = 386;
       break;

    case META1_MASK | '\\':
       ckey = 299;
       break;

    case META1_MASK | '[':
       ckey = 282;
       break;

    case META1_MASK | ']':
       ckey = 283;
       break;

    case META1_MASK | ';':
       ckey = 295;
       break;

    case META1_MASK | '\'':
       ckey = 296;
       break;

    case META1_MASK | ',':
       ckey = 307;
       break;

    case META1_MASK | '.':
       ckey = 308;
       break;

    case META1_MASK | '/':
      /*ckey = 309; */
       ckey = 420;
       break;

      /* shift-Tab */
    case META1_MASK | META2_MASK | 9:
       ckey = 271;
       break;

    case META1_MASK | KEY_INS:
       ckey = 418;
       break;
    case META1_MASK | KEY_DEL:
       ckey = 419;
       break;
    case META1_MASK | KEY_BS:
    case META1_MASK | 0x7f:
       ckey = 270;
       break;
    case META1_MASK | 9:
       ckey = 421;
       break;

    case META2_MASK | KEY_INS:
       ckey = 402;
       break;
    case META2_MASK | KEY_DEL:
       ckey = 403;
       break;
    case META2_MASK | KEY_BS:
    case META2_MASK | 0x7f:
       ckey = 127;
       break;
    case META2_MASK | 9:
       ckey = 404;
       break;

    case META1_MASK | KEY_ENTER:
       ckey = 422;
       break;
    case META1_MASK | KEY_ESC:
       ckey = 257;
       break;

    case META2_MASK | KEY_ENTER:
       ckey = 10;
       break;

      /*case META1_MASK | '/':
         ckey = 420;
         break; */
    case META1_MASK | '*':
       ckey = 311;
       break;
    case META1_MASK | '-':
       ckey = 330;
       break;
    case META1_MASK | '+':
       ckey = 334;
       break;

    case META2_MASK | '/':
       ckey = 405;
       break;
    case META2_MASK | '*':
       ckey = 406;
       break;
    case META2_MASK | '-':
       ckey = 398;
       break;
    case META2_MASK | '+':
       ckey = 400;
       break;

    case META2_MASK | '2':
       ckey = 259;
       break;

    default:
       if (key & (META_MASK | META1_MASK | META2_MASK))
	{
	   switch (key & ~(META1_MASK | META2_MASK))
	    {
	    case KEY_F1:
	    case KEY_F2:
	    case KEY_F3:
	    case KEY_F4:
	    case KEY_F5:
	    case KEY_F6:
	    case KEY_F7:
	    case KEY_F8:
	    case KEY_F9:
	    case KEY_F10:
	       switch (key & ~(META1_MASK | META2_MASK))
		{
		case KEY_F1:
		   ckey = -10;
		   break;
		case KEY_F2:
		   ckey = -11;
		   break;
		case KEY_F3:
		   ckey = -12;
		   break;
		case KEY_F4:
		   ckey = -13;
		   break;
		case KEY_F5:
		   ckey = -14;
		   break;
		case KEY_F6:
		   ckey = -15;
		   break;
		case KEY_F7:
		   ckey = -16;
		   break;
		case KEY_F8:
		   ckey = -17;
		   break;
		case KEY_F9:
		   ckey = -18;
		   break;
		case KEY_F10:
		   ckey = -19;
		   break;
		}
	       switch (key & (META1_MASK | META2_MASK))
		{
		case META2_MASK:
		   ckey -= 10;
		   break;
		case META1_MASK:
		   ckey -= 20;
		   break;
		}
	       break;
	    default:
	       flags = key & ~(META1_MASK | META2_MASK);
	       if (flags < 27)
		  ckey = altkeys[flags - 1];
	       else if (flags == '`' || flags == '~')
		  ckey = 297;
	       else if (flags >= '0' && flags <= '9')
		  ckey = altnums[flags - '0'];
	       else if (flags >= 'A' && flags <= 'Z')
		  ckey = altkeys[flags - 'A'];
	       else if (flags >= 'a' && flags <= 'z')
		  ckey = altkeys[flags - 'a'];
	       else if (key == (0x10000 | META2_MASK))
		{
		   ckey = 'J' - '@';
		   break;
		}
	       else
		{
		   ckey = key & ~(META_MASK | META1_MASK | META2_MASK);
		}

#if 0
	       switch (key & (META1_MASK | META2_MASK))
		{
		case META1_MASK:
		   ckey += 256;
		   break;
		case META2_MASK:
		   ckey += 512;
		   break;
		case META1_MASK | META2_MASK:
		   ckey += 768;
		   break;
		}
#endif

	       break;
	    }
	}
       else
	  ckey = key;
       break;
    }

   return ckey;
}

int
clip___KEYBOARD(ClipMachine * ClipMachineMemory)
{
  /* second parameters as "not clear keyboard buffer" */
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   if (!ClipMachineMemory->fullscreen)
      return 0;

   if (!_clip_parl(ClipMachineMemory, 2) || _clip_parinfo(ClipMachineMemory, 0) == 0)
    {
       int       tmp = ClipMachineMemory->lastkey;

       while (_clip_key(ClipMachineMemory, 0, 0xFF));
       ClipMachineMemory->lastkey = tmp;
       *ClipMachineMemory->kbdptr = ClipMachineMemory->kbdbuf;
		/*_clip_logg(0,"aaa1 clear kbdbuf,%p,%p",*ClipMachineMemory->kbdptr,ClipMachineMemory->kbdbuf);*/
    }

   if (vp == NULL)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
    {
       unsigned char *s = (unsigned char *) vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

       int       l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

       int       i;

       int       n = *ClipMachineMemory->kbdptr - ClipMachineMemory->kbdbuf;

		/*_clip_logg(0,"aaa2 clear kbdbuf,%p,%p,%s",*ClipMachineMemory->kbdptr,ClipMachineMemory->kbdbuf,s);*/
       if ((l - n) > ClipMachineMemory->typeahead)
	  l = ClipMachineMemory->typeahead - n;

       for (i = 0, s = s + l - 1; i < l; ++i, --s, (*ClipMachineMemory->kbdptr)++)
	  **ClipMachineMemory->kbdptr = *s;
    }
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       int       key = _clip_parni(ClipMachineMemory, 1);

       int       n = *ClipMachineMemory->kbdptr - ClipMachineMemory->kbdbuf;

		/*_clip_logg(0,"aaa3 clear kbdbuf,%p,%p,%d",*ClipMachineMemory->kbdptr,ClipMachineMemory->kbdbuf,key);*/
       if (n < ClipMachineMemory->typeahead)
	{
	   **ClipMachineMemory->kbdptr = key;
	   (*ClipMachineMemory->kbdptr)++;
	}

    }
	/*_clip_logg(0,"aaa3 clear kbdbuf,%p,%p",*ClipMachineMemory->kbdptr,ClipMachineMemory->kbdbuf);*/

   return 0;
}

int
clip_LASTKEY(ClipMachine * ClipMachineMemory)
{
   _clip_retndp(ClipMachineMemory, ClipMachineMemory->lastkey, 10, 0);
   return 0;
}

int
clip_SETLASTKEY(ClipMachine * ClipMachineMemory)
{
   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      ClipMachineMemory->lastkey = _clip_parni(ClipMachineMemory, 1);
   _clip_retc(ClipMachineMemory, "");
   return 0;
}

int
clip_NEXTKEY(ClipMachine * ClipMachineMemory)
{
   int       eventmask = _clip_parni(ClipMachineMemory, 1);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      eventmask = ClipMachineMemory->eventmask;

   _clip_fullscreen(ClipMachineMemory);
   if (*ClipMachineMemory->kbdptr != ClipMachineMemory->kbdbuf)
      _clip_retndp(ClipMachineMemory, (*ClipMachineMemory->kbdptr)[-1], 10, 0);
   else
    {
       long      key;

       int       ckey, l;

       key = getWait_Key(ClipMachineMemory->screen->base, 0);
       if (key)
	{
	   ckey = key2clip(key, eventmask);
	   if ((l = *ClipMachineMemory->kbdptr - ClipMachineMemory->kbdbuf) >= ClipMachineMemory->typeahead)
	    {
	       ClipMachineMemory->typeahead *= 2;
	       ClipMachineMemory->kbdbuf =
		(int *) realloc(ClipMachineMemory->kbdbuf, sizeof(int) * ClipMachineMemory->typeahead);

	       *ClipMachineMemory->kbdptr = ClipMachineMemory->kbdbuf + l;
	    }
	   *(*ClipMachineMemory->kbdptr)++ = ckey;

	   _clip_retndp(ClipMachineMemory, ckey, 10, 0);
	}
       else
	  _clip_retndp(ClipMachineMemory, 0, 10, 0);
    }
   return 0;
}

int
clip_ISKBDEMPTY(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);
   _clip_retl(ClipMachineMemory, *ClipMachineMemory->kbdptr == ClipMachineMemory->kbdbuf);
   return 0;
}

int
clip_CHR(ClipMachine * ClipMachineMemory)
{
   unsigned char buf[2];

   int       ch = _clip_parni(ClipMachineMemory, 1);

   buf[0] = ch;
   buf[1] = 0;
   _clip_retcn(ClipMachineMemory, (char *) buf, 1);

   return 0;
}

int
clip_ASC(ClipMachine * ClipMachineMemory)
{
   unsigned char *str = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   int       ch = 0;

   if (str)
      ch = *str;

   _clip_retni(ClipMachineMemory, ch);

   return 0;
}

#define timer_sub(a, b, result) \
   do {                         \
	   (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;     \
	   (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;  \
	   if ((result)->tv_usec < 0) {                      \
		  --(result)->tv_sec;                        \
		  (result)->tv_usec += 1000000;              \
	   }                                                 \
   } while (0)

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

CLIP_DLLEXPORT int
_clip_key(ClipMachine * ClipMachineMemory, int timeout_ms, int mask)
{
   unsigned long key;

   int       ckey = 0;

	/*_clip_fullscreen(ClipMachineMemory); */
	/*_clip_logg(0,"bbb1 clip_key,%p,%p",*ClipMachineMemory->kbdptr, ClipMachineMemory->kbdbuf);*/
   if (*ClipMachineMemory->kbdptr != ClipMachineMemory->kbdbuf)
    {
		/*_clip_logg(0,"bbb2 clear lastkey,%p,%p",*ClipMachineMemory->kbdptr, ClipMachineMemory->kbdbuf);*/
       (*ClipMachineMemory->kbdptr)--;
       ckey = ClipMachineMemory->lastkey = **ClipMachineMemory->kbdptr;
       return ckey;
    }

   signal(SIGINT, _clip_sigint_real);
   if (timeout_ms < 0)
      timeout_ms = 1000 * 6000;

   do
    {
       struct timeval end, tv, dt;

       gettimeofday(&tv, 0);
       dt.tv_sec = timeout_ms / 1000;
       dt.tv_usec = (timeout_ms % 1000) * 1000;

       timer_add(&tv, &dt, &end);

       key = getWait_Key(ClipMachineMemory->screen->base, timeout_ms);

       ckey = key2clip(key, mask);

       if (ckey != 0)
	{
	   if (timeout_ms != 0)
	      ClipMachineMemory->lastkey = ckey;

	   break;
	}

       gettimeofday(&tv, 0);
       timer_sub(&end, &tv, &dt);

       timeout_ms = dt.tv_sec * 1000 + dt.tv_usec / 1000;

    }
   while (timeout_ms > 0);

   signal(SIGINT, _clip_sigint);
   return ckey;
}

int
clip_SCANKEY(ClipMachine * ClipMachineMemory)
{
   unsigned long key;

   long      ms;

   if (ClipMachineMemory->argc < 1)
    {
       ms = 0;
    }
   else
    {
       double    t = _clip_parnd(ClipMachineMemory, 1);

       if (t > 0)
	  ms = t * 1000;
       else if (t == 0)
	  ms = 1000 * 6000;
       else
	  ms = 0;
    }

   dialog_init(ClipMachineMemory);
   sync_mp(ClipMachineMemory);

   key = getRawWait_Key(ClipMachineMemory->screen->base, ms);
   _clip_retnl(ClipMachineMemory, key);

   return 0;
}

int
clip_KBDSTAT(ClipMachine * ClipMachineMemory)
{
   int       flags;

   int       r;

   _clip_fullscreen(ClipMachineMemory);

   flags = getState_Key(ClipMachineMemory->screen->base);

   r = 0;
   if (flags & (1 << KEY_SHIFTL))
      r |= 0x1;
   if (flags & (1 << KEY_SHIFTR))
      r |= 0x2;
   if (flags & (1 << KEY_SHIFT) && !r)
      r |= 0x3;

   if (flags & ((1 << KEY_CTRL) | (1 << KEY_CTRLL) | (1 << KEY_CTRLR)))
      r |= 0x4;
   if (flags & ((1 << KEY_ALT) | (1 << KEY_ALTGR)))
      r |= 0x8;
   if (flags & (1 << KEY_SLOCK))
      r |= 0x10;
   if (flags & (1 << KEY_NATKBD))
      r |= 0x20000;
   if (flags & (1 << KEY_NUMLOCK))
      r |= 0x20;
   if (flags & (1 << KEY_CAPSLOCK))
      r |= 0x40;
   if (flags & (1 << KEY_INSLOCK))
      r |= 0x80;

   _clip_retni(ClipMachineMemory, r);

   return 0;
}

int
clip___KSETSTATE(ClipMachine * ClipMachineMemory, int locktype)
{
   int       newvalue = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);

   if (_clip_parinfo(ClipMachineMemory, 1) != LOGICAL_type_of_ClipVarType)
      newvalue = -1;
   _clip_retl(ClipMachineMemory, setState_Key(ClipMachineMemory->screen->base, newvalue, locktype));

   return 0;
}

int
clip_KSETNUM(ClipMachine * ClipMachineMemory)
{
   return clip___KSETSTATE(ClipMachineMemory, 1);
}

int
clip_KSETCAPS(ClipMachine * ClipMachineMemory)
{
   return clip___KSETSTATE(ClipMachineMemory, 2);
}

int
clip_KSETSCROLL(ClipMachine * ClipMachineMemory)
{
   return clip___KSETSTATE(ClipMachineMemory, 3);
}

int
clip_KSETINS(ClipMachine * ClipMachineMemory)
{
   return clip___KSETSTATE(ClipMachineMemory, 4);
}

int
clip__INKEY(ClipMachine * ClipMachineMemory)
{
   int       ckey;

   int       mask = ClipMachineMemory->eventmask;

   dialog_init(ClipMachineMemory);
   sync_mp(ClipMachineMemory);

   if (ClipMachineMemory->argc > 0 && _clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
    {
       double    t = _clip_parnd(ClipMachineMemory, 1);

       if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	  mask = _clip_parni(ClipMachineMemory, 2);

       if (t > 0)
	  ckey = _clip_key(ClipMachineMemory, t * 1000, mask);
       else if (t == 0)
	  ckey = _clip_key(ClipMachineMemory, -1, mask);
       else
	  ckey = _clip_key(ClipMachineMemory, 0, mask);
    }
   else
      ckey = _clip_key(ClipMachineMemory, 0, mask);

   _clip_retndp(ClipMachineMemory, ckey, 10, 0);

   return 0;
}

int
clip_SETKEY(ClipMachine * ClipMachineMemory)
{
   int       key = _clip_parni(ClipMachineMemory, 1);

   ClipVar  *bp = _clip_spar(ClipMachineMemory, 2);

   char      buf[16];

   long      hash;

   void     *p;

   snprintf(buf, sizeof(buf), "setkey_%x", key);
   hash = _clip_hashstr(buf);

   p = _clip_fetch_item(ClipMachineMemory, hash);

   if (bp)
    {
       ClipVar  *rp;

       if (p)
	{
	   rp = (ClipVar *) p;
	   _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), rp);
	   _clip_destroy(ClipMachineMemory, rp);
	   _clip_free_item(ClipMachineMemory, hash);
	}
       rp = (ClipVar *) calloc(sizeof(ClipVar), 1);

       _clip_clone(ClipMachineMemory, rp, bp);
       _clip_store_item(ClipMachineMemory, hash, rp);
    }
   else
    {
       if (p)
	{
	   ClipVar  *rp = (ClipVar *) p;

	   _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), rp);
	}
    }

   return 0;
}

#if 0

int
clip___WAIT(ClipMachine * ClipMachineMemory)
{
   int       l;

   int       ckey = 0;

   char     *prompt = _clip_parcl(ClipMachineMemory, 1, &l);

   dialog_init(ClipMachineMemory);
   if (!prompt)
    {
       prompt = "Press any key...";
       l = strlen(prompt);
    }

   out_dev(ClipMachineMemory, "\n", 1, _clip_colorSelect(ClipMachineMemory), 1);

   out_dev(ClipMachineMemory, prompt, l, _clip_colorSelect(ClipMachineMemory), 1);

   sync_mp(ClipMachineMemory);

   ckey = _clip_key(ClipMachineMemory, -1, _CLIP_INKEY_KEYBOARD);
   _clip_retni(ClipMachineMemory, ckey);

   return 0;
}

#endif

int
clip___ACCEPTSTR(ClipMachine * ClipMachineMemory)
{
   int       c;

   if ((c = ClipMachineMemory->history.count_of_ClipVect))
      _clip_retc(ClipMachineMemory, ClipMachineMemory->history.items_of_ClipVect[c - 1]);
   else
      _clip_retc(ClipMachineMemory, "");
   return 0;
}

int
clip___ACCEPT(ClipMachine * ClipMachineMemory)
{
   int       l, c = ClipMachineMemory->history.count_of_ClipVect;

   int       ckey = 0;

   int       pos, cpos, size = 16;

   unsigned char *s;

   char     *prompt = _clip_parcl(ClipMachineMemory, 1, &l);

   int       attr = _clip_colorSelect(ClipMachineMemory);

   int       hpos = c;

   dialog_init(ClipMachineMemory);
   if (prompt)
    {
		/*_clip_fullscreen(ClipMachineMemory); */
      /*out_dev(ClipMachineMemory, "\n", 1, attr, 1); */
       out_dev(ClipMachineMemory, prompt, l, attr, 1);
       sync_mp(ClipMachineMemory);
    }

   s = (unsigned char *) malloc(size);

   s[0] = 0;

   for (pos = 0, cpos = 0;;)
    {
       int       i;

       sync_mp(ClipMachineMemory);

       ckey = _clip_key(ClipMachineMemory, -1, _CLIP_INKEY_KEYBOARD);

       if (pos >= size - 1)
	{
	   size = size * 3 / 2;
	   s = (unsigned char *) realloc(s, size);
	}

       switch (ckey)
	{
	case 13:		/* Enter */
	   s[pos] = 0;
	   goto ret;
	case 27:		/* Esc */
	   s[0] = 0;
	   goto ret;
	case 5:		/* Up */
	   if (!hpos)
	      break;
	   hpos--;
	 newstr:
	   {
	      char     *sp = ClipMachineMemory->history.items_of_ClipVect[hpos];

	      int       l = strlen(sp);

	      free(s);
	      size = l + 4;
	      s = (unsigned char *) malloc(size);
	      memcpy(s, sp, l);
	      s[l] = 0;

	      for (i = 0; i < cpos; ++i)
		 out_dev(ClipMachineMemory, "\b", 1, attr, 1);

	      out_dev(ClipMachineMemory, (char *) s, l, attr, 1);

	      for (i = l; i < pos; ++i)
		 out_dev(ClipMachineMemory, " ", 1, attr, 1);

	      for (; i > l; --i)
		 out_dev(ClipMachineMemory, "\b", 1, attr, 1);

	      pos = cpos = l;
	   }
	   break;
	case 24:		/* Down */
	   if (hpos >= c - 1)
	      break;
	   ++hpos;
	   goto newstr;
	case 21:		/* Ctrl-U */
	   for (i = 0; i < cpos; ++i)
	      out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	   for (i = 0; i < pos; ++i)
	      out_dev(ClipMachineMemory, " ", 1, attr, 1);
	   for (i = 0; i < pos; ++i)
	      out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	   cpos = pos = 0;
	   break;
	case 1:		/* Home */
	   for (i = 0; i < cpos; ++i)
	      out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	   cpos = 0;
	   break;
	case 6:		/* End */
	   if (cpos < pos)
	    {
	       out_dev(ClipMachineMemory, (char *) (s + cpos), pos - cpos, attr, 1);
	       cpos = pos;
	    }
	   break;
	case 19:		/* Left */
	   if (cpos > 0)
	    {
	       out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	       --cpos;
	    }
	   break;
	case 4:		/* Right */
	   if (cpos < pos)
	    {
	       out_dev(ClipMachineMemory, (char *) (s + cpos), 1, attr, 1);
	       ++cpos;
	    }
	   break;
	case 8:		/* BS */
	   if (cpos > 0)
	    {
	       --pos;
	       --cpos;
	       if (pos == cpos)
		  out_dev(ClipMachineMemory, "\b \b", 3, attr, 1);
	       else
		{
		   int       l = pos - cpos;

		   memmove(s + cpos, s + cpos + 1, l);
		   s[pos] = ' ';
		   out_dev(ClipMachineMemory, "\b", 1, attr, 1);
		   out_dev(ClipMachineMemory, (char *) (s + cpos), l + 1, attr, 1);
		   for (i = 0; i <= l; ++i)
		      out_dev(ClipMachineMemory, "\b", 1, attr, 1);
		}
	    }
	   break;
	case 7:		/* Del */
	   if (cpos < pos)
	    {
	       --pos;
	       {
		  int       l = pos - cpos;

		  memmove(s + cpos, s + cpos + 1, l);
		  s[pos] = ' ';
		  out_dev(ClipMachineMemory, (char *) (s + cpos), l + 1, attr, 1);
		  for (i = 0; i <= l; ++i)
		     out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	       }
	    }
	   break;
	default:
	   if (ckey >= 32 && ckey < 256)
	    {
	       unsigned char b;

	       b = ckey;
	       if (cpos == pos)
		{
		   s[cpos++] = b;
		   pos = cpos;
		   out_dev(ClipMachineMemory, (char *) (&b), 1, attr, 1);
		}
	       else
		{
		   int       l = pos - cpos;

		   memmove(s + cpos + 1, s + cpos, l);
		   s[cpos] = b;
		   out_dev(ClipMachineMemory, (char *) (&b), 1, attr, 1);
		   out_dev(ClipMachineMemory, (char *) (s + cpos + 1), l, attr, 1);
		   for (i = 0; i < l; ++i)
		      out_dev(ClipMachineMemory, "\b", 1, attr, 1);
		   ++cpos;
		   ++pos;
		}
	    }
	   break;
	}
    }

 ret:
   _clip_retc(ClipMachineMemory, (char *) s);

   if (c == 0 || strcmp(ClipMachineMemory->history.items_of_ClipVect[c - 1], (char *) s))
      add_ClipVect(&ClipMachineMemory->history, s);
   else
      free(s);

   if (c >= CLIP_MAX_HISTORY)
    {
       free(ClipMachineMemory->history.items_of_ClipVect[0]);
       remove_ClipVect(&ClipMachineMemory->history, 0);
    }

   out_dev(ClipMachineMemory, "\n", 1, attr, 1);
   sync_mp(ClipMachineMemory);

   return 0;
}

static void
wind_region(ClipMachine * ClipMachineMemory, int *ptop, int *pleft, int *pbottom, int *pright, int full)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       ftop =
    (full == 2 ? wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect : wp->ClipRect_format_of_ClipWindow.top_of_ClipRect);
   int       fleft =
    (full == 2 ? wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect : wp->ClipRect_format_of_ClipWindow.left_of_ClipRect);

   if (ptop)
      *ptop += ftop;
   if (pleft)
      *pleft += fleft;
   if (pbottom)
      *pbottom += ftop;
   if (pright)
      *pright += fleft;
}

static void
clip_region(ClipMachine * ClipMachineMemory, int *ptop, int *pleft, int *pbottom, int *pright, int full, int wnum)
{
   ClipWindow *wp;

   int       top = -1024, left = -1024, bottom = 1024, right = 1024;

   int       Top, Bottom, Left, Right, ftop, fleft, columns, lines;

   if (wnum == -1)
      wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;
   else
      wp = ClipMachineMemory->windows + wnum;

  /*int Top = wp->rect.top; */
   Top = (full ? wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect : wp->ClipRect_format_of_ClipWindow.top_of_ClipRect);
   Bottom =
    (full == 2 ? wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect : wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect);
   Left = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect : wp->ClipRect_format_of_ClipWindow.left_of_ClipRect);
   Right =
    (full == 2 ? wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect : wp->ClipRect_format_of_ClipWindow.right_of_ClipRect);
   ftop = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect : wp->ClipRect_format_of_ClipWindow.top_of_ClipRect);
   fleft = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect : wp->ClipRect_format_of_ClipWindow.left_of_ClipRect);
   columns = ClipMachineMemory->screen->base->Columns - 1;
   lines = ClipMachineMemory->screen->base->Lines - 1;

   if (Bottom > lines)
      Bottom = lines;
   if (Right > columns)
      Right = columns;

   if (ptop)
      top = *ptop;
   if (pleft)
      left = *pleft;
   if (pbottom)
      bottom = *pbottom;
   if (pright)
      right = *pright;

   top += ftop;
   bottom += ftop;
   left += fleft;
   right += fleft;

   if (top < Top)
      top = Top;
   else if (top > Bottom)
      top = Bottom;

   if (bottom < Top)
      bottom = Top;
//      else if (bottom == Top)
  //              bottom = Bottom;
   else if (bottom > Bottom)
      bottom = Bottom;

   if (bottom < top)
      bottom = top;

   if (left < Left)
      left = Left;
   else if (left > Right)
      left = Right;

   if (right < Left)
      right = Left;
/*	else if (right == Left)
		right = Right;*/
   else if (right > Right)
      right = Right;

   if (right < left)
      right = left;

   if (ptop)
      *ptop = top;
   if (pleft)
      *pleft = left;
   if (pbottom)
      *pbottom = bottom;
   if (pright)
      *pright = right;

}

static char *
save_region(ClipMachine * ClipMachineMemory, char *mem, int top, int left, int bottom, int right, int *len)
{
   int       i, j;

   unsigned char *s, *p;

   Screen   *sp = ClipMachineMemory->screen;

#ifdef CLIP_DOS_SCRBUF
   i = 2;
#else
   i = 3;
#endif

   *len = (bottom - top + 1) * (right - left + 1) * i;
   s = (unsigned char *) realloc(mem, *len);

   for (i = top, p = s; i <= bottom; ++i)
      for (j = left; j <= right; ++j)
       {
	  *p++ = sp->chars[i][j];
	  *p++ = sp->colors[i][j];
#ifndef CLIP_DOS_SCRBUF
	  *p++ = sp->attrs[i][j];
#endif
       }
   return (char *) s;
}

static void
rest_region(ClipMachine * ClipMachineMemory, int top, int left, int bottom, int right, char *s, int l)
{
   int       i, j;

   unsigned char *p, *e;

   Screen   *sp = ClipMachineMemory->screen;

   int       lines = sp->base->Lines;

   int       columns = sp->base->Columns;

   for (i = top, p = (unsigned char *) s, e = p + l; i <= bottom; ++i)
    {
       if (i < 0 || i >= lines)
	  continue;
       sp->touched[i] = 1;
       for (j = left; j <= right; ++j)
	{
	   if (j < 0 || j >= columns)
	      continue;
	   if (p >= e)
	      return;
	   sp->chars[i][j] = *p++;
	   if (p >= e)
	      return;
	   sp->colors[i][j] = *p++;
#ifndef CLIP_DOS_SCRBUF
	   if (p >= e)
	      return;
	   sp->attrs[i][j] = *p++;
#endif
	}
    }
}

int
clip_SAVESCREEN(ClipMachine * ClipMachineMemory)
{
   int       l;

   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   int       bottom = _clip_parni(ClipMachineMemory, 3);

   int       right = _clip_parni(ClipMachineMemory, 4);

   int       par = _clip_parinfo(ClipMachineMemory, 0);

   int       mainscreen = (_clip_parl(ClipMachineMemory, 5) != 0 ? 0 : ClipMachineMemory->wnum);

   int       modescreen = (_clip_parl(ClipMachineMemory, 5) == 0 ? 0 : 2);

   ClipVar  *rp;

   char     *s;

   ClipWindow *wp;

   wp = ClipMachineMemory->windows + mainscreen;

   _clip_fullscreen(ClipMachineMemory);

   if (par < 3 || _clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      bottom = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect;
   if (par < 4 || _clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      right = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect;

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, modescreen, mainscreen);

   s = save_region(ClipMachineMemory, 0, top, left, bottom, right, &l);

   rp = RETPTR(ClipMachineMemory);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l;

   sync_mp(ClipMachineMemory);

   return 0;
}

int
clip_RESTSCREEN(ClipMachine * ClipMachineMemory)
{
   int       l;

   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   int       bottom = _clip_parni(ClipMachineMemory, 3);

   int       right = _clip_parni(ClipMachineMemory, 4);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 5);

   char     *s;

   if (!vp || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      return 0;

   _clip_fullscreen(ClipMachineMemory);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      top = 0;
   if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
      left = 0;
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
    {
       ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

       bottom = wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
    }
   if (_clip_parinfo(ClipMachineMemory, 4) != NUMERIC_type_of_ClipVarType)
    {
       ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

       right = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
    }
   s = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
   l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);
   rest_region(ClipMachineMemory, top, left, bottom, right, s, l);

   sync_mp(ClipMachineMemory);
   return 0;
}

int
clip_SCROLL(ClipMachine * ClipMachineMemory)
{
   int       i, j;

   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   int       bottom = _clip_parni(ClipMachineMemory, 3);

   int       right = _clip_parni(ClipMachineMemory, 4);

   int       vert = _clip_parni(ClipMachineMemory, 5);

   int       hor = _clip_parni(ClipMachineMemory, 6);

   int       attr = _clip_colorSelect(ClipMachineMemory);

   char      fill = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb));

   Screen   *sp;

   int       fs = ClipMachineMemory->fullscreen;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      top = 0;
   if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
      left = 0;
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      bottom = sp->base->Lines - 1;
   if (_clip_parinfo(ClipMachineMemory, 4) != NUMERIC_type_of_ClipVarType)
      right = sp->base->Columns - 1;

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);

   if (!vert && !hor)
    {
       if (!fs)
	  clear_Screen(ClipMachineMemory->screen);

       for (i = top; i <= bottom; ++i)
	{
	   sp->touched[i] = 1;
	   for (j = left; j <= right; ++j)
	    {
	       sp->chars[i][j] = fill;
	       sp->colors[i][j] = attr;
	       sp->attrs[i][j] = 0;
	    }
	}

       return 0;
    }

   if (vert)
    {
       char     *ls = 0, *rs = 0;

       int       ll, rl;

       if ((right - left) < (sp->base->Columns - /* / */ 2))
	{
	   for (i = top; i <= bottom; ++i)
	      sp->touched[i] = 1;

	   if (vert > 0)
	    {
	       for (i = left; i <= right; ++i)
		{
		   for (j = top; j <= bottom - vert; ++j)
		    {
		       sp->chars[j][i] = sp->chars[j + vert][i];
		       sp->colors[j][i] = sp->colors[j + vert][i];
		       sp->attrs[j][i] = sp->attrs[j + vert][i];
		    }
		   if (vert > bottom)
		      vert = bottom;
		   for (j = bottom - vert + 1; j <= bottom; ++j)
		    {
		       sp->chars[j][i] = fill;
		       sp->colors[j][i] = attr;
		       sp->attrs[j][i] = 0;
		    }
		}
	    }
	   else
	    {
	       for (i = left; i <= right; ++i)
		{
		   for (j = bottom; j >= top - vert; --j)
		    {
		       sp->chars[j][i] = sp->chars[j + vert][i];
		       sp->colors[j][i] = sp->colors[j + vert][i];
		       sp->attrs[j][i] = sp->attrs[j + vert][i];
		    }
		   if ((0 - vert) > top)
		      vert = 0 - top;
		   for (j = top - vert - 1; j >= top; --j)
		    {
		       sp->chars[j][i] = fill;
		       sp->colors[j][i] = attr;
		       sp->attrs[j][i] = 0;
		    }
		}
	    }
	}
       else
	{
	   if (left > 0)
	      ls = save_region(ClipMachineMemory, 0, top, 0, bottom, left - 1, &ll);
	   if (right < sp->base->Columns - 1)
	      rs = save_region(ClipMachineMemory, 0, top, right + 1, bottom, sp->base->Columns - 1, &rl);

	   scrollw_Screen(sp, top, left, bottom, right, vert, attr);

	   if (ls)
	      rest_region(ClipMachineMemory, top, 0, bottom, left - 1, ls, ll);
	   if (rs)
	      rest_region(ClipMachineMemory, top, right + 1, bottom, sp->base->Columns - 1, rs, rl);
	}
    }

   if (hor > 0)
    {
       for (i = top; i <= bottom; ++i)
	{
	   sp->touched[i] = 1;
	   for (j = left; j <= right - hor; ++j)
	    {
	       sp->chars[i][j] = sp->chars[i][j + hor];
	       sp->colors[i][j] = sp->colors[i][j + hor];
	       sp->attrs[i][j] = sp->attrs[i][j + hor];
	    }
	   for (j = right - hor + 1; j <= right; ++j)
	    {
	       sp->chars[i][j] = fill;
	       sp->colors[i][j] = attr;
	       sp->attrs[i][j] = attr;
	    }
	}
    }
   else if (hor < 0)
    {
       for (i = top; i <= bottom; ++i)
	{
	   sp->touched[i] = 1;
	   for (j = right; j >= left - hor; --j)
	    {
	       sp->chars[i][j] = sp->chars[i][j + hor];
	       sp->colors[i][j] = sp->colors[i][j + hor];
	       sp->attrs[i][j] = sp->attrs[i][j + hor];
	    }
	   for (j = left - hor - 1; j >= left; --j)
	    {
	       sp->chars[i][j] = fill;
	       sp->colors[i][j] = attr;
	       sp->attrs[i][j] = 0;
	    }
	}
    }

   sync_mp(ClipMachineMemory);

   return 0;
}

int
clip_MEMORY(ClipMachine * ClipMachineMemory)
{
   _clip_retnl(ClipMachineMemory, 0x7fffffffL);
   return 0;
}

int
clip_ALLOFREE(ClipMachine * ClipMachineMemory)
{
   _clip_retnl(ClipMachineMemory, 0xFFFF);
   return 0;
}

int
clip_STACKFREE(ClipMachine * ClipMachineMemory)
{
   _clip_retnl(ClipMachineMemory, 0xFFFF);
   return 0;
}

/* ][ window funcs */

typedef struct
{
   ClipAttr  attr;
   int       cursor;
   ClipRect  rect;
   int       dr, dc;
   int       y, x;
   int       l;
}
WindowSave;

static void
save_window(ClipMachine * ClipMachineMemory, ClipWindow * wp)
{
   int       l;

   char     *s;

   WindowSave *ws;

   ClipRect  w;

   int       lines = ClipMachineMemory->screen->base->Lines;

   int       columns = ClipMachineMemory->screen->base->Columns;

   w = wp->ClipRect_rect_of_ClipWindow;
   if (w.top_of_ClipRect < 0)
      w.top_of_ClipRect = 0;
   if (w.bottom_of_ClipRect < 0)
      return;
   if (w.left_of_ClipRect < 0)
      w.left_of_ClipRect = 0;
   if (w.right_of_ClipRect < 0)
      return;
   if (w.top_of_ClipRect >= lines)
      return;
   if (w.bottom_of_ClipRect >= lines)
      w.bottom_of_ClipRect = lines - 1;
   if (w.left_of_ClipRect >= columns)
      return;
   if (w.right_of_ClipRect >= columns)
      w.right_of_ClipRect = columns - 1;

   s =
    save_region(ClipMachineMemory, wp->save_of_ClipWindow, w.top_of_ClipRect, w.left_of_ClipRect, w.bottom_of_ClipRect,
		w.right_of_ClipRect, &l);
   s = (char *) realloc(s, l + sizeof(WindowSave));
   memmove(s + sizeof(WindowSave), s, l);
   ws = (WindowSave *) s;
   ws->attr = ClipMachineMemory->attr;
   ws->cursor = ClipMachineMemory->cursor;
   ws->rect = w;
   ws->dr = w.top_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   ws->dc = w.left_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   ws->y = ClipMachineMemory->screen->y - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   ws->x = ClipMachineMemory->screen->x - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   ws->l = l;

   wp->save_of_ClipWindow = s;
}

static void
draw_shadow(ClipMachine * ClipMachineMemory, ClipWindow * wp)
{
   int       lines, columns, nrow, ncol;

   int       top, left, bottom, right;

   int       i, j, attr;

   Screen   *sp;

   if (wp->shadow_of_ClipWindow == -1)
      return;

   sp = ClipMachineMemory->screen;
   lines = sp->base->Lines;
   columns = sp->base->Columns;
   attr = wp->shadow_of_ClipWindow;

   nrow = 1;
#if 0
   ncol = (columns - 20) / lines;
   if (ncol < 1)
      ncol = 1;
#else
   ncol = 2;
#endif

   top = wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   left = wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   bottom = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect;
   right = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect;

   for (i = top; i <= bottom + nrow; i++)
    {
       if (i < 0 || i >= lines)
	  continue;
       sp->touched[i] = 1;
       for (j = left + ncol; j <= right + ncol; j++)
	{
	   if (j < 0 || j >= columns)
	      continue;
	   if ((i >= (top + nrow) && j > right) || (j >= (left + ncol) && i > bottom))
	    {
	      /*int pg = sp->colors[i][j] & PG_ATTR; */

	       sp->colors[i][j] = attr /*| pg */ ;
	    }
	}
    }
}

static void
rest_window(ClipMachine * ClipMachineMemory, ClipWindow * wp)
{
   int       l;

   char     *s;

   WindowSave *ws;

   int       nr, nc, top, left, y, x;

   if (!(s = wp->save_of_ClipWindow))
      return;

   ws = (WindowSave *) s;
   s += sizeof(WindowSave);
   nr = ws->rect.bottom_of_ClipRect - ws->rect.top_of_ClipRect;
   nc = ws->rect.right_of_ClipRect - ws->rect.left_of_ClipRect;
   l = ws->l;
   top = wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect + ws->dr;
   left = wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect + ws->dc;

   rest_region(ClipMachineMemory, top, left, top + nr, left + nc, s, l);

   ClipMachineMemory->attr = ws->attr;
   ClipMachineMemory->cursor = ws->cursor;
   y = ws->y /*+ wp->rect.top */ ;
   x = ws->x /*+ wp->rect.left */ ;
   clip_region(ClipMachineMemory, &y, &x, 0, 0, 2, -1);
   ClipMachineMemory->screen->y = y;
   ClipMachineMemory->screen->x = x;

   draw_shadow(ClipMachineMemory, wp);
}

static void
destroy_window(ClipWindow * wp)
{
   if (wp->save_of_ClipWindow)
      free(wp->save_of_ClipWindow);
}

static int
select_window(ClipMachine * ClipMachineMemory, int n)
{
   int       no = -1;

   ClipWindow *wp = 0;

   int       ono = -1, nno, i;

   for (i = 0; i <= ClipMachineMemory->wnum; i++)
    {
       wp = ClipMachineMemory->windows + i;
       if (wp->no_of_ClipWindow == n)
	{
	   ono = i;
	   break;
	}
    }
   if (ono != -1)
    {
       ClipWindow w;

       nno = ClipMachineMemory->wnum;

       if ((ClipMachineMemory->windows + nno)->no_of_ClipWindow != 0)
	  save_window(ClipMachineMemory, ClipMachineMemory->windows + nno);
       if (nno != ono)
	{
	   w = ClipMachineMemory->windows[ono];
	   for (i = ono; i < ClipMachineMemory->wnum; i++)
	      ClipMachineMemory->windows[i] = ClipMachineMemory->windows[i + 1];
	   ClipMachineMemory->windows[ClipMachineMemory->wnum] = w;
	}
       if (n != 0)
	  rest_window(ClipMachineMemory, ClipMachineMemory->windows + ClipMachineMemory->wnum);

       sync_mp(ClipMachineMemory);
    }

   return no;
}

static void
calc_center(ClipMachine * ClipMachineMemory, ClipWindow * wp, int *top, int *left, int *bottom, int *right)
{
   int       sh = ClipMachineMemory->screen->base->Lines;

   int       sw = ClipMachineMemory->screen->base->Columns;

   int       h = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect + 1;

   int       w = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect + 1;

   int       t, l, b, r;

   t = (sh - h) / 2;
   b = t + h - 1;
   l = (sw - w) / 2;
   r = l + w - 1;

   if (top)
      *top = t;
   if (left)
      *left = l;
   if (bottom)
      *bottom = b;
   if (right)
      *right = r;
}

static void
move_window(ClipMachine * ClipMachineMemory, ClipWindow * wp, int top, int left)
{
   int       i, dv, dh;

   save_window(ClipMachineMemory, ClipMachineMemory->windows + ClipMachineMemory->wnum);

   dv = top - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   dh = left - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;

   wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect += dv;
   wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect += dv;
   wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect += dh;
   wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect += dh;
   wp->ClipRect_format_of_ClipWindow.top_of_ClipRect += dv;
   wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect += dv;
   wp->ClipRect_format_of_ClipWindow.left_of_ClipRect += dh;
   wp->ClipRect_format_of_ClipWindow.right_of_ClipRect += dh;

   for (i = 0; i <= ClipMachineMemory->wnum; i++)
      rest_window(ClipMachineMemory, ClipMachineMemory->windows + i);

   sync_mp(ClipMachineMemory);
}

static void
adjust_cursor(ClipMachine * ClipMachineMemory)
{
   int       x, y;

   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   x = ClipMachineMemory->screen->x;
   y = ClipMachineMemory->screen->y;
   if (y < wp->ClipRect_format_of_ClipWindow.top_of_ClipRect)
      y = wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
   if (y > wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect)
      y = wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect;
   if (x < wp->ClipRect_format_of_ClipWindow.left_of_ClipRect)
      x = wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
   if (x > wp->ClipRect_format_of_ClipWindow.right_of_ClipRect)
      x = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect;
   y -= wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
   x -= wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
   clip_region(ClipMachineMemory, &y, &x, 0, 0, 0, -1);

   ClipMachineMemory->screen->y = y;
   ClipMachineMemory->screen->x = x;
}

/* Closes all windows */
int
clip_WACLOSE(ClipMachine * ClipMachineMemory)
{
   int       num, i;

   ClipWindow *wp, w;

   _clip_fullscreen(ClipMachineMemory);
   num = ClipMachineMemory->wnum;

   for (i = 0; i <= num; i++)
    {
       wp = ClipMachineMemory->windows + i;
       if (wp->no_of_ClipWindow == 0)
	  w = *wp;
       else
	  destroy_window(wp);
    }

   ClipMachineMemory->windows[0] = w;

   ClipMachineMemory->wnum = 0;

   select_window(ClipMachineMemory, 0);

   return 0;
}

/*
   Allocates allowable screen area for windows
 */
int
clip_WBOARD(ClipMachine * ClipMachineMemory)
{
   int       top, left, bottom, right;

   _clip_fullscreen(ClipMachineMemory);

   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
    {
       Screen   *sp = ClipMachineMemory->screen;

       top = left = 0;
       bottom = sp->base->Lines - 1;
       right = sp->base->Columns - 1;
    }
   else
    {
       top = _clip_parni(ClipMachineMemory, 1);
       left = _clip_parni(ClipMachineMemory, 2);
       bottom = _clip_parni(ClipMachineMemory, 3);
       right = _clip_parni(ClipMachineMemory, 4);
    }

   ClipMachineMemory->wboard.top_of_ClipRect = top;
   ClipMachineMemory->wboard.left_of_ClipRect = left;
   ClipMachineMemory->wboard.bottom_of_ClipRect = bottom;
   ClipMachineMemory->wboard.right_of_ClipRect = right;

   _clip_retni(ClipMachineMemory, 0);

   return 0;
}

/*
   Places a frame around the active window
 */
int
clip_WBOXTERM(ClipMachine * ClipMachineMemory)
{
   int       cl = 0;

   unsigned char *chars = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &cl);

   unsigned char bchars[16];

   int       chars_n = _clip_parni(ClipMachineMemory, 1);

   ClipRect  rect, format;

   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       fill = 1;

   _clip_fullscreen(ClipMachineMemory);

   if (ClipMachineMemory->argc < 1)
      chars_n = 4;

   ClipMachineMemory->colorSelect = 0;

   rect = wp->ClipRect_rect_of_ClipWindow;
   format = wp->ClipRect_format_of_ClipWindow;

   format.top_of_ClipRect++;
   format.left_of_ClipRect++;
   format.bottom_of_ClipRect--;
   format.right_of_ClipRect--;

   if (format.bottom_of_ClipRect - format.top_of_ClipRect < 0 || format.right_of_ClipRect - format.left_of_ClipRect < 1)
    {
       _clip_retni(ClipMachineMemory, -1);
       return 0;
    }

   if (chars)
    {
       cl = strlen((const char *) chars);
       if (cl >= sizeof(bchars))
	  cl = sizeof(bchars) - 1;

       memcpy(bchars, chars, cl);
       if (cl < 9)
	  bchars[8] = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb));
       chars = bchars;
    }
   disp_box(ClipMachineMemory, 0, 0,
	    wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect,
	    wp->ClipRect_format_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect, chars, cl,
	    chars_n, 0, fill);

   wp->ClipRect_format_of_ClipWindow = format;
   if (fill)
    {
       ClipMachineMemory->screen->x = 0;
       ClipMachineMemory->screen->y = 0;
    }
   adjust_cursor(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, ClipMachineMemory->wnum);
   return 0;
}

/*
   Returns a window to the visible screen area, or centers it
 */
int
clip_WCENTER(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lCenter = _clip_parl(ClipMachineMemory, 1);

   int       top, left;

   _clip_fullscreen(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);

   calc_center(ClipMachineMemory, wp, &top, &left, 0, 0);
   if (lCenter)
      move_window(ClipMachineMemory, wp, top, left);
   else
      move_window(ClipMachineMemory, wp, top, left);

   return 0;
}

/*
   Closes the active window
 */
int
clip_WCLOSE(ClipMachine * ClipMachineMemory)
{
   int       num, i;

   ClipWindow *wp;

   _clip_fullscreen(ClipMachineMemory);

   num = ClipMachineMemory->wnum;
   wp = ClipMachineMemory->windows + num;

   if (wp->no_of_ClipWindow == 0)
      return 0;

   destroy_window(wp);

   num = --(ClipMachineMemory->wnum);
   wp = ClipMachineMemory->windows + num;
   if (wp->no_of_ClipWindow == 0 && num > 0)
    {
       ClipWindow wp2;

       wp2 = ClipMachineMemory->windows[num - 1];
       ClipMachineMemory->windows[num - 1] = ClipMachineMemory->windows[num];
       ClipMachineMemory->windows[num] = wp2;
    }
   wp = ClipMachineMemory->windows + num;

   for (i = 0; i <= ClipMachineMemory->wnum; i++)
      if ((ClipMachineMemory->windows + i)->no_of_ClipWindow == 0)
	 rest_window(ClipMachineMemory, ClipMachineMemory->windows + i);
   for (i = 0; i <= ClipMachineMemory->wnum; i++)
      if ((ClipMachineMemory->windows + i)->no_of_ClipWindow != 0)
	 rest_window(ClipMachineMemory, ClipMachineMemory->windows + i);

   sync_mp(ClipMachineMemory);

   return 0;
}

/*
   Returns position of the leftmost column of the active window
 */
int
clip_WCOL(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lCenter = _clip_parl(ClipMachineMemory, 1);

   int       r = wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;

   _clip_fullscreen(ClipMachineMemory);

   if (lCenter)
      calc_center(ClipMachineMemory, wp, 0, &r, 0, 0);

   _clip_retni(ClipMachineMemory, r);
   return 0;
}

/*
   Returns position of leftmost column of formatted area of window
 */
int
clip_WFCOL(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lRelative = _clip_parl(ClipMachineMemory, 1);

   int       r;

   _clip_fullscreen(ClipMachineMemory);

   if (lRelative)
      r = wp->ClipRect_format_of_ClipWindow.left_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   else
      r = wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;

   _clip_retni(ClipMachineMemory, r);
   return 0;
}

/*
   Returns position of rightmost col of formatted area of a window
 */
int
clip_WFLASTCOL(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lRelative = _clip_parl(ClipMachineMemory, 1);

   int       r;

   _clip_fullscreen(ClipMachineMemory);

   if (lRelative)
      r = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.right_of_ClipRect;
   else
      r = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect;

   _clip_retni(ClipMachineMemory, r);
   return 0;
}

/*
   Returns position of bottom row of formatted area of a window
 */
int
clip_WFLASTROW(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lRelative = _clip_parl(ClipMachineMemory, 1);

   int       r;

   _clip_fullscreen(ClipMachineMemory);

   if (lRelative)
      r = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect;
   else
      r = wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect;

   _clip_retni(ClipMachineMemory, r);
   return 0;
}

/*
   Determines the usable area within a window
 */
int
clip_WFORMAT(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       dt, dl, db, dr;

   ClipRect  format = wp->ClipRect_format_of_ClipWindow;

   _clip_fullscreen(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);
   if (ClipMachineMemory->argc < 4)
    {
       wp->ClipRect_format_of_ClipWindow = wp->ClipRect_rect_of_ClipWindow;
       return 0;
    }

   dt = _clip_parni(ClipMachineMemory, 1);

   dl = _clip_parni(ClipMachineMemory, 2);
   db = _clip_parni(ClipMachineMemory, 3);
   dr = _clip_parni(ClipMachineMemory, 4);

   format.top_of_ClipRect += dt;
   format.left_of_ClipRect += dl;
   format.bottom_of_ClipRect -= db;
   format.right_of_ClipRect -= dr;

   if (format.bottom_of_ClipRect - format.top_of_ClipRect < 0 || format.right_of_ClipRect - format.left_of_ClipRect < 0)
    {
       return 0;
    }

   if (format.top_of_ClipRect < wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect)
      format.top_of_ClipRect = wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   if (format.left_of_ClipRect < wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect)
      format.left_of_ClipRect = wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   if (format.bottom_of_ClipRect > wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect)
      format.bottom_of_ClipRect = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect;
   if (format.right_of_ClipRect > wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect)
      format.right_of_ClipRect = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect;

   wp->ClipRect_format_of_ClipWindow = format;

   adjust_cursor(ClipMachineMemory);

   return 0;
}

/*
   Returns position of top row of the formatted area of a window
 */
int
clip_WFROW(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lRelative = _clip_parl(ClipMachineMemory, 1);

   int       r;

   _clip_fullscreen(ClipMachineMemory);

   if (lRelative)
      r = wp->ClipRect_format_of_ClipWindow.top_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   else
      r = wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;

   _clip_retni(ClipMachineMemory, r);
   return 0;
}

/*
   Returns position of the rightmost column of the active window
 */
int
clip_WLASTCOL(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lCenter = _clip_parl(ClipMachineMemory, 1);

   int       r = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect;

   _clip_fullscreen(ClipMachineMemory);

   if (lCenter)
      calc_center(ClipMachineMemory, wp, 0, 0, 0, &r);

   _clip_retni(ClipMachineMemory, r);
   return 0;
}

/*
   Returns the position of the bottom row of the active window
 */
int
clip_WLASTROW(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lCenter = _clip_parl(ClipMachineMemory, 1);

   int       r = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect;

   _clip_fullscreen(ClipMachineMemory);

   if (lCenter)
      calc_center(ClipMachineMemory, wp, 0, 0, &r, 0);

   _clip_retni(ClipMachineMemory, r);
   return 0;
}

/*
   Turns the screen border overstep mode on or off
 */
int
clip_WMODE(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);

   return 0;
}

/*
   Moves a window
 */
int
clip_WMOVE(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   _clip_fullscreen(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);

   move_window(ClipMachineMemory, wp, top, left);

   return 0;
}

/*
   Determines the highest window handle
 */
int
clip_WNUM(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);
   _clip_retni(ClipMachineMemory, ClipMachineMemory->wnum);
   return 0;
}

/*
   Opens a new window
 */
int
clip_WOPEN(ClipMachine * ClipMachineMemory)
{
   int       top, left, bottom, right;

   int       erase;

   int       no = -1, i;

   ClipWindow *wp = 0;

   ClipRect  wb = ClipMachineMemory->wboard;

   _clip_fullscreen(ClipMachineMemory);
   if (ClipMachineMemory->argc < 4)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 1);
   left = _clip_parni(ClipMachineMemory, 2);
   bottom = _clip_parni(ClipMachineMemory, 3);
   right = _clip_parni(ClipMachineMemory, 4);
   erase = _clip_parl(ClipMachineMemory, 5);

   {
      top = top > wb.top_of_ClipRect ? top : wb.top_of_ClipRect;
      left = left > wb.left_of_ClipRect ? left : wb.left_of_ClipRect;
      bottom = bottom < wb.bottom_of_ClipRect ? bottom : wb.bottom_of_ClipRect;
      right = right < wb.right_of_ClipRect ? right : wb.right_of_ClipRect;
   }

   if (top > bottom || left > right)
      return EG_ARG;

   save_window(ClipMachineMemory, ClipMachineMemory->windows + ClipMachineMemory->wnum);
   ClipMachineMemory->wnum++;
   ClipMachineMemory->windows =
    (ClipWindow *) realloc(ClipMachineMemory->windows, (ClipMachineMemory->wnum + 1) * sizeof(ClipWindow));
   wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   memset(wp, 0, sizeof(ClipWindow));
   for (i = 0, no = 0; i < ClipMachineMemory->wnum; i++)
      if (ClipMachineMemory->windows[i].no_of_ClipWindow > no)
	 no = ClipMachineMemory->windows[i].no_of_ClipWindow;
   wp->no_of_ClipWindow = no + 1;
   wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect = top;
   wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect = bottom;
   wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect = left;
   wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect = right;
   wp->ClipRect_format_of_ClipWindow = wp->ClipRect_rect_of_ClipWindow;
   wp->shadow_of_ClipWindow = ClipMachineMemory->wshadow;

  //if (ClipMachineMemory->wshadow)
  //      erase = 1;

   ClipMachineMemory->screen->y = 0;
   ClipMachineMemory->screen->x = 0;
   adjust_cursor(ClipMachineMemory);

   if (erase)
    {
       int       attr = _clip_colorSelect(ClipMachineMemory);

       int       j;

       char      fill = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb));

       Screen   *sp = ClipMachineMemory->screen;

       bottom -= top;
       top = 0;
       right -= left;
       left = 0;
       clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 2, -1);
       for (i = top; i <= bottom; ++i)
	{
	   sp->touched[i] = 1;
	   for (j = left; j <= right; ++j)
	    {
	       sp->chars[i][j] = fill;
	       sp->colors[i][j] = attr;
	       sp->attrs[i][j] = 0;
	    }
	}
       draw_shadow(ClipMachineMemory, wp);
       sync_mp(ClipMachineMemory);
    }

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);

   return 0;
}

int
clip_WREOPEN(ClipMachine * ClipMachineMemory)
{
   int       top, left, bottom, right;

   int       erase, i;

   ClipWindow *wp = 0;

   _clip_fullscreen(ClipMachineMemory);
   if (ClipMachineMemory->argc < 4)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 1);
   left = _clip_parni(ClipMachineMemory, 2);
   bottom = _clip_parni(ClipMachineMemory, 3);
   right = _clip_parni(ClipMachineMemory, 4);
   erase = _clip_parl(ClipMachineMemory, 5);

   if (top > bottom || left > right)
      return EG_ARG;

  //save_window(ClipMachineMemory, ClipMachineMemory->windows + ClipMachineMemory->wnum);
   wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect = top;
   wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect = bottom;
   wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect = left;
   wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect = right;
   wp->ClipRect_format_of_ClipWindow = wp->ClipRect_rect_of_ClipWindow;

   ClipMachineMemory->screen->y = 0;
   ClipMachineMemory->screen->x = 0;
   adjust_cursor(ClipMachineMemory);

   if (erase)
    {
       int       attr = _clip_colorSelect(ClipMachineMemory);

       int       j;

       char      fill = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb));

       Screen   *sp = ClipMachineMemory->screen;

       bottom -= top;
       top = 0;
       right -= left;
       left = 0;
       clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 2, -1);
       for (i = top; i <= bottom; ++i)
	{
	   sp->touched[i] = 1;
	   for (j = left; j <= right; ++j)
	    {
	       sp->chars[i][j] = fill;
	       sp->colors[i][j] = attr;
	       sp->attrs[i][j] = 0;
	    }
	}
       draw_shadow(ClipMachineMemory, wp);
       sync_mp(ClipMachineMemory);
    }

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);

   return 0;
}

/*
   Returns the position of the top row of the active window
 */
int
clip_WROW(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int       lCenter = _clip_parl(ClipMachineMemory, 1);

   int       r = wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;

   _clip_fullscreen(ClipMachineMemory);

   if (lCenter)
      calc_center(ClipMachineMemory, wp, &r, 0, 0, 0);

   _clip_retni(ClipMachineMemory, r);
   return 0;
}

/*
   Activates one of the open windows
 */
int
clip_WSELECT(ClipMachine * ClipMachineMemory)
{
   int       no;

   int       n = _clip_parni(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);

   no = (ClipMachineMemory->windows + ClipMachineMemory->wnum)->no_of_ClipWindow;
#if 0
   if (ClipMachineMemory->argc > 0 && n != no)
      select_window(ClipMachineMemory, n);
   _clip_retni(ClipMachineMemory, no);
#else
   if (ClipMachineMemory->argc > 0 && n != no)
      select_window(ClipMachineMemory, n);
   _clip_retni(ClipMachineMemory, (ClipMachineMemory->windows + ClipMachineMemory->wnum)->no_of_ClipWindow);
#endif
   return 0;
}

/*
   Turns the interactive movement mode on or off
 */
int
clip_WSETMOVE(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);

   return 0;
}

/*
   Sets the window shadow colors
 */
int
clip_WSETSHADOW(ClipMachine * ClipMachineMemory)
{
   char     *color = _clip_parc(ClipMachineMemory, 1);

   int       ncolor = _clip_parni(ClipMachineMemory, 1);

   int       r = ClipMachineMemory->wshadow;

   _clip_fullscreen(ClipMachineMemory);

   if (ClipMachineMemory->argc > 0)
    {
       if (color)
	  ClipMachineMemory->wshadow = get_color(ClipMachineMemory, color);
       else
	  ClipMachineMemory->wshadow = ncolor;
    }

   _clip_retni(ClipMachineMemory, r);

   return 0;
}

/*
   Determines the step width of interactive window movement
 */
int
clip_WSTEP(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);

   return 0;
}

/* window funcs ][ */

static void
dialog_init(ClipMachine * ClipMachineMemory)
{
   if (!ClipMachineMemory->fullscreen)
    {
       static int dialog = 0;

       if (!dialog)
	{
	   int       r = _clip_init_tty(ClipMachineMemory);

	   if (!r)
	      dialog = 1;
	   else
	    {
	       int       l;

	       char      msg[256];

	       snprintf(msg, sizeof(msg), "\nrequest to switch into DIALOG mode failed: %s\n", ClipMachineMemory->syserr);
	       l = strlen(msg);
	       out_log(ClipMachineMemory, msg, l, 0);
	       out_err(ClipMachineMemory, msg, l, 0);
	       exit(2);
	    }
	}
/*_clip_restore_tty(ClipMachineMemory);*/
    }
}

int
clip___BEEP(ClipMachine * ClipMachineMemory)
{
   if (ClipMachineMemory->fullscreen)
      ClipMachineMemory->screen->beeps++;
   else
      fputc('\007', (FILE *) ClipMachineMemory->out);

   sync_mp(ClipMachineMemory);
   return 0;
}

/*
   screenmix(cCharStr, cAttrStr [, nLine [,nColumn]]) ->Nil
 */
int
clip_SCREENMIX(ClipMachine * ClipMachineMemory)
{
   int       len, alen;

   unsigned char *str;

   unsigned char *astr;

   int       y, x, ey, ex, i, l;

   Screen   *sp;

   ClipWindow *wp;

   str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &len);
   astr = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &alen);

   if (!str || !astr)
      return EG_ARG;

   _clip_fullscreen(ClipMachineMemory);

   sp = ClipMachineMemory->screen;

   wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   if (ClipMachineMemory->argc > 2)
      y = _clip_parnl(ClipMachineMemory, 3);
   else
      y = ClipMachineMemory->screen->y - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;

   if (ClipMachineMemory->argc > 3)
      x = _clip_parnl(ClipMachineMemory, 4);
   else
      x = ClipMachineMemory->screen->x - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;

   if (len < alen)
      l = alen;
   else
      l = len;

   ex = x + l;
   ey = y;

   clip_region(ClipMachineMemory, &y, &x, &ey, &ex, 1, -1);

   if (y <= ey)
    {
       sp->touched[y] = 1;
       for (i = 0; x <= ex && i <= l; ++i, ++x)
	{
	   if (i < len)
	      sp->chars[y][x] = str[i];
	   if (i < alen)
	      sp->colors[y][x] = astr[i];
	}
    }

   sync_mp(ClipMachineMemory);

   return 0;
}

/*
	   MPRESENT() --> lIsPresent
*/
int
clip_MPRESENT(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retl(ClipMachineMemory, sp->mouse_present);

   return 0;
}

/*
	   MHIDE() --> nOldVisibleStatus
	   note: all parameters are ignored
*/
int
clip_MHIDE(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_visible);
   sp->mouse_visible = 0;

   return 0;
}

/*
	   MSHOW() --> nOldVisibleStatus
	   note: all parameters are ignored
*/
int
clip_MSHOW(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_visible);
   sp->mouse_visible = 1;

   return 0;
}

/*
	   MSETCURSOR( [<lVisible>] ) --> lIsVisible
*/
int
clip_MSETCURSOR(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_visible);

   if (ClipMachineMemory->argc > 0)
      sp->mouse_visible = _clip_parl(ClipMachineMemory, 1);

   return 0;
}

/*
	   MROW() --> nMouseRow
*/
int
clip_MROW(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_y);

   return 0;
}

/*
	   MCOL() --> nMouseCol
*/
int
clip_MCOL(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_x);

   return 0;
}

/*
	   MLEFTDOWN() --> lIsPressed
*/
int
clip_MLEFTDOWN(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retl(ClipMachineMemory, (sp->mouse_buttons & MOUSE_BUTTONS_LEFT) ? 1 : 0);

   return 0;
}

/*
	   MRIGHTDOWN() --> lIsPressed
*/
int
clip_MRIGHTDOWN(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retl(ClipMachineMemory, (sp->mouse_buttons & MOUSE_BUTTONS_RIGHT) ? 1 : 0);

   return 0;
}

/*
	   MMIDDLEDOWN() --> lIsPressed
*/
int
clip_MMIDDLEDOWN(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retl(ClipMachineMemory, (sp->mouse_buttons & MOUSE_BUTTONS_MIDDLE) ? 1 : 0);

   return 0;
}

/*
	   MSETBOUNDS( [<nTop>], [<nLeft>], [<nBottom>],
				 [<nRight>] ) --> NIL
*/
int
clip_MSETBOUNDS(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   int       top, bottom, left, right;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   top = 0;
   bottom = sp->Lines - 1;
   left = 0;
   right = sp->Columns - 1;

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
    {
       top = _clip_parni(ClipMachineMemory, 1);
       if (top < 0)
	  top = 0;
       if (top >= sp->Lines)
	  top = sp->Lines - 1;
    }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
    {
       left = _clip_parni(ClipMachineMemory, 1);
       if (left < 0)
	  left = 0;
       if (left >= sp->Columns)
	  left = sp->Columns - 1;
    }

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
    {
       bottom = _clip_parni(ClipMachineMemory, 1);
       if (bottom < 0)
	  bottom = 0;
       if (bottom >= sp->Lines)
	  bottom = sp->Lines - 1;
    }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
    {
       right = _clip_parni(ClipMachineMemory, 1);
       if (right < 0)
	  right = 0;
       if (right >= sp->Columns)
	  right = sp->Columns - 1;
    }

   if (left > right || top > bottom)
      return EG_ARG;

   sp->mouse_top = top;
   sp->mouse_left = left;
   sp->mouse_bottom = bottom;
   sp->mouse_right = right;

   return 0;
}

/*
	   MSAVESTATE() --> cSaveState
*/
int
clip_MSAVESTATE(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   int      *save;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   save = (int *) malloc(7 * sizeof(int));

   save[0] = sp->mouse_visible;
   save[1] = sp->mouse_y;
   save[2] = sp->mouse_x;
   save[3] = sp->mouse_top;
   save[4] = sp->mouse_left;
   save[5] = sp->mouse_bottom;
   save[6] = sp->mouse_right;

   _clip_retcn_m(ClipMachineMemory, (char *) save, 7 * sizeof(int));

   return 0;
}

/*
	   MRESTSTATE( <cSaveState> ) --> NIL
*/
int
clip_MRESTSTATE(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   int      *save;

   char     *s;

   int       l;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   s = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!s || l != (7 * sizeof(int)))
      return EG_ARG;

   save = (int *) s;

   sp->mouse_visible = save[0];
   sp->mouse_y = save[1];
   sp->mouse_x = save[2];
   sp->mouse_top = save[3];
   sp->mouse_left = save[4];
   sp->mouse_bottom = save[5];
   sp->mouse_right = save[6];

   return 0;
}

/*
	   MDBLCLK([<nNewSpeed>]) --> nSpeed
	   note: really not work
*/
int
clip_MDBLCLK(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_dclick_speed);

   if (ClipMachineMemory->argc > 0)
      sp->mouse_dclick_speed = _clip_parni(ClipMachineMemory, 1);

   return 0;
}

/*
	   MSETPOS( <nRow>, <nCol> ) --> NIL
*/
int
clip_MSETPOS(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   sp->mouse_y = _clip_parni(ClipMachineMemory, 1);
   sp->mouse_x = _clip_parni(ClipMachineMemory, 2);

   return 0;
}

/*
	   MSTATE() --> aState | 0
*/
int
clip_MSTATE(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   long      dim;

   ClipVar  *rp, var;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   if (!sp->mouse_present)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }

   dim = 11;

   rp = RETPTR(ClipMachineMemory);
   _clip_array(ClipMachineMemory, rp, 1, &dim);

   CLEAR_CLIPVAR(&var);

  /* LLM_STATE_X */
   _clip_var_num(sp->mouse_x, &var);
   dim = 0;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_Y */
   _clip_var_num(sp->mouse_y, &var);
   dim = 1;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_ROW */
   _clip_var_num(sp->mouse_y, &var);
   dim = 2;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_COL */
   _clip_var_num(sp->mouse_x, &var);
   dim = 3;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_LEFT */
   _clip_var_num((sp->mouse_buttons & MOUSE_BUTTONS_LEFT) ? 1 : 0, &var);
   dim = 4;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_RIGHT */
   _clip_var_num((sp->mouse_buttons & MOUSE_BUTTONS_RIGHT) ? 1 : 0, &var);
   dim = 5;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_VISIBLE */
   _clip_var_num(sp->mouse_visible, &var);
   dim = 6;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_DRIVER */
   _clip_var_str(sp->mouse_driver, strlen(sp->mouse_driver), &var);
   dim = 7;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_SHAPE */
   _clip_var_num(1, &var);
   dim = 8;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_CLICKS_LEFT */
   _clip_var_num(sp->mouse_clicks_left, &var);
   sp->mouse_clicks_left = 0;
   dim = 9;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

  /* LLM_STATE_CLICKS_RIGHT */
   _clip_var_num(sp->mouse_clicks_right, &var);
   sp->mouse_clicks_right = 0;
   dim = 10;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   return 0;
}

int
clip_ISCOLOR(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);
   _clip_retl(ClipMachineMemory, iscolor_tty(ClipMachineMemory->screen->base));
   return 0;
}

int
clip_CENTER(ClipMachine * ClipMachineMemory)
{
   int       l1, l2, nl, i;

   unsigned char *ret, *beg, *end, fillchr = ' ';

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   int       len = _clip_parni(ClipMachineMemory, 2);

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l2);

   int       flag = _clip_parl(ClipMachineMemory, 4);

   if (str == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CENTER");
    }
   if (s != NULL)
      fillchr = *s;
   if (_clip_parinfo(ClipMachineMemory, 3) == LOGICAL_type_of_ClipVarType)
    {
       flag = _clip_parl(ClipMachineMemory, 3);
       s = (unsigned char *) (" ");
    }
   if (len == 0)
    {
       len = 80;
       _clip_fullscreen(ClipMachineMemory);
       if (ClipMachineMemory->fullscreen)
	{
	   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	   len = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
	}
    }
   ret = malloc(len + 1);
   for (beg = str; beg < str + l1 && (*beg == ' ' || *beg == '\t'); beg++);
   for (end = str + l1 - 1; end > beg && (*end == ' ' || *end == '\t'); end--);
   end++;
   nl = (len - (end - beg)) / 2;
   for (i = 0; i < nl; i++)
      ret[i] = fillchr;
   for (; beg < end; beg++, i++)
      ret[i] = *beg;
   if (flag)
    {
       for (; i < len; i++)
	  ret[i] = fillchr;
    }
   ret[i] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, i);
   return 0;
}

typedef struct
{
   int       hi;
   int       wi;

   char    **chars;
   char    **colors;
   char    **attrs;
}
WinBuf;

static Coll *winbufs = 0;

static WinBuf *new_WinBuf(int hi, int wi);

static void destroy_WinBuf(void *item);

static WinBuf *find_WinBuf(int no);

/*
   buf = winbuf_create(height,width) - ����������� height x width
	   (����� ��� ����� ���� ���).
*/
int
clip_WINBUF_CREATE(ClipMachine * ClipMachineMemory)
{
   int       height, width, i;

   WinBuf   *wb;

   if (ClipMachineMemory->argc < 2)
      return EG_ARG;

   height = _clip_parni(ClipMachineMemory, 1);
   width = _clip_parni(ClipMachineMemory, 2);

   wb = new_WinBuf(height, width);

   if (!winbufs)
      winbufs = new_Coll(destroy_WinBuf, 0);

   for (i = 0; i < winbufs->count; i++)
    {
       if (!winbufs->items[i])
	{
	   winbufs->items[i] = wb;
	   break;
	}
    }

   if (i == winbufs->count)
      append_Coll(winbufs, wb);

   _clip_retni(ClipMachineMemory, i);

   return 0;
}

int
clip_WINBUF_DESTROY(ClipMachine * ClipMachineMemory)
{
   int       no;

   WinBuf   *wp;

   if (ClipMachineMemory->argc < 1)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);

   if (!wp)
      return EG_ARG;

   destroy_WinBuf(wp);
   winbufs->items[no] = 0;

   return 0;
}

static WinBuf *
new_WinBuf(int hi, int wi)
{
   WinBuf   *wb;

   int       size, y, x;

   char     *mem, *chars, *colors, *attrs;

   size = sizeof(WinBuf) + hi * wi * 3 + sizeof(char *) * hi * 3;
   mem = (char *) calloc(size, 1);
   wb = (WinBuf *) mem;

   wb->hi = hi;
   wb->wi = wi;

   chars = (char *) (mem + sizeof(WinBuf));
   colors = chars + wi * hi;
   attrs = colors + wi * hi;
   wb->chars = (char **) (attrs + wi * hi);
   wb->colors = (char **) (attrs + wi * hi + sizeof(char *) * hi);
   wb->attrs = (char **) (attrs + wi * hi + sizeof(char *) * hi * 2);

   for (y = 0; y < hi; y++)
    {
       wb->chars[y] = chars + wi * y;
       wb->colors[y] = colors + wi * y;
       wb->attrs[y] = attrs + wi * y;

       for (x = 0; x < wi; x++)
	{
	   wb->chars[y][x] = ' ';
	   wb->colors[y][x] = 7;
	   wb->attrs[y][x] = 0;
	}
    }

   return wb;
}

static void
destroy_WinBuf(void *item)
{
   free(item);
}

static WinBuf *
find_WinBuf(int no)
{
   if (!winbufs)
      return 0;

   if (no >= 0 && no < winbufs->count)
    {
       return (WinBuf *) winbufs->items[no];
    }

   return 0;
}

/*
  winbuf_out_at(buf,top,left,str,color) - ������� str ��������  ������top,left ��� color. �������� � ���� ���.
*/
int
clip_WINBUF_OUT_AT(ClipMachine * ClipMachineMemory)
{
   int       no;

   WinBuf   *wp;

   int       top, left;

   char     *str;

   int       slen;

   ClipVar  *vp;

   int       attr;

   if (ClipMachineMemory->argc < 5)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);

   if (!wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);
   str = _clip_parcl(ClipMachineMemory, 4, &slen);

   if (!str)
      return EG_ARG;

   vp = _clip_par(ClipMachineMemory, 5);
   if (vp)
    {
       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	  attr = _clip_parni(ClipMachineMemory, 5);
       else
	  attr = get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 5));
    }
   else
      attr = _clip_colorSelect(ClipMachineMemory);

   if (top >= 0 && top < wp->hi)
    {
       int       y = top;

       int       x, i;

       for (x = left, i = 0; x < wp->wi && i < slen; i++, x++)
	{
	   int       ch;

	   ch = ClipMachineMemory->term_xlat[((unsigned char *) str)[i]];

	   if (ch < 32 && !is_pgch(ch))
	    {
	       wp->chars[y][x] = ch + 64;
	       wp->colors[y][x] = attr | 0x8;
	       wp->attrs[y][x] = 0;
	    }
	   else
	    {
	       wp->chars[y][x] = ch;
	       wp->colors[y][x] = attr;
	       wp->attrs[y][x] = 0;
	    }
	}
    }

   return 0;
}

/*
winbuf_attr_at(buf,top,left,bottom,right,cAttr)
���� �������top,left,bottom,right ����� cAttr ���� buf.
*/

int
clip_WINBUF_ATTR_AT(ClipMachine * ClipMachineMemory)
{
   int       no;

   WinBuf   *wp;

   int       top, left, bottom, right;

   ClipVar  *vp;

   int       attr;

   int       y, x;

   if (ClipMachineMemory->argc < 5)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);

   if (!wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);
   bottom = _clip_parni(ClipMachineMemory, 4);
   right = _clip_parni(ClipMachineMemory, 5);

   vp = _clip_par(ClipMachineMemory, 6);
   if (vp)
    {
       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	  attr = _clip_parni(ClipMachineMemory, 6);
       else
	  attr = get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 6));
    }
   else
      attr = _clip_colorSelect(ClipMachineMemory);

   if (top < 0)
      top = 0;
   if (left < 0)
      left = 0;

   for (y = top; y <= bottom && y < wp->hi; y++)
    {
       for (x = left; x <= right && x < wp->wi; x++)
	{
	   wp->colors[y][x] = attr;
	}
    }

   return 0;
}

/*
  winbuf_copy(scr_buf,top1,left1,height1,width1,dest_buf,top2,left2) -
  ����� ������ scr_buf, ������ ������
  top1,left1,height1,width1 �dest_buf � ������top2,left2
*/
int
clip_WINBUF_COPY(ClipMachine * ClipMachineMemory)
{
   int       no;

   WinBuf   *wp, *d_wp;

   int       top, left, hi, wi, d_top, d_left;

   int       x, y, d_y, d_x;

   if (ClipMachineMemory->argc < 6)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);
   if (!wp)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 6);
   d_wp = find_WinBuf(no);
   if (!d_wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);

   if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType)
      hi = _clip_parni(ClipMachineMemory, 4);
   else
      hi = wp->hi;
   if (hi > wp->hi)
      hi = wp->hi;

   if (_clip_parinfo(ClipMachineMemory, 5) == NUMERIC_type_of_ClipVarType)
      wi = _clip_parni(ClipMachineMemory, 5);
   else
      wi = wp->wi;
   if (wi > wp->wi)
      wi = wp->wi;

   d_top = _clip_parni(ClipMachineMemory, 7);
   d_left = _clip_parni(ClipMachineMemory, 8);

   for (y = top, d_y = d_top; y < hi && d_y < d_wp->hi; y++, d_y++)
    {
       if (y < 0 || d_y < 0)
	  continue;
       for (x = left, d_x = d_left; x < wi && d_x < d_wp->wi; x++, d_x++)
	{
	   if (x < 0 || d_x < 0)
	      continue;
	   d_wp->chars[d_y][d_x] = wp->chars[y][x];
	   d_wp->colors[d_y][d_x] = wp->colors[y][x];
	   d_wp->attrs[d_y][d_x] = wp->attrs[y][x];
	}
    }

   return 0;
}

/*
  winbuf_display(buf,top,left[,r_top,r_left[,r_height,r_width]]) -
	������ ���buf ������ ������top,left. �� ����	r_top,r_left,r_height,r_width - ������ � buf, ���� ����
	������ ���������. ���� r_top,r_left ����� �top,left
*/

int
clip_WINBUF_DISPLAY(ClipMachine * ClipMachineMemory)
{
   int       no;

   WinBuf   *wp;

   int       top, left;

   int       r_top, r_left, r_bottom, r_right;

   int       d_top, d_left, d_hi, d_wi;

   int       x, y, xi, yi;

   Screen   *sp;

   if (ClipMachineMemory->argc < 3)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);
   if (!wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);

   if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType)
      d_top = _clip_parni(ClipMachineMemory, 4);
   else
      d_top = 0;
   if (d_top < 0)
      d_top = 0;

   if (_clip_parinfo(ClipMachineMemory, 5) == NUMERIC_type_of_ClipVarType)
      d_left = _clip_parni(ClipMachineMemory, 5);
   else
      d_left = 0;
   if (d_left < 0)
      d_left = 0;

   if (_clip_parinfo(ClipMachineMemory, 6) == NUMERIC_type_of_ClipVarType)
      d_hi = _clip_parni(ClipMachineMemory, 6);
   else
      d_hi = wp->hi;
   if (d_hi > wp->hi)
      d_hi = wp->hi;

   if (_clip_parinfo(ClipMachineMemory, 7) == NUMERIC_type_of_ClipVarType)
      d_wi = _clip_parni(ClipMachineMemory, 7);
   else
      d_wi = wp->wi;
   if (d_wi > wp->wi)
      d_wi = wp->wi;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   r_top = top;
   r_left = left;
   r_bottom = r_top + d_hi;
   r_right = r_left + d_wi;

   clip_region(ClipMachineMemory, &r_top, &r_left, &r_bottom, &r_right, 0, -1);

   for (y = top, yi = d_top; yi < d_hi; ++y, ++yi)
    {
       if (y < r_top || y > r_bottom)
	  continue;
       sp->touched[y] = 1;
       for (x = left, xi = d_left; xi < d_wi; ++x, ++xi)
	{
	   if (x < r_left || x > r_right)
	      continue;
	   sp->chars[y][x] = wp->chars[yi][xi];
	   sp->colors[y][x] = wp->colors[yi][xi];
	   sp->attrs[y][x] = wp->attrs[yi][xi];
	}
    }

   sync_mp(ClipMachineMemory);

   return 0;
}

/*
  winbuf_out_trans(buf,top,left,str,color) - output "transparent" text
  2003/08/21, Mauricio Abre
*/
int
clip_WINBUF_OUT_TRANS(ClipMachine * ClipMachineMemory)
{
   int       no;

   WinBuf   *wp;

   int       top, left;

   char     *str;

   int       slen;

   ClipVar  *vp;

   int       attr;

   if (ClipMachineMemory->argc < 5)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);

   if (!wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);
   str = _clip_parcl(ClipMachineMemory, 4, &slen);

   if (!str)
      return EG_ARG;

   vp = _clip_par(ClipMachineMemory, 5);
   if (vp)
    {
       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	  attr = _clip_parni(ClipMachineMemory, 5);
       else
	  attr = get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 5));
    }
   else
      attr = _clip_colorSelect(ClipMachineMemory);

   if (top >= 0 && top < wp->hi)
    {
       int       y = top;

       int       x, i;

       for (x = left, i = 0; x < wp->wi && i < slen; i++, x++)
	{
	   int       ch;

	   ch = ClipMachineMemory->term_xlat[((unsigned char *) str)[i]];

	   if (ch < 32 && !is_pgch(ch))
	    {
	       wp->chars[y][x] = ch + 64;
	       wp->colors[y][x] = attr | 0x8;
	       wp->attrs[y][x] = 0;
	    }
	   else
	    {
	       wp->chars[y][x] = ch;
	       wp->colors[y][x] = (wp->colors[y][x] & 0xf0) | (attr & 0x0f);
	       wp->attrs[y][x] = 0;
	    }
	}
    }

   return 0;
}

int
clip_GETMEMBUF(ClipMachine * ClipMachineMemory)
{
   ClipVar  *rp = RETPTR(ClipMachineMemory);

   if (ClipMachineMemory->pbuf)
      _clip_var_str(ClipMachineMemory->pbuf->buf_of_OutBuf,
		    ClipMachineMemory->pbuf->ptr_of_OutBuf - ClipMachineMemory->pbuf->buf_of_OutBuf, rp);
   return 0;
}

int
clip_SCREENCHAR(ClipMachine * ClipMachineMemory)
{
   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   Screen   *sp = ClipMachineMemory->screen;

   char     *r = calloc(1, 2);

   if ((_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType) || (top < 0) || (top >= sp->base->Lines))
      top = sp->y;
   if ((_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType) || (left < 0) || (left >= sp->base->Columns))
      left = sp->x;

   r[0] = sp->chars[top][left];
   _clip_retcn_m(ClipMachineMemory, r, 1);
   return 0;
}

int
clip_SCREENATTR(ClipMachine * ClipMachineMemory)
{
   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   Screen   *sp = ClipMachineMemory->screen;

   int       r;

   _clip_retni(ClipMachineMemory, 0);
   if (!ClipMachineMemory->fullscreen)
      return 0;

   if ((_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType) || (top < 0) || (top >= sp->base->Lines))
      top = sp->y;
   if ((_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType) || (left < 0) || (left >= sp->base->Columns))
      left = sp->x;

   r = (int) sp->colors[top][left];
   _clip_retni(ClipMachineMemory, r);
   return 0;
}

int
clip_SCREENSTR(ClipMachine * ClipMachineMemory)
{
   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   int       l = _clip_parni(ClipMachineMemory, 3);

   Screen   *sp = ClipMachineMemory->screen;

   char     *r, *p;

   int       i, b;

#ifdef CLIP_DOS_SCRBUF
   b = 2;
#else
   b = 3;
#endif

   if ((_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType) || (top < 0) || (top >= sp->base->Lines))
      top = sp->y;
   if ((_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType) || (left < 0) || (left >= sp->base->Columns))
      left = sp->x;
   if ((_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType) || (l < 1) || (left + l > sp->base->Columns))
      l = sp->base->Columns - left;

   r = calloc(1, l * b + 1);

   for (i = 0, p = r; i < l; i++)
    {
       *p++ = sp->chars[top][left + i];
       *p++ = sp->colors[top][left + i];
#ifndef CLIP_DOS_SCRBUF
       *p++ = sp->attrs[top][left + i];
#endif
    }
   _clip_retcn_m(ClipMachineMemory, r, l * b);
   return 0;
}

int
clip_SCREENSTRING(ClipMachine * ClipMachineMemory)
{
   int       top = _clip_parni(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   int       l = _clip_parni(ClipMachineMemory, 3);

   Screen   *sp = ClipMachineMemory->screen;

   char     *r;

   if ((_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType) || (top < 0) || (top >= sp->base->Lines))
      top = sp->y;
   if ((_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType) || (left < 0) || (left >= sp->base->Columns))
      left = sp->x;
   if ((_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType) || (l < 1) || (left + l > sp->base->Columns))
      l = sp->base->Columns - left;

   r = calloc(1, l + 1);
   memcpy(r, sp->chars[top] + left, l);
   _clip_retcn_m(ClipMachineMemory, r, l);
   return 0;
}
