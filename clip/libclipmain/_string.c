/*
	Copyright (C) 2001  ITK
	Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>
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
#include <sys/types.h>

#include "ci_clip.h"
#include "ci_error.ch"
#include "ci_rational.h"
#include "ci_hash.h"

#include "ci_koi2alt.h"
#include "ci_koi2win.h"
#include "ci_alt2koi.h"
#include "ci_win2koi.h"

#define CS_KOI	1
#define CS_ALT	2
#define CS_WIN	3
#define CS_MAC	4

#define NEW(type)	((type*)calloc(sizeof(type),1))

const char *
_atl(const char *src, const char *dst, int slen, int dlen)
{
   const char *end, *de, *s = src, *d = dst;

   for (end = src + slen, de = dst + dlen - 1; src < end; src++)
      if (*src == *d)
       {
	  s = src;
	  for (; src < end && d < de && *(++src) == *(++d););
	  if (d == de && *src == *d)
	     return s;
	  src = s;
	  d = dst;
       }
   return 0;
}

const char *
_atr(const char *src, const char *dst, int slen, int dlen)
{
   const char *s = src + slen - 1, *d = dst + dlen - 1, *ss = s;

   for (; s >= src; s--)
      if (*s == *d)
       {
	  ss = s;
	  for (; s >= src && d >= dst && *(--s) == *(--d););
	  if (d == dst - 1)
	     return s + 1;
	  s = ss;
	  d = dst + dlen - 1;
       }
   return 0;
}

int
clip_ALLTRIM(ClipMachine * ClipMachineMemory)
{
   int       vl = 0, rl = 0;

   char     *rp, *e;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
      //return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ALLTRIM");
    }
   rp = (char *) malloc(vl + 1);
   for (e = vp; e < vp + vl && (*e == ' ' || *e == '\t'); e++);
   rl = vl - (e - vp);
   memcpy(rp, e, rl);
   for (e = rp + rl - 1; e >= rp && (*e == ' ' || *e == '\t'); e--, rl--);
   rp = (char *) realloc(rp, rl + 1);
   rp[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, rl);
   return 0;
}

int
clip_LTRIM(ClipMachine * ClipMachineMemory)
{
   int       vl = 0, rl = 0;

   char     *rp, *e, *end;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "LTRIM");
    }
   for (e = vp, end = vp + vl; e < end && (*e == ' ' || *e == '\t'); e++);
   rl = vl - (e - vp);
   rp = (char *) malloc(rl + 1);
   memcpy(rp, e, rl);
   rp[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, rl);
   return 0;
}

int
clip_RTRIM(ClipMachine * ClipMachineMemory)
{
   int       vl = 0, rl = 0;

   char     *rp, *e;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RTRIM");
    }
   for (e = vp + vl - 1; e >= vp && (*e == ' ' || *e == '\t'); e--);
   rl = e - vp + 1;
   rp = (char *) malloc(rl + 1);
   memcpy(rp, vp, rl);
   rp[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, rl);
   return 0;
}

int
clip_TRIM(ClipMachine * ClipMachineMemory)
{
   return clip_RTRIM(ClipMachineMemory);
}

int
clip_AT(ClipMachine * ClipMachineMemory)
{
   int       i = 1, sl, l, ll;

   const char *ss;

   char     *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char     *str = _clip_parcl(ClipMachineMemory, 2, &l);

   int       kol = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL || s == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "AT");
    }
   if (kol <= 0)
      kol = 1;
   ss = str;
   while (1)
    {
       ll = l - (ss - str);
       ss = _atl(ss, s, ll, sl);
       if (i == kol && ss != NULL)
	{
	   _clip_retni(ClipMachineMemory, ss - str + 1);
	   break;
	}
       if (ss == NULL)
	{
	   _clip_retni(ClipMachineMemory, 0);
	   break;
	}
       i++;
       ss++;

    }
   return 0;
}

int
clip_ATL(ClipMachine * ClipMachineMemory)
{
   return clip_AT(ClipMachineMemory);
}

int
clip_RAT(ClipMachine * ClipMachineMemory)
{
   int       sl, l, ll, i = 1;

   const char *ss;

   char     *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char     *str = _clip_parcl(ClipMachineMemory, 2, &l);

   int       kol = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL || s == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RAT");
    }
   if (kol <= 0)
      kol = 1;
   ss = str + l;
   while (1)
    {
       ll = ss - str;
       ss = _atr(str, s, ll, sl);
       if (i == kol && ss != NULL)
	{
	   _clip_retni(ClipMachineMemory, ss - str + 1);
	   break;
	}
       if (ss == NULL)
	{
	   _clip_retni(ClipMachineMemory, 0);
	   break;
	}
       i++;
       ss--;

    }
   return 0;
}

int
clip_ATR(ClipMachine * ClipMachineMemory)
{
   return clip_RAT(ClipMachineMemory);
}

int
clip_UAT(ClipMachine * ClipMachineMemory)
{
   int       sl, l, ll;

   char     *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char     *str = _clip_parcl(ClipMachineMemory, 2, &l);

   int       from = _clip_parni(ClipMachineMemory, 3);

   int       to = _clip_parni(ClipMachineMemory, 4);

   int       direct = _clip_parni(ClipMachineMemory, 5);

   const char *ss = NULL;

   if (str == NULL || s == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RAT");
    }

   if (from <= 0)
      from = 1;
   from--;

   if (to > l || to <= 0)
      to = l;
   to--;

   if (direct >= 0)
      ss = _atl(str + from, s, to - from + 1, sl);
   else
      ss = _atr(str + from, s, to - from + 1, sl);

   ll = ss - str;
   if (ss == NULL || (ll + sl) > (to + 1))
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, ss - str + 1);

   return 0;
}

int
clip_ATALPHA(ClipMachine * ClipMachineMemory)
{
   int       i, sl;

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &sl);

   if (s == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }
   for (i = 0; i < sl && ((s[i] > ' ' && s[i] <= '~') || (s[i] >= '!' && s[i] <= '9') || s[i] == '_' || s[i] == '>'); i++);

   if (i < sl)
      _clip_retni(ClipMachineMemory, i + 1);
   else
      _clip_retni(ClipMachineMemory, 0);

   return 0;
}

int
clip_STRFINDEOL(ClipMachine * ClipMachineMemory)
{
   int       i, sl;

   char     *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   int       begpos = _clip_parni(ClipMachineMemory, 2);

   if (s == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }
   if (begpos < 1)
      begpos = 1;
   for (i = begpos - 1; i < sl; i++)
    {
       if (s[i] == '\n')
	{
	   i++;
	   break;
	}
    }
   _clip_retni(ClipMachineMemory, i + 1);
   return 0;
}

int
clip_STRFINDBOL(ClipMachine * ClipMachineMemory)
{
   int       i, sl;

   char     *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   int       begpos = _clip_parni(ClipMachineMemory, 2);

   if (s == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }
   if (begpos < 1 || begpos > sl)
      begpos = sl + 1;

   begpos--;
   if (s[begpos] == '\n')
      begpos--;
   for (i = begpos; i >= 0; i--)
    {
       if (s[i] == '\n')
	{
	   i++;
	   break;
	}
    }
   _clip_retni(ClipMachineMemory, i + 1);
   return 0;
}

int
clip_BIN2I(ClipMachine * ClipMachineMemory)
{
   int       vl = 0;

   short     ret;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BIN2I");
    }
   ret = *((short *) vp);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_I2BIN(ClipMachine * ClipMachineMemory)
{
   char      buf[3];

   char     *e;

   short     i = (short) _clip_parni(ClipMachineMemory, 1);

   e = (char *) (&i);
   buf[0] = e[0];
   buf[1] = e[1];
   buf[2] = 0;
   _clip_retcn(ClipMachineMemory, buf, 2);
   return 0;
}

int
clip_BIN2B(ClipMachine * ClipMachineMemory)
{
   int       vl = 0;

   char      ret;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BIN2I");
    }
   ret = *((char *) vp);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_B2BIN(ClipMachine * ClipMachineMemory)
{
   char      buf[2];

   char      i = (char) _clip_parni(ClipMachineMemory, 1);

   buf[0] = i;
   buf[2] = 0;
   _clip_retcn(ClipMachineMemory, buf, 1);
   return 0;
}

int
clip_L2BIN(ClipMachine * ClipMachineMemory)
{
   char      buf[5];

   char     *e;

   long      i = (long) _clip_parnl(ClipMachineMemory, 1);

   e = (char *) (&i);
   buf[0] = e[0];
   buf[1] = e[1];
   buf[2] = e[2];
   buf[3] = e[3];
   buf[4] = 0;
   _clip_retcn(ClipMachineMemory, buf, 4);
   return 0;
}

int
clip_BIN2L(ClipMachine * ClipMachineMemory)
{
   int       vl = 0;

   long      ret;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BIN2L");
    }
   ret = *((long *) vp);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_BIN2W(ClipMachine * ClipMachineMemory)
{
   int       vl = 0;

   unsigned short ret;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BIN2W");
    }
   ret = *((unsigned short *) vp);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_BIN2DW(ClipMachine * ClipMachineMemory)
{
   int       vl = 0;

   unsigned long ret;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BIN2L");
    }
   ret = *((unsigned long *) vp);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_HARDCR(ClipMachine * ClipMachineMemory)
{
   int       vl = 0;

   char     *ret, *e;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "HARDCR");
    }
   ret = malloc(vl + 1);
   memcpy(ret, vp, vl);
   for (e = ret; e < ret + vl; e++)
      if (*e == (char) 141)
	 *e = '\n';
   ret[vl] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, vl);
   return 0;
}

int
_clip_isalpha(unsigned char ch)
{
#if 0
   int       ret = 0;

   if (ch >= 'A' && ch <= 'Z')
      ret = 1;
   if (ch >= 'a' && ch <= 'z')
      ret = 1;
   if (ch > 127)
      ret = _clip_isalpha_tbl[ch - 128];
   return ret;
#else
   return _clip_isalpha_tbl[ch];
#endif
}

int
clip_ISALPHA(ClipMachine * ClipMachineMemory)
{
   char     *vp = _clip_parc(ClipMachineMemory, 1);

   _clip_retl(ClipMachineMemory, 0);
   if (vp != NULL)
      _clip_retl(ClipMachineMemory, _clip_isalpha(vp[0]));
   return 0;
}

int
clip_ISDIGIT(ClipMachine * ClipMachineMemory)
{
   char      ch;

   int       ret = 0;

   char     *vp = _clip_parc(ClipMachineMemory, 1);

   _clip_retl(ClipMachineMemory, 0);
   if (vp == NULL)
      return 0;
   ch = vp[0];
   if (ch >= '0' && ch <= '9')
      ret = 1;
   _clip_retl(ClipMachineMemory, ret > 0);
   return 0;
}

static void
translate(unsigned char *str, int len, int up)
{
   unsigned char *tbl = (up ? _clip_uptbl : _clip_lowtbl);

   while (len)
    {
       int       s = *str;

       *str = tbl[s];
       --len;
       ++str;
    }
}

int
clip_UPPER(ClipMachine * ClipMachineMemory)
{
   int       l;

   char     *s;

   char     *r;

   s = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!s)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }

   r = _clip_memdup(s, l);
   translate((unsigned char *) r, l, 1);

   _clip_retcn_m(ClipMachineMemory, r, l);

   return 0;
}

int
clip_UPPE(ClipMachine * ClipMachineMemory)
{
   return clip_UPPER(ClipMachineMemory);
}

int
clip_XUPPER(ClipMachine * ClipMachineMemory)
{
   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
    {
       int       l;

       char     *s;

       char     *r;

       s = _clip_parcl(ClipMachineMemory, 1, &l);
       if (!s)
	{
	   _clip_retc(ClipMachineMemory, "");
	   return 0;
	}

       r = _clip_memdup(s, l);
       translate((unsigned char *) r, l, 1);

       _clip_retcn_m(ClipMachineMemory, r, l);
    }
   else
    {
       ClipVar  *r = RETPTR(ClipMachineMemory);

       _clip_dup(ClipMachineMemory, r, _clip_par(ClipMachineMemory, 1));
    }

   return 0;
}

int
clip_LOWER(ClipMachine * ClipMachineMemory)
{
   int       l;

   char     *s;

   char     *r;

   s = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!s)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }

   r = _clip_memdup(s, l);
   translate((unsigned char *) r, l, 0);

   _clip_retcn_m(ClipMachineMemory, r, l);

   return 0;
}

int
clip_LOWE(ClipMachine * ClipMachineMemory)
{
   return clip_LOWER(ClipMachineMemory);
}

int
clip_ISUPPER(ClipMachine * ClipMachineMemory)
{
   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   if (!s)
      _clip_retl(ClipMachineMemory, 0);
   else
    {
       int       ch = *s;

       _clip_retl(ClipMachineMemory, (_clip_isalpha_tbl[ch] && ch == _clip_uptbl[ch]));
      /* _clip_retl(ClipMachineMemory, (ch == _clip_uptbl[ch])); */
    }

   return 0;
}

int
clip_ISLOWER(ClipMachine * ClipMachineMemory)
{
   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   if (!s)
      _clip_retl(ClipMachineMemory, 0);
   else
    {
       int       ch = *s;

       _clip_retl(ClipMachineMemory, (_clip_isalpha_tbl[ch] && ch == _clip_lowtbl[ch]));
		/*_clip_retl(ClipMachineMemory, (ch == _clip_lowtbl[ch]));*/
    }

   return 0;
}

int
clip_ISWORD(ClipMachine * ClipMachineMemory)
{
   int       ret = 1;

   unsigned char *ptr;

   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   if (!s || !(*s))
    {
       _clip_retl(ClipMachineMemory, 0);
       return 0;
    }
   for (ptr = s; (*ptr); ptr++)
    {
       switch (*ptr)
	{
	case '0'...'9':
	   if (ptr == s)
	      ret = 0;
	   break;
	case 'a'...'z':
	case 'A'...'Z':
	case '_':
	   break;
	default:
	   ret = 0;
	   break;
	}
       if (!ret)
	  break;
    }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}

int
clip_MAX(ClipMachine * ClipMachineMemory)
{
   int       t1 = _clip_parinfo(ClipMachineMemory, 1);

   int       t2 = _clip_parinfo(ClipMachineMemory, 1);

   if (t1 == NUMERIC_type_of_ClipVarType && t2 == NUMERIC_type_of_ClipVarType)
    {
       double    n1, n2;

       int       l1, l2, d1, d2;

       n1 = _clip_parnd(ClipMachineMemory, 1);
       _clip_parp(ClipMachineMemory, 1, &l1, &d1);
       n2 = _clip_parnd(ClipMachineMemory, 2);
       _clip_parp(ClipMachineMemory, 2, &l2, &d2);
       if (n1 > n2)
	  _clip_retndp(ClipMachineMemory, n1, l1, d1);
       else
	  _clip_retndp(ClipMachineMemory, n2, l2, d2);
       return 0;
    }
   if (t1 == DATE_type_of_ClipVarType && t2 == DATE_type_of_ClipVarType)
    {
       long      ret, n1, n2;

       n1 = _clip_pardj(ClipMachineMemory, 1);
       n2 = _clip_pardj(ClipMachineMemory, 2);
       if (n1 > n2)
	  ret = n1;
       else
	  ret = n2;
       _clip_retdj(ClipMachineMemory, ret);
       return 0;
    }
   _clip_retni(ClipMachineMemory, 0);
   return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MAX");
}

int
clip_MIN(ClipMachine * ClipMachineMemory)
{
   int       t1 = _clip_parinfo(ClipMachineMemory, 1);

   int       t2 = _clip_parinfo(ClipMachineMemory, 1);

   if (t1 == NUMERIC_type_of_ClipVarType && t2 == NUMERIC_type_of_ClipVarType)
    {
       double    n1, n2;

       int       l1, l2, d1, d2;

       n1 = _clip_parnd(ClipMachineMemory, 1);
       _clip_parp(ClipMachineMemory, 1, &l1, &d1);
       n2 = _clip_parnd(ClipMachineMemory, 2);
       _clip_parp(ClipMachineMemory, 2, &l2, &d2);
       if (n1 < n2)
	  _clip_retndp(ClipMachineMemory, n1, l1, d1);
       else
	  _clip_retndp(ClipMachineMemory, n2, l2, d2);
       return 0;
    }
   if (t1 == DATE_type_of_ClipVarType && t2 == DATE_type_of_ClipVarType)
    {
       long      ret, n1, n2;

       n1 = _clip_pardj(ClipMachineMemory, 1);
       n2 = _clip_pardj(ClipMachineMemory, 2);
       if (n1 < n2)
	  ret = n1;
       else
	  ret = n2;
       _clip_retdj(ClipMachineMemory, ret);
       return 0;
    }
   _clip_retni(ClipMachineMemory, 0);
   return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MIN");
}

int
clip_MEMOLINE(ClipMachine * ClipMachineMemory)
{
   int       vl = 0, i, j, len, nstr, ntab, wrap;

   char     *rp, *e, *e2, *wrappos = NULL;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MEMOLINE");
    }
   len = _clip_parni(ClipMachineMemory, 2);
   if (len <= 4)
      len = 79;
   nstr = _clip_parni(ClipMachineMemory, 3);
   if (nstr <= 1)
      nstr = 1;
   ntab = _clip_parni(ClipMachineMemory, 4);
   if (ntab < 1)
      ntab = 1;
   if (ntab >= len)
      ntab = len - 1;

   if (_clip_parinfo(ClipMachineMemory, 5) == LOGICAL_type_of_ClipVarType)
      wrap = _clip_parl(ClipMachineMemory, 5);
   else
      wrap = 1;
   rp = (char *) malloc(len + 1);
   memset(rp, ' ', len);
   rp[len] = 0;

   for (i = 1, j = 1, e = vp; e < vp + vl && i < nstr; e++, j++)
    {
       if (*e == ' ')
	  wrappos = e;
       if (j >= len || *e == '\n')
	{
	   if (wrap && wrappos != NULL && e < vp + vl - 1 && *e != '\n')
	    {
	       if (*(e + 1) == ' ')
		  e++;
	       else
		  e = wrappos;
	    }
	   i++;
	   j = 0;
	}
       if (*e == '\t')
	  j += ntab - 1;
       if (*e == '\r')
	  j--;
    }
   if (i < nstr)
    {
       _clip_retcn_m(ClipMachineMemory, rp, len);
       return 0;
    }
   if (i > 1 && j > len)
    {
       if (wrap && wrappos != NULL)
	  e = wrappos + 1;
       else
	  e++;
    }
   wrappos = NULL;
   e2 = rp;
   for (j = 0; e < vp + vl && j <= len && *e != '\n'; e++, j++, e2++)
    {
       if (*e == ' ')
	  wrappos = e2;
       if (*e != '\r')
	  *e2 = *e;
       else
	{
	   j--;
	   e2--;
	}
       if (*e == '\t')
	  j += ntab - 1;
    }
   if (j > len)
    {
       if (wrap && wrappos != NULL)
	  e2 = wrappos + 1;
       else
	  e2++;
    }
   e = rp + len;
   for (; e2 <= e; e2++)
      *e2 = ' ';
   rp[len] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, len);
   return 0;
}

int
clip_MLCOUNT(ClipMachine * ClipMachineMemory)
{
   int       vl = 0, i, j, len, ntab, wrap;

   char     *e, *wrappos = NULL;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }
   len = _clip_parni(ClipMachineMemory, 2);
   if (len <= 4)
      len = 79;
   ntab = _clip_parni(ClipMachineMemory, 3);
   if (ntab < 1)
      ntab = 1;
   if (ntab >= len)
      ntab = len - 1;
   if (_clip_parinfo(ClipMachineMemory, 5) == LOGICAL_type_of_ClipVarType)
      wrap = _clip_parl(ClipMachineMemory, 5);
   else
      wrap = 1;

   for (i = 0, j = 1, e = vp; e < vp + vl; e++, j++)
    {
       if (*e == ' ')
	  wrappos = e;
       if (j >= len || *e == '\n')
	{
	   if (wrap && wrappos != NULL && e < vp + vl - 1 && *e != '\n')
	    {
	       if (*(e + 1) == ' ')
		  e++;
	       else
		{
		   e = wrappos;
		   wrappos = NULL;
		}
	    }
	   i++;
	   j = 0;
	}
       if (*e == '\t')
	  j += ntab - 1;
       if (*e == '\r')
	  j--;
    }
   _clip_retni(ClipMachineMemory, i + (j > 1));
   return 0;
}

int
clip_MLPOS(ClipMachine * ClipMachineMemory)
{
   int       vl = 0, i, j, len, nstr, ntab, npos = 1, wrap;

   char     *e, *wrappos = NULL;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MLPOS");
    }
   len = _clip_parni(ClipMachineMemory, 2);
   if (len <= 4)
      len = 79;
   nstr = _clip_parni(ClipMachineMemory, 3);
   if (nstr <= 1)
      nstr = 1;
   ntab = _clip_parni(ClipMachineMemory, 4);
   if (ntab < 1)
      ntab = 1;
   if (ntab >= len)
      ntab = len - 1;
   if (_clip_parinfo(ClipMachineMemory, 5) == LOGICAL_type_of_ClipVarType)
      wrap = _clip_parl(ClipMachineMemory, 5);
   else
      wrap = 1;

   for (i = 1, j = 1, e = vp; e < vp + vl; e++, j++)
    {
       if (*e == ' ')
	  wrappos = e;
       if (j >= len || *e == '\n')
	{
	   if (wrap && wrappos != NULL && e < vp + vl - 1 && *e != '\n')
	    {
	       if (*(e + 1) == ' ')
		  e++;
	       else
		  e = wrappos;
	    }
	   i++;
	   j = 0;
	}
       if (*e == '\t')
	  j += ntab - 1;
       if (*e == '\r')
	  j--;
       if (i >= nstr && j >= npos)
	  break;
    }
   _clip_retni(ClipMachineMemory, e - vp + 1);
   return 0;
}

int
clip_MLCTOPOS(ClipMachine * ClipMachineMemory)
{
   int       vl = 0, i, j, len, nstr, ntab, npos;

   char     *e;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MLTOPOS");
    }
   len = _clip_parni(ClipMachineMemory, 2);
   if (len <= 4)
      len = 79;
   nstr = _clip_parni(ClipMachineMemory, 3);
   if (nstr <= 1)
      nstr = 1;
   npos = _clip_parni(ClipMachineMemory, 4);
   if (npos <= 1)
      npos = 0;
   npos++;
   ntab = _clip_parni(ClipMachineMemory, 5);
   if (ntab < 1)
      ntab = 1;
   if (ntab >= len)
      ntab = len - 1;

   for (i = 1, j = 1, e = vp; e < vp + vl; e++, j++)
    {
       if (j >= len || *e == '\n')
	{
	   i++;
	   j = 0;
	}
       if (*e == '\t')
	  j += ntab - 1;
       if (*e == '\r')
	  j--;
       if (i >= nstr && j >= npos)
	  break;
    }
  /*printf("%d,%d,%d,%d...",i,j,e,vp); */
   _clip_retni(ClipMachineMemory, e - vp + 1);
   return 0;
}

int
clip_PADR(ClipMachine * ClipMachineMemory)
{
   int       bl, i;

   int       len = _clip_parni(ClipMachineMemory, 2);

   char     *s = _clip_parc(ClipMachineMemory, 3);

   char     *buf, *ret, ch;

   int       t1 = _clip_parinfo(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   if (len < 0)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }

   if (s == NULL)
      ch = ' ';
   else
      ch = s[0];
   switch (t1)
    {
    case NUMERIC_type_of_ClipVarType:
       buf = _clip_strFunc(ClipMachineMemory, vp, len, vp->ClipType_t_of_ClipVar.dec_of_ClipType, 2);
       bl = strlen(buf);
       break;
    case DATE_type_of_ClipVarType:
       buf = _clip_date_to_str(_clip_pardj(ClipMachineMemory, 1), ClipMachineMemory->date_format);
       bl = strlen(buf);
       break;
    case CHARACTER_type_of_ClipVarType:
       buf = _clip_parcl(ClipMachineMemory, 1, &bl);
       break;
    default:
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }
   ret = malloc(len + 1);
   for (i = 0; i < bl && i < len; i++)
      ret[i] = buf[i];
   for (; i < len; i++)
      ret[i] = ch;
   ret[len] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, len);

   if (t1 != CHARACTER_type_of_ClipVarType)
      free(buf);
   return 0;
}

int
clip_PAD(ClipMachine * ClipMachineMemory)
{
   return clip_PADR(ClipMachineMemory);
}

int
clip_PADL(ClipMachine * ClipMachineMemory)
{
   int       bl, i, j;

   int       len = _clip_parni(ClipMachineMemory, 2);

   char     *s = _clip_parc(ClipMachineMemory, 3);

   char     *buf, *ret, ch;

   int       t1 = _clip_parinfo(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   if (len < 0)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }

   if (s == NULL)
      ch = ' ';
   else
      ch = s[0];
   switch (t1)
    {
    case NUMERIC_type_of_ClipVarType:
       buf = _clip_strFunc(ClipMachineMemory, vp, len, vp->ClipType_t_of_ClipVar.dec_of_ClipType, 2);
       bl = strlen(buf);
       break;
    case DATE_type_of_ClipVarType:
       buf = _clip_date_to_str(_clip_pardj(ClipMachineMemory, 1), ClipMachineMemory->date_format);
       bl = strlen(buf);
       break;
    case CHARACTER_type_of_ClipVarType:
       buf = _clip_parcl(ClipMachineMemory, 1, &bl);
       break;
    default:
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }
   ret = malloc(len + 1);
   for (i = 0; i < len - bl; i++)
      ret[i] = ch;
   for (j = 0; i < len && j < bl; i++, j++)
      ret[i] = buf[j];
   ret[len] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, len);

   if (t1 != CHARACTER_type_of_ClipVarType)
      free(buf);
   return 0;
}

int
clip_PADC(ClipMachine * ClipMachineMemory)
{
   int       bl, i, j, k;

   int       len = _clip_parni(ClipMachineMemory, 2);

   char     *s = _clip_parc(ClipMachineMemory, 3);

   char     *buf, *ret, ch;

   int       t1 = _clip_parinfo(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   if (len < 0)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }

   if (s == NULL)
      ch = ' ';
   else
      ch = s[0];
   switch (t1)
    {
    case NUMERIC_type_of_ClipVarType:
       buf = _clip_strFunc(ClipMachineMemory, vp, len, vp->ClipType_t_of_ClipVar.dec_of_ClipType, 3);
       bl = strlen(buf);
       break;
    case DATE_type_of_ClipVarType:
       buf = _clip_date_to_str(_clip_pardj(ClipMachineMemory, 1), ClipMachineMemory->date_format);
       bl = strlen(buf);
       break;
    case CHARACTER_type_of_ClipVarType:
       buf = _clip_parcl(ClipMachineMemory, 1, &bl);
       break;
    default:
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }
   ret = malloc(len + 1);
   k = (len - bl) / 2;
   for (i = 0; i < k && i < len; i++)
      ret[i] = ch;
   for (j = 0; j < bl && i < len; i++, j++)
      ret[i] = buf[j];
   for (; i < len; i++)
      ret[i] = ch;
   ret[len] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, len);

   if (t1 != CHARACTER_type_of_ClipVarType)
      free(buf);
   return 0;
}

int
clip_REPLICATE(ClipMachine * ClipMachineMemory)
{
   int       i, k, j, vl, len;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   int       kol = _clip_parni(ClipMachineMemory, 2);

   char     *ret;

   if (kol <= 0)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }

   if (vp == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "REPLICATE");
    }
   len = vl * kol;
   ret = malloc(len + 1);
   for (i = 0, k = 0; i < kol; i++)
      for (j = 0; j < vl; j++, k++)
	 ret[k] = vp[j];
   ret[len] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, len);

   return 0;
}

int
clip_REPL(ClipMachine * ClipMachineMemory)
{
   return clip_REPLICATE(ClipMachineMemory);
}

int
clip_REPLI(ClipMachine * ClipMachineMemory)
{
   return clip_REPLICATE(ClipMachineMemory);
}

int
clip_REPLIC(ClipMachine * ClipMachineMemory)
{
   return clip_REPLICATE(ClipMachineMemory);
}

int
clip_REPLICA(ClipMachine * ClipMachineMemory)
{
   return clip_REPLICATE(ClipMachineMemory);
}

int
clip_REPLICAT(ClipMachine * ClipMachineMemory)
{
   return clip_REPLICATE(ClipMachineMemory);
}

int
clip_LEFT(ClipMachine * ClipMachineMemory)
{
   int       vl = 0;

   int       i;

   char     *rp;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   int       nl = _clip_parni(ClipMachineMemory, 2);

   _clip_retc(ClipMachineMemory, "");
   if (vp == NULL)
      return 0;
   if (nl < 0)
      nl = 0;
   if (nl > vl)
      nl = vl;

   rp = malloc(nl + 1);
   if (nl > vl)
      nl = vl;
   for (i = 0; i < nl; i++)
      rp[i] = vp[i];
   rp[i] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, nl);
   return 0;
}

int
clip_RIGHT(ClipMachine * ClipMachineMemory)
{
   int       vl = 0;

   int       i, j;

   char     *rp;

   char     *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   int       nl = _clip_parni(ClipMachineMemory, 2);

   _clip_retc(ClipMachineMemory, "");
   if (vp == NULL)
      return 0;
   if (nl < 0)
      nl = 0;
   if (nl > vl)
      nl = vl;
   rp = malloc(nl + 1);
   for (i = vl - nl, j = 0; i < vl; i++, j++)
      rp[j] = vp[i];
   rp[j] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, nl);
   return 0;
}

int
clip_SPACE(ClipMachine * ClipMachineMemory)
{
   int       kol = _clip_parni(ClipMachineMemory, 1);

   char     *ret;

   if (kol <= 0)
      kol = 0;
   ret = malloc(kol + 1);
   if (!_clip_parl(ClipMachineMemory, 2))
      memset(ret, ' ', kol);
   ret[kol] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, kol);
   return 0;
}

int
clip_SPAC(ClipMachine * ClipMachineMemory)
{
   return clip_SPACE(ClipMachineMemory);
}

int
clip_STRTRAN(ClipMachine * ClipMachineMemory)
{
   int       l, l1, l2, rl = 0, n;

   char     *ret, *e, *m, *end, *end1, *end2, *e1, *e2;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &l);

   char     *s1 = _clip_parcl(ClipMachineMemory, 2, &l1);

   char     *s2 = _clip_parcl(ClipMachineMemory, 3, &l2);

   int       from = _clip_parni(ClipMachineMemory, 4);

   int       kol = _clip_parni(ClipMachineMemory, 5);

   if (str == NULL || s1 == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STRTRAN");
    }
   if (s2 == NULL)
    {
       s2 = "";
       l2 = 0;
    }
   if (from <= 0)
      from = 1;
   if (kol <= 0)
      kol = 0;
   from--;
   end = str + l;
   end1 = s1 + l1;
   end2 = s2 + l2;
   if (kol == 0)
    {
       for (e = str + from; e < end; e++)
	{
	   for (e1 = s1, e2 = e; e1 < end1 && e2 < end && (*e1) == (*e2); e1++, e2++);
	   if (e1 == end1)
	    {
	       kol++;
	       e += l1 - 1;
	    }
	   if (e >= end)
	      break;
	}
    }
   if (kol == 0)
    {
       _clip_retcn(ClipMachineMemory, str, l);
       return 0;
    }
   rl = l + (l2 - l1) * kol + 1;
   ret = malloc(rl + 1);
   memcpy(ret, str, from);
   for (e = str + from, m = e, n = 0; e < end && n < kol; e++)
    {
       for (e1 = s1, e2 = e; e1 < end1 && e2 < end && (*e1) == (*e2); e1++, e2++);
       if (e1 != end1)
	  continue;
       memcpy(ret + from, m, e - m);
       from += e - m;
       memcpy(ret + from, s2, l2);
       from += l2;
       if (e1 == end1)
	{
	   e += l1 - 1;
	   m = e + 1;
	   n++;
	}
    }
   memcpy(ret + from, m, str + l - m);
   from += str + l - m;
   ret[from] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, from);
   return 0;
}

int
clip_STUFF(ClipMachine * ClipMachineMemory)
{
   int       l, l1, rl;

   char     *ret;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &l);

   int       from = _clip_parni(ClipMachineMemory, 2);

   int       kol = _clip_parni(ClipMachineMemory, 3);

   char     *s1 = _clip_parcl(ClipMachineMemory, 4, &l1);

   if (str == NULL || s1 == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STUFF");
    }
   if (from <= 0)
      from = 1;
   if (from > l)
      from = l + 1;
   if (kol <= 0)
      kol = 0;
   if ((from + kol) > l)
      kol = l - from + 1;
   from--;
   rl = l - kol + l1;
   if (rl <= 0)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }
   ret = malloc(rl + 1);
   memcpy(ret, str, from);
   memcpy(ret + from, s1, l1);
   memcpy(ret + from + l1, str + from + kol, l - from - kol);
   ret[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, rl);
   return 0;
}

int
clip_SUBSTR(ClipMachine * ClipMachineMemory)
{
  /* sorry test */
   int       l;

   char     *ret;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &l);

   int       from = _clip_parni(ClipMachineMemory, 2);

   int       kol = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SUBSTR");
    }
   if (kol <= 0)
      kol = 0;
   if (from < 0)
    {
       from = l + from;
       if (from < 0)
	  from = 0;
    }
   else if (from)
      from--;
   if (_clip_parinfo(ClipMachineMemory, 0) < 3)
      kol = l - from;
   if ((kol + from) > l)
      kol = l - from;
   if (kol < 0)
      kol = 0;
   ret = malloc(kol + 1);
   memcpy(ret, str + from, kol);
   ret[kol] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, kol);
   return 0;
}

int
clip_SUBS(ClipMachine * ClipMachineMemory)
{
   return clip_SUBSTR(ClipMachineMemory);
}

int
clip_SUBST(ClipMachine * ClipMachineMemory)
{
   return clip_SUBSTR(ClipMachineMemory);
}

int
clip_HASHSTR(ClipMachine * ClipMachineMemory)
{
   char     *s;

   long      hash;

   char     *str = _clip_parc(ClipMachineMemory, 1);

   if (str == NULL)
    {
       _clip_retnl(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "HASHSTR");
    }
   hash = _clip_hashstr(str);
   _clip_retnl(ClipMachineMemory, hash);

   s = (char *) HashTable_fetch(ClipMachineMemory->hashnames, hash);
   if (!s)
    {
       s = strdup(str);
       HashTable_store(ClipMachineMemory->hashnames, s, hash);
    }
   return 0;
}

static int
_clip_calc_crc(ClipMachine * ClipMachineMemory, int type)
{
   long      hash;

   int       len;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &len);

   if (str == NULL)
    {
       _clip_retnl(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "HASHSTR");
    }

   if (type == 32)
      hash = _clip_hashbytes32(0, str, len);
   else
      hash = _clip_hashbytes16(0, str, len);

   _clip_retnl(ClipMachineMemory, hash);

   return 0;
}

int
clip_CRC32(ClipMachine * ClipMachineMemory)
{
   return _clip_calc_crc(ClipMachineMemory, 32);
}

int
clip_CRC16(ClipMachine * ClipMachineMemory)
{
   return _clip_calc_crc(ClipMachineMemory, 16);
}

int
clip_HASHNAME(ClipMachine * ClipMachineMemory)
{
   long      hash = _clip_parnl(ClipMachineMemory, 1);

   char      buf[256];

   _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
   _clip_retc(ClipMachineMemory, buf);

   return 0;
}

int
clip_STRFORMAT(ClipMachine * ClipMachineMemory)
{
   int       i, len, nw, lw, n, c, c1;

   char     *str1;

   char     *str2 = _clip_parcl(ClipMachineMemory, 1, &len);

   int       lend = _clip_parni(ClipMachineMemory, 2);

   if (str2 == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STRFORMAT");
    }

   str1 = (char *) malloc(lend + 1);

   for (len--; str2[len] == ' '; len--);
   str2[len + 1] = 0;
   if (len > lend)
      str2[lend + 1] = 0;

   for (i = 0; str2[i] == ' '; i++);
   for (; str2[i] != ' ' && str2[i] != 0; i++);
   n = i;
   for (nw = 0, lw = 0; str2[i] != 0; nw++)
    {
       for (; str2[i] == ' '; i++);
       for (; str2[i] != ' ' && str2[i] != 0; i++, lw++);
    }
   if (nw != 0)
    {
       c = (lend - n - lw) / nw;
       c1 = lend - n - lw - c * nw;
    }
   else
    {
       c = lend - n;
       c1 = 0;
       nw = 1;
    }
   for (i = 0; i < n; i++)
      str1[i] = str2[i];
   for (; nw > 0; nw--)
    {
       for (lw = 0; lw < c; lw++, n++)
	  str1[n] = ' ';
       if (c1 > 0)
	{
	   str1[n] = ' ';
	   n++;
	   c1--;
	}
       for (; str2[i] == ' '; i++);
       for (; str2[i] != ' ' && str2[i] != 0; i++, n++)
	  str1[n] = str2[i];

    }
   str1[n] = 0;
   _clip_retcn_m(ClipMachineMemory, str1, n);
   return 0;
}

int
clip_STRUNFORMAT(ClipMachine * ClipMachineMemory)
{
   int       i, len, nw;

   char     *str1;

   char     *str2 = _clip_parcl(ClipMachineMemory, 1, &len);

   if (str2 == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STRUNFORMAT");
    }
   for (nw = 0; str2[nw] == ' '; nw++);
   for (i = 0; i < len;)
    {
       nw++;
       if (str2[i] == ' ')
	  while (str2[i] == ' ')
	     i++;
       else
	  i++;
    }
   str1 = (char *) malloc(nw + 1);

   for (i = 0, nw = 0; str2[i] == ' '; i++, nw++)
      str1[nw] = str2[i];
   for (; i < len;)
    {
       str1[nw] = str2[i];
       nw++;
       if (str2[i] == ' ')
	  while (str2[i] == ' ')
	     i++;
       else
	  i++;
    }
   str1[nw] = 0;
   _clip_retcn_m(ClipMachineMemory, str1, nw);
   return 0;
}

int
clip_OCCURS(ClipMachine * ClipMachineMemory)
{
   int       i = 0, sl, l, ll;

   const char *ss;

   char     *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char     *str = _clip_parcl(ClipMachineMemory, 2, &l);

   if (str == NULL || s == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "OCCURS");
    }
   ss = str;
   while (1)
    {
       ll = l - (ss - str);
       ss = _atl(ss, s, ll, sl);
       if (ss == NULL)
	  break;
       i++;
       ss++;

    }
   _clip_retni(ClipMachineMemory, i);
   return 0;
}

int
clip_DESCEND(ClipMachine * ClipMachineMemory)
{
   int       i, j, l, d;

   int       t = _clip_parinfo(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   unsigned char *buf, *str, ch;

   double    n;

   switch (t)
    {
    case MEMO_type_of_ClipVarType:
    case CHARACTER_type_of_ClipVarType:
       str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);
       buf = calloc(l + 1, 1);
       buf[l] = 0;
       for (i = 0; i < l; i++)
	{
	   ch = str[i];
	   ch = 256 - _clip_cmptbl[ch];
	   for (j = 0; j < 256; j++)
	      if (_clip_cmptbl[j] == ch)
		 break;
	   if (j < 256)
	      buf[i] = j;
	   else
	      buf[i] = ch;
	}
       _clip_retcn_m(ClipMachineMemory, (char *) buf, l);
       break;
    case DATE_type_of_ClipVarType:
       _clip_retnl(ClipMachineMemory, 5231808 - _clip_pardj(ClipMachineMemory, 1));
       break;
    case NUMERIC_type_of_ClipVarType:
       if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	{
	   _clip_retnr(ClipMachineMemory, rational_inv(vp->ClipRationalVar_r_of_ClipVar.rational_of_ClipRationalVar), l, d);
	}
       else
	{
	   n = _clip_parnd(ClipMachineMemory, 1);
	   _clip_parp(ClipMachineMemory, 1, &l, &d);
	   _clip_retndp(ClipMachineMemory, -1 * n, l, d);
	}
       break;
    case LOGICAL_type_of_ClipVarType:
       _clip_retl(ClipMachineMemory, !_clip_parl(ClipMachineMemory, 1));
       break;
    }
   return 0;
}

void
_clip_recodeString(char *str, int len, int sourceCharset, int targetCharset)
{
   int       i;

   unsigned char *s;

   if (sourceCharset == targetCharset)
      return;
   for (i = 0, s = (unsigned char *) str; i < len; ++i, ++s)
    {
       int       ch = *s;

       if (ch < 128)
	  continue;
       switch (sourceCharset)
	{
	case CS_KOI:
	   break;
	case CS_ALT:
	   ch = alt2koi[ch - 128];
	   break;
	case CS_WIN:
	   ch = win2koi[ch - 128];
	   break;
	}
       if (ch < 128)
	{
	   *s = ch;
	   continue;
	}
       switch (targetCharset)
	{
	case CS_KOI:
	   break;
	case CS_ALT:
	   ch = koi2alt[ch - 128];
	   break;
	case CS_WIN:
	   ch = koi2win[ch - 128];
	   break;
	}
       *s = ch;
    }
}

int
clip_ALT2KOI(ClipMachine * ClipMachineMemory)
{

   int       l;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   ret[l] = 0;
   _clip_recodeString((char *) ret, l, CS_ALT, CS_KOI);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}

int
clip_KOI2ALT(ClipMachine * ClipMachineMemory)
{

   int       l;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   ret[l] = 0;
   _clip_recodeString((char *) ret, l, CS_KOI, CS_ALT);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}

int
clip_ADDSLASH(ClipMachine * ClipMachineMemory)
{
   int       count = 0, i, l;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL || l == 0)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }
   for (i = 0; i < l; i++)
    {
       switch (str[i])
	{
	case '"':
	case '\'':
	case 0:
	   if (i == 0 || str[i - 1] != '\\')
	      count++;
	   break;
	}
    }
   ret = malloc(l + count + 1);
   for (count = 0, i = 0; i < l; i++, count++)
    {
       switch (str[i])
	{
	case '"':
	case '\'':
	case 0:
	   if (i == 0 || str[i - 1] != '\\')
	    {
	       ret[count] = '\\';
	       count++;
	    }
	   break;
	}
       ret[count] = str[i] ? str[i] : '0';
    }
   ret[count] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, count);
   return 0;
}

int
clip_SPLITDELIM(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = RETPTR(ClipMachineMemory);

   char     *str = _clip_parc(ClipMachineMemory, 1);

   char     *fdelim = _clip_parc(ClipMachineMemory, 2);

   char     *rdelim = _clip_parc(ClipMachineMemory, 3);

   char     *b, *p, f, r;
   long      dims[1] = { 0 };
   ClipVar   v;

   _clip_array(ClipMachineMemory, ap, 1, dims);

   if (!str || !str[0])
      return 0;
   if (!fdelim)
      f = '\"';
   else
      f = fdelim[0];
   if (!rdelim)
      r = ',';
   else
      r = rdelim[0];

   p = str;
   while (*p)
    {
       while (*p && *p == ' ')
	  p++;
       if (!(*p))
	  continue;
       b = p;
       if (*p == f)
	{
	   while (*p && *(++p) != f);
	   if (!(*p))
	      continue;
	   _clip_var_str(b + 1, p - b - 1, &v);
	   _clip_aadd(ClipMachineMemory, ap, &v);
	   while (*p && *(p++) != r);
	   continue;
	}
       while (*p && *p != r)
	  p++;
       _clip_var_str(b, p - b, &v);
       _clip_aadd(ClipMachineMemory, ap, &v);
       if (*p)
	  p++;
    }
   return 0;
}

int
clip_BASE64ENCODE(ClipMachine * ClipMachineMemory)
{
   int       inlen;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &inlen);

   const char base64char[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   char     *inp = str;

   int       outlen = 0;

   int       l = ((inlen + 2) / 3) * 4;

   char     *outp, *out;

   l += (inlen + 53) / 54;
   out = outp = calloc(1, l + 1);

   while (inlen >= 3)
    {
       *outp++ = base64char[(inp[0] >> 2) & 0x3f];
       *outp++ = base64char[((inp[0] & 0x03) << 4) | ((inp[1] >> 4) & 0x0f)];
       *outp++ = base64char[((inp[1] & 0x0f) << 2) | ((inp[2] >> 6) & 0x03)];
       *outp++ = base64char[inp[2] & 0x3f];

       inp += 3;
       inlen -= 3;
       if (++outlen == 18)
	{
	   *outp++ = 10;
	   outlen = 0;
	}
    }

   if (inlen > 0)
    {
       *outp++ = base64char[(inp[0] >> 2) & 0x3f];
       if (inlen == 1)
	{
	   *outp++ = base64char[(inp[0] & 0x03) << 4];
	   *outp++ = '=';
	}
       else
	{
	   *outp++ = base64char[((inp[0] & 0x03) << 4) | ((inp[1] >> 4) & 0x0f)];
	   *outp++ = base64char[((inp[1] & 0x0f) << 2)];
	}
       *outp++ = '=';
    }

   _clip_retcn_m(ClipMachineMemory, out, l);
   return 0;
}

int
clip_CHARSPLIT(ClipMachine * ClipMachineMemory)
{
   int       i = 1, sl, l, ll, kol;

   const char *ss;

   char     *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char     *str = _clip_parcl(ClipMachineMemory, 2, &l);

   ClipVar  *arr = RETPTR(ClipMachineMemory);

   long      ln[2];

   if (str == NULL || s == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHSPLIT");
    }

   ln[0] = 0;
   _clip_array(ClipMachineMemory, arr, 1, ln);

   ss = str;
   kol = 0;
   while (1)
    {
       ll = l - (ss - str);
       ss = _atl(ss, s, ll, sl);
       if (ss != NULL)
	{
	   ClipVar  *st;

	   st = NEW(ClipVar);
	   _clip_var_str(str + kol, ss - str - kol, st);
	   _clip_aadd(ClipMachineMemory, arr, st);

	   _clip_delete(ClipMachineMemory, st);

	   kol = ss - str + 1;
	}
       if (ss == NULL)
	{
	   ClipVar  *st;

	   st = NEW(ClipVar);
	   _clip_var_str(str + kol, l - kol, st);
	   _clip_aadd(ClipMachineMemory, arr, st);

	   _clip_delete(ClipMachineMemory, st);
	   break;
	}
       i++;
       ss++;

    }
   return 0;
}
