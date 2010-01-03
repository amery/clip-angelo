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
#include <time.h>
#include <math.h>
#include "ci_clip.h"
#include "ci_error.ch"
#include "ci_rational.h"

#ifdef HAVE_FLOAT_H
#include <float.h>
#endif

int
clip_EXP(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (ClipMachineMemory->flags & FIXED_FLAG)
      dec = ClipMachineMemory->decimals;
   else
      dec *= 2;
   if (dec <= 0)
      dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, exp(d), len, dec);
   return 0;
}

int
clip_LOG(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (ClipMachineMemory->flags & FIXED_FLAG)
      dec = ClipMachineMemory->decimals;
   else
      dec *= 2;
   if (dec <= 0)
      dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, log(d), len, dec);
   return 0;
}

int
clip_LOG10(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (ClipMachineMemory->flags & FIXED_FLAG)
      dec = ClipMachineMemory->decimals;
   else
      dec *= 2;
   if (dec <= 0)
      dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, log10(d), len, dec);
   return 0;
}

int
clip_POW(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   double    d2 = _clip_parnd(ClipMachineMemory, 2);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (ClipMachineMemory->flags & FIXED_FLAG)
      dec = ClipMachineMemory->decimals;
   else
      dec *= 2;
   if (dec <= 0)
      dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, pow(d, d2), len, dec);
   return 0;
}

int
clip_SQRT(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (ClipMachineMemory->flags & FIXED_FLAG)
      dec = ClipMachineMemory->decimals;
   else
      dec *= 2;
   if (dec <= 0)
      dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, sqrt(d), len, dec);
   return 0;
}

int
clip_INT(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = 0;
#ifdef ARCH_i386
   (*(long long *) &d)++;
#endif
   _clip_retndp(ClipMachineMemory, d - fmod(d, 1.00), len, dec);
   return 0;
}

int
clip_MOD(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   double    d2 = _clip_parnd(ClipMachineMemory, 2);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (ClipMachineMemory->flags & FIXED_FLAG)
      dec = ClipMachineMemory->decimals;
   else
      dec *= 2;
   if (dec <= 0)
      dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, fmod(d, d2), len, dec);
   return 0;
}

int
clip_ROUND(ClipMachine * ClipMachineMemory)
{
#if 1
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   int       de = _clip_parni(ClipMachineMemory, 2);

#ifdef ARCH_i386
   (*(long long *) &d)++;
#endif

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = de;
   if (dec <= 0)
      dec = 0;
   if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
    {
       char     *tmp = rational_toString(vp->ClipRationalVar_r_of_ClipVar.rational_of_ClipRationalVar, 10, dec, 0);

       _clip_retnr(ClipMachineMemory, rational_fromString(tmp), len, dec);
       free(tmp);
       return 0;
    }
   if (d == 0.0)
    {
       _clip_retndp(ClipMachineMemory, d, len, de);
       return 0;
    }
   if (de == 0)
    {
       if (d < 0.0)
	  d = ceil(d - 0.5);
       else
	  d = floor(d + 0.5);
       _clip_retndp(ClipMachineMemory, d, len, dec);
       return 0;
    }
   if (de < 0)
    {
       double    tmp = pow(10, -de);

       if (d < 0.0)
	  d = ceil((d / tmp) - 0.5);
       else
	  d = floor((d / tmp) + 0.5);
       d *= tmp;
    }
   else
    {
       double    tmp = pow(10, de);

       if (d < 0.0)
	  d = ceil((d * tmp) - 0.5);
       else
	  d = floor((d * tmp) + 0.5);
       d /= tmp;
    }
   _clip_retndp(ClipMachineMemory, d, len, dec);
   return 0;
#else
   int       len, dec, len1;

   char      buf[33], ch;

   double    ret, d = _clip_parnd(ClipMachineMemory, 1);

   int       de = _clip_parni(ClipMachineMemory, 2);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);

   buf[32] = 0;
   if (len > 30)
      len = 30;
   _clip_dtostr(buf, len, de + 1, d, 0);
   len1 = strlen(buf) - 1;
   ch = buf[len1];
   if (ch >= '5')
      buf[len1 - 1]++;
   buf[len1] = 0;
   ret = _clip_strtod(buf, &dec);
   _clip_retndp(ClipMachineMemory, ret, len, de);
   return 0;
#endif
}

int
clip_ROUN(ClipMachine * ClipMachineMemory)
{
   return clip_ROUND(ClipMachineMemory);
}

int
clip_SIN(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, sin(d), len, dec);
   return 0;
}

int
clip_COS(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, cos(d), len, dec);
   return 0;
}

int
clip_TAN(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, tan(d), len, dec);
   return 0;
}

int
clip_ASIN(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, asin(d), len, dec);
   return 0;
}

int
clip_ACOS(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, acos(d), len, dec);
   return 0;
}

int
clip_ATAN(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, atan(d), len, dec);
   return 0;
}

int
clip_ATN2(ClipMachine * ClipMachineMemory)
{
   int       len1, dec1, len2, dec2;

   double    d1 = _clip_parnd(ClipMachineMemory, 1);

   double    d2 = _clip_parnd(ClipMachineMemory, 2);

   _clip_parp(ClipMachineMemory, 1, &len1, &dec1);
   _clip_parp(ClipMachineMemory, 2, &len2, &dec2);
   dec1 = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, atan2(d1, d2), len1, dec1);
   return 0;
}

int
clip_ATAN2(ClipMachine * ClipMachineMemory)
{
   int       len1, dec1, len2, dec2;

   double    d1 = _clip_parnd(ClipMachineMemory, 1);

   double    d2 = _clip_parnd(ClipMachineMemory, 2);

   _clip_parp(ClipMachineMemory, 1, &len1, &dec1);
   _clip_parp(ClipMachineMemory, 2, &len2, &dec2);
   dec1 = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, atan2(d1, d2), len1, dec1);
   return 0;
}

int
clip_ABS(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   _clip_retndp(ClipMachineMemory, fabs(d), len, dec);
   return 0;
}

int
clip_CEILING(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = 0;
   _clip_retndp(ClipMachineMemory, ceil(d), len, dec);
   return 0;
}

int
clip_FLOOR(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = 0;
   _clip_retndp(ClipMachineMemory, floor(d), len, dec);
   return 0;
}

int
clip_PI(ClipMachine * ClipMachineMemory)
{
   _clip_retndp(ClipMachineMemory, M_PI, 21, 19);
   return 0;
}

int
clip_SIGN(ClipMachine * ClipMachineMemory)
{
   int       len, dec, ret = 0;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   if (d > 0)
      ret = 1;
   if (d < 0)
      ret = -1;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_DTOR(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, M_PI * d / 180, len, dec);
   return 0;
}

int
clip_COT(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, 1 / tan(d), len, dec);
   return 0;
}

int
clip_FACT(ClipMachine * ClipMachineMemory)
{
   double    ret = 1;

   int       i;

   int       num = _clip_parni(ClipMachineMemory, 1);

   if (num > 21)
    {
       _clip_retni(ClipMachineMemory, -1);
       return 0;
    }
   for (i = 1; i <= num; i++)
      ret *= i;
   _clip_retndp(ClipMachineMemory, ret, num, 0);
   return 0;
}

int
clip_FV(ClipMachine * ClipMachineMemory)
{
   int       len, dec, i;

   double    ret;

   double    sum = _clip_parnd(ClipMachineMemory, 1);

   double    prec = 1.00 + _clip_parnd(ClipMachineMemory, 2);

   int       per = _clip_parni(ClipMachineMemory, 3);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   for (ret = sum, i = 0; i < per; i++)
      ret *= prec;
   _clip_retndp(ClipMachineMemory, ret, len, dec);
   return 0;
}

int
clip_GETPREC(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, 16);
   return 0;
}

int
clip_SETPREC(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, "");
   return 0;
}

int
clip_RTOD(ClipMachine * ClipMachineMemory)
{
   int       len, dec;

   double    d = _clip_parnd(ClipMachineMemory, 1);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = ClipMachineMemory->decimals;
   _clip_retndp(ClipMachineMemory, d * 180 / M_PI, len, dec);
   return 0;
}

int
clip_RANDOM(ClipMachine * ClipMachineMemory)
{
   long      ret = 0;

   long      d = _clip_parnl(ClipMachineMemory, 1);

   long      l = _clip_parl(ClipMachineMemory, 1);

   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      d = 0xFFFF;

   if (d != 0)
      ret = random() % d;
   else
      ret = random();
   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType && !l)
      ret -= d / 2;
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}

int
clip_RAND(ClipMachine * ClipMachineMemory)
{
   double    ret = ((double) random()) / 0x7FFFFFFF;

   if (ret < 0)
      ret = 0 - ret;
   _clip_retnd(ClipMachineMemory, ret);
   return 0;
}

int
clip_RANDOMIZE(ClipMachine * ClipMachineMemory)
{
   srandom(time(0));
   return 0;
}

/*
   #define clip_err(clipvar,iff,msg,retf,ret)   \
   if ((iff)) \
   { \
   _clip_trap_printf((clipvar),__FILE__,__LINE__,(msg)); \
   retf((clipvar),(ret)); \
   return 1; \
   }
 */

int
clip_BITTOC(ClipMachine * ClipMachineMemory)
{
   unsigned int Num = _clip_parni(ClipMachineMemory, 1);

   int       LenPattern;

   char     *BitPattern = _clip_parcl(ClipMachineMemory, 2, &LenPattern);

   char     *p, res[] = "                ";

   unsigned int Mask;

   int       Len = _clip_parinfo(ClipMachineMemory, 0), RealLen;

   int       t1 = _clip_parinfo(ClipMachineMemory, 1);

   int       t2 = _clip_parinfo(ClipMachineMemory, 2);

   if (Len < 2 || t1 != NUMERIC_type_of_ClipVarType || t2 != CHARACTER_type_of_ClipVarType)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BITTOC");
    }

   if (LenPattern > 16)
      LenPattern = 16;
   if (_clip_parl(ClipMachineMemory, 3))
    {
       for (RealLen = LenPattern, Len = 0, Mask = 1 << (LenPattern - 1); Len < LenPattern; Mask >>= 1, Len++)
	  if (Num & Mask)
	     res[Len] = BitPattern[Len];
    }
   else
    {
       for (RealLen = 0, p = res, Len = 0, Mask = 1 << (LenPattern - 1); Len < LenPattern; Mask >>= 1, Len++)
	  if (Num & Mask)
	   {
	      *p = BitPattern[Len];
	      p++;
	      RealLen++;
	   }
    }
   _clip_retcn(ClipMachineMemory, res, RealLen);
   return 0;
}

int
clip_CTOBIT(ClipMachine * ClipMachineMemory)
{
   int       LSel, LMask, i, j;

   char     *CSel = _clip_parcl(ClipMachineMemory, 1, &LSel);

   char     *CMask = _clip_parcl(ClipMachineMemory, 2, &LMask);

   unsigned int Res = 0;

   if (_clip_parinfo(ClipMachineMemory, 0) != 2 || CSel == NULL || CMask == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CTOBIT");
    }

   if (LSel > 16)
      LSel = 16;
   if (LMask > 16)
      LMask = 16;
   for (i = 0; i < LSel; i++)
    {
       for (j = 0; j < LMask && CSel[i] != CMask[j]; j++);
       if (j != LMask)
	  Res |= 1 << (LMask - j - 1);
    }
   _clip_retni(ClipMachineMemory, Res);
   return 0;
}

int
clip_CELSIUS(ClipMachine * ClipMachineMemory)
{
   _clip_retnd(ClipMachineMemory, (_clip_parnd(ClipMachineMemory, 1) - 32) * 5 / 9);
   return 0;
}

int
clip_FAHRENHEIT(ClipMachine * ClipMachineMemory)
{
   _clip_retnd(ClipMachineMemory, _clip_parnd(ClipMachineMemory, 1) * 9 / 5 + 32);
   return 0;
}

int
StrToInt(char *s, int len, int *res)
{
   unsigned int i;

   int       j;

   for (i = j = 0; j < len && j < sizeof(int) * 2; j++)

      switch (s[j])
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
	  i = (i << 4) + (s[j] - '0');
	  break;
       case 'A':
       case 'B':
       case 'C':
       case 'D':
       case 'E':
       case 'F':
	  i = (i << 4) + s[j] - 'A' + 10;
	  break;
       case 'a':
       case 'b':
       case 'c':
       case 'd':
       case 'e':
       case 'f':
	  i = (i << 4) + s[j] - 'a' + 10;
	  break;
       default:
	  if (j == 0)
	     return j + 1;
	  else
	   {
	      *res = i;
	      return 0;
	   }
       }
   *res = i;
   return 0;
}

int
clip_NUMNOT(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, ~_clip_parni(ClipMachineMemory, 1));
   return 0;
}

int
clip_NUMAND(ClipMachine * ClipMachineMemory)
{
   int       Num;

   int       i = _clip_parinfo(ClipMachineMemory, 0);

   int       SLen;

   char     *S;

   int       tmp;

   if (i < 2)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMAND");
    }
   switch (_clip_parinfo(ClipMachineMemory, 1))
    {
    case NUMERIC_type_of_ClipVarType:
       Num = _clip_parni(ClipMachineMemory, 1);
       break;
    case CHARACTER_type_of_ClipVarType:
       S = _clip_parcl(ClipMachineMemory, 1, &SLen);
       StrToInt(S, SLen, &Num);
       break;
    default:
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMAND");
    }
   for (; i > 1; i--)
    {
       switch (_clip_parinfo(ClipMachineMemory, i))
	{
	case NUMERIC_type_of_ClipVarType:
	   tmp = _clip_parni(ClipMachineMemory, i);
	   break;
	case CHARACTER_type_of_ClipVarType:
	   S = _clip_parcl(ClipMachineMemory, i, &SLen);
	   StrToInt(S, SLen, &tmp);
	   break;
	default:
	   _clip_retni(ClipMachineMemory, 0);
	   return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMAND");
	}
       Num &= tmp;
    }
   _clip_retni(ClipMachineMemory, Num);
   return 0;
}

int
clip_NUMOR(ClipMachine * ClipMachineMemory)
{
   int       Num;

   int       i = _clip_parinfo(ClipMachineMemory, 0);

   int       SLen;

   char     *S;

   int       tmp;

   if (i < 2)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMOR");
    }
   switch (_clip_parinfo(ClipMachineMemory, 1))
    {
    case NUMERIC_type_of_ClipVarType:
       Num = _clip_parni(ClipMachineMemory, 1);
       break;
    case CHARACTER_type_of_ClipVarType:
       S = _clip_parcl(ClipMachineMemory, 1, &SLen);
       StrToInt(S, SLen, &Num);
       break;
    default:
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMOR");
    }
   for (; i > 1; i--)
    {
       switch (_clip_parinfo(ClipMachineMemory, i))
	{
	case NUMERIC_type_of_ClipVarType:
	   tmp = _clip_parni(ClipMachineMemory, i);
	   break;
	case CHARACTER_type_of_ClipVarType:
	   S = _clip_parcl(ClipMachineMemory, i, &SLen);
	   StrToInt(S, SLen, &tmp);
	   break;
	default:
	   _clip_retni(ClipMachineMemory, 0);
	   return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMOR");
	}
       Num |= tmp;
    }
   _clip_retni(ClipMachineMemory, Num);
   return 0;
}

int
clip_NUMXOR(ClipMachine * ClipMachineMemory)
{
   int       Num1, Num2, SLen;

   char     *S;

   if (_clip_parinfo(ClipMachineMemory, 0) != 2)
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMXOR");
    }
   switch (_clip_parinfo(ClipMachineMemory, 1))
    {
    case NUMERIC_type_of_ClipVarType:
       Num1 = _clip_parni(ClipMachineMemory, 1);
       break;
    case CHARACTER_type_of_ClipVarType:
       S = _clip_parcl(ClipMachineMemory, 1, &SLen);
       StrToInt(S, SLen, &Num1);
       break;
    default:
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMXOR");
    }
   switch (_clip_parinfo(ClipMachineMemory, 2))
    {
    case NUMERIC_type_of_ClipVarType:
       Num2 = _clip_parni(ClipMachineMemory, 2);
       break;
    case CHARACTER_type_of_ClipVarType:
       S = _clip_parcl(ClipMachineMemory, 2, &SLen);
       StrToInt(S, SLen, &Num2);
       break;
    default:
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMXOR");
    }
   _clip_retni(ClipMachineMemory, Num1 ^ Num2);
   return 0;
}

int
clip_SETBIT(ClipMachine * ClipMachineMemory)
{
   int       Num, SLen;

   int       i = _clip_parinfo(ClipMachineMemory, 0), Bit;

   char     *S;

   if ((i == 1) || (i > 33))
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SETBIT");
    }
   switch (_clip_parinfo(ClipMachineMemory, 1))
    {
    case NUMERIC_type_of_ClipVarType:
       Num = _clip_parni(ClipMachineMemory, 1);
       break;
    case CHARACTER_type_of_ClipVarType:
       S = _clip_parcl(ClipMachineMemory, 1, &SLen);
       StrToInt(S, SLen, &Num);
       break;
    default:
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SETBIT");
    }
   for (; i > 1; i--)
    {
       Bit = _clip_parni(ClipMachineMemory, i);
       if (Bit > 0 && Bit <= 32)
	  Num |= 1 << (Bit - 1);
    }
   _clip_retni(ClipMachineMemory, Num);
   return 0;
}

int
clip_CLEARBIT(ClipMachine * ClipMachineMemory)
{
   int       Num, SLen;

   int       i = _clip_parinfo(ClipMachineMemory, 0), Bit;

   char     *S;

   if ((i == 1) || (i > 33))
    {
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CLEARBIT");
    }
   switch (_clip_parinfo(ClipMachineMemory, 1))
    {
    case NUMERIC_type_of_ClipVarType:
       Num = _clip_parni(ClipMachineMemory, 1);
       break;
    case CHARACTER_type_of_ClipVarType:
       S = _clip_parcl(ClipMachineMemory, 1, &SLen);
       StrToInt(S, SLen, &Num);
       break;
    default:
       _clip_retni(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CLEARBIT");
    }
   for (; i > 1; i--)
    {
       Bit = _clip_parni(ClipMachineMemory, i);
       if (Bit > 0 && Bit <= 32)
	  Num &= ~(1 << (Bit - 1));
    }
   _clip_retni(ClipMachineMemory, Num);
   return 0;
}

int
clip_ISBIT(ClipMachine * ClipMachineMemory)
{
   int       Bit, SLen;

   int       Num;

   int       res = 0;

   char     *S;

   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
    {
       _clip_retl(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ISBIT");
    }
   switch (_clip_parinfo(ClipMachineMemory, 1))
    {
    case NUMERIC_type_of_ClipVarType:
       Num = _clip_parni(ClipMachineMemory, 1);
       break;
    case CHARACTER_type_of_ClipVarType:
       S = _clip_parcl(ClipMachineMemory, 1, &SLen);
       StrToInt(S, SLen, &Num);
       break;
    default:
       _clip_retl(ClipMachineMemory, 0);
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ISBIT");
    }
   if (_clip_parinfo(ClipMachineMemory, 0) == 1)
    {
       res = (Num) ? 1 : 0;
    }
   else
    {
       Bit = _clip_parni(ClipMachineMemory, 2) - 1;
       if (Bit > 31 || Bit < 0)
	  Bit = 0;
       res = (Num & (1 << Bit)) ? 1 : 0;
    }
   _clip_retl(ClipMachineMemory, res);
   return 0;
}

int
clip_CTOF(ClipMachine * ClipMachineMemory)
{
   int       SLen;

   char     *S = _clip_parcl(ClipMachineMemory, 1, &SLen);

   _clip_retnd(ClipMachineMemory, *((double *) S));
   return 0;
}

int
clip_FTOC(ClipMachine * ClipMachineMemory)
{
   double    res = _clip_parnd(ClipMachineMemory, 1);

   _clip_retcn(ClipMachineMemory, (char *) (&res), 8);
   return 0;
}

int
clip_CTON(ClipMachine * ClipMachineMemory)
{
   long      ret = 0;

   char     *str = _clip_parc(ClipMachineMemory, 1);

   int       base = _clip_parni(ClipMachineMemory, 2);

   if (base <= 0 || base > 36)
      base = 10;
   if (str != NULL)
      ret = strtol(str, NULL, base);
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}

int
clip_NTOC(ClipMachine * ClipMachineMemory)
{
   int       i = 256, b, j, l;

   char     *ret, *buf, fill;

   static char base_sym[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

   unsigned long data = (unsigned long) _clip_parnl(ClipMachineMemory, 1);

   char     *sdata = _clip_parc(ClipMachineMemory, 1);

   int       base = _clip_parni(ClipMachineMemory, 2);

   int       len = _clip_parni(ClipMachineMemory, 3);

   char     *str = _clip_parc(ClipMachineMemory, 4);

   int       neg = 0;

   if (base <= 0 || base > 36)
      base = 10;
   if (sdata != NULL)
      data = strtol(sdata, NULL, 16);
   if (data < 0)
    {
       neg = 1;
       data = 0 - data;
    }
   fill = str == NULL ? ' ' : (*str);
   buf = malloc(i + 1);
   buf[i] = 0;
   memset(buf, fill, i);
   for (b = 1; i >= 0; i--, b++)
    {
       j = data % base;
       buf[i] = base_sym[j];
       data = data / base;
       if (data == 0)
	  break;
    }
   if (neg)
      i--;
   l = 256 - i + 1;
   if (len <= 0)
      len = l;
   ret = calloc(len + 1, 1);
   if (len < l)
    {
       memset(ret, '*', len);
       free(buf);
       _clip_retcn_m(ClipMachineMemory, ret, len);
       return 0;
    }
   memset(ret, fill, len);
   memcpy(ret + len - l, buf + i, l);
   if (neg)
    {
       if (fill == ' ')
	  ret[len - l] = '-';
       else
	  *ret = '-';

    }
   free(buf);
   _clip_retcn_m(ClipMachineMemory, ret, len);
   return 0;
}

int
clip_INFINITY(ClipMachine * ClipMachineMemory)
{
#ifdef FLT_MAX
#else
#ifdef MAXFLOAT
#define FLT_MAX MAXFLOAT
#else
#ifdef FLOATMAX
#define FLT_MAX FLOATMAX
#else
#define FLT_MAX 3.40282347e+38F
#endif
#endif
#endif
   _clip_retnd(ClipMachineMemory, FLT_MAX);
   return 0;
}

int
clip_EXPONENT(ClipMachine * ClipMachineMemory)
{
   int       ret = 0, flag = 0;

   double    data = fabs(_clip_parnd(ClipMachineMemory, 1));

   if (data == 0)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }
   if (data < 1)
    {
       flag = 1;
       data = 2 / data;
    }
   for (; data >= 2; ret++)
      data = data / 2;
   if (flag)
      ret = 0 - ret;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_MANTISSA(ClipMachine * ClipMachineMemory)
{
   int       flag_neg = 0, flag = 0;

   double    data = _clip_parnd(ClipMachineMemory, 1);

   if (data == 0)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }
   if (data < 0)
    {
       flag_neg = 1;
       data = fabs(data);
    }
   if (data < 1)
    {
       flag = 1;
       data = 2 / data;
    }
   for (; data >= 2;)
      data = data / 2;
   if (flag_neg)
      data = 0 - data;
   if (flag)
      data = 2 / data;
   _clip_retndp(ClipMachineMemory, data, 7, 5);
   return 0;
}

double
_clip_strtod_base(char *str, int base)
{
   int       i, val = 0;

   double    ret = 0, b;

   if (base <= 0 || base > 36)
      base = 10;
   for (i = 0; str[i] != 0; i++);
   for (i--, b = 1; i >= 0; i--, b *= base)
    {
       switch (str[i])
	{
	case '0'...'9':
	   val = str[i] - '0';
	   break;
	case 'a'...'z':
	   val = str[i] - 'a' + 10;
	   break;
	case 'A'...'Z':
	   val = str[i] - 'A' + 10;
	   break;
	default:
	   val = 0;
	}
       if (val < base)
	  ret += val * b;
       else
	  break;
    }
   return ret;
}

int
clip_INTNEG(ClipMachine * ClipMachineMemory)
{
   double    data = _clip_parnd(ClipMachineMemory, 1);

   char     *str = _clip_parc(ClipMachineMemory, 1);

   int       flag = _clip_parl(ClipMachineMemory, 2);

   if (str != NULL)
      data = (double) _clip_strtod_base(str, 16);
   if (data <= 0)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }
   if (flag)
      _clip_retnl(ClipMachineMemory, (unsigned long) data);
   else
      _clip_retnl(ClipMachineMemory, (short) data);
   return 0;
}

int
clip_INTPOS(ClipMachine * ClipMachineMemory)
{
   double    data = _clip_parnd(ClipMachineMemory, 1);

   char     *str = _clip_parc(ClipMachineMemory, 1);

   int       flag = _clip_parl(ClipMachineMemory, 2);

   if (str != NULL)
      data = (double) _clip_strtod_base(str, 16);
   if (flag)
      _clip_retnd(ClipMachineMemory, (unsigned long) data);
   else
      _clip_retnl(ClipMachineMemory, (unsigned short) data);
   return 0;
}

int
clip_LTON(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, _clip_parl(ClipMachineMemory, 1));
   return 0;
}

int
clip_NUMHIGH(ClipMachine * ClipMachineMemory)
{
   unsigned short data = _clip_parnl(ClipMachineMemory, 1);

   int       par = _clip_parinfo(ClipMachineMemory, 1);

   if (par != NUMERIC_type_of_ClipVarType && par != CHARACTER_type_of_ClipVarType)
    {
       _clip_retni(ClipMachineMemory, -1);
       return 0;
    }
   if (par == CHARACTER_type_of_ClipVarType)
      data = strtol(_clip_parc(ClipMachineMemory, 1), NULL, 16);
   _clip_retnl(ClipMachineMemory, data >> 8);
   return 0;
}

int
clip_NUMLOW(ClipMachine * ClipMachineMemory)
{
   unsigned short data = _clip_parnl(ClipMachineMemory, 1);

   int       par = _clip_parinfo(ClipMachineMemory, 1);

   if (par != NUMERIC_type_of_ClipVarType && par != CHARACTER_type_of_ClipVarType)
    {
       _clip_retni(ClipMachineMemory, -1);
       return 0;
    }
   if (par == CHARACTER_type_of_ClipVarType)
      data = strtol(_clip_parc(ClipMachineMemory, 1), NULL, 16);
   _clip_retnl(ClipMachineMemory, data & 0x00FF);
   return 0;
}

int
clip_NUMMIRR(ClipMachine * ClipMachineMemory)
{
   int       i;

   unsigned short ret = 0, tmp = 0;

   unsigned char bdata, bret = 0, btmp = 0;

   unsigned short data = _clip_parnl(ClipMachineMemory, 1);

   int       par = _clip_parinfo(ClipMachineMemory, 1);

   if (par != NUMERIC_type_of_ClipVarType && par != CHARACTER_type_of_ClipVarType)
    {
       _clip_retni(ClipMachineMemory, -1);
       return 0;
    }
   if (par == CHARACTER_type_of_ClipVarType)
      data = strtol(_clip_parc(ClipMachineMemory, 1), NULL, 16);
   if (_clip_parl(ClipMachineMemory, 2))
    {
       bdata = data & 0xFF;
       for (i = 0; i < 4; i++)
	{
	   btmp = (1 << i) & bdata;
	   btmp = btmp << (8 - 2 * i);
	   bret = bret | btmp;
	}
       for (i = 8; i > 3; i--)
	{
	   btmp = (1 << i) & bdata;
	   btmp = (btmp >> i) << (7 - i);
	   bret = bret | btmp;
	}
       _clip_retnl(ClipMachineMemory, bret);
       return 0;
    }
   else
    {
       for (i = 0; i < 8; i++)
	{
	   tmp = (1 << i) & data;
	   tmp = tmp << (15 - 2 * i);
	   ret = ret | tmp;
	}
       for (i = 16; i > 7; i--)
	{
	   tmp = (1 << i) & data;
	   tmp = (tmp >> i) << (15 - i);
	   ret = ret | tmp;
	}
       _clip_retnl(ClipMachineMemory, ret);
       return 0;
    }
   return 0;
}

int
clip_NUMROL(ClipMachine * ClipMachineMemory)
{
   unsigned long ret = 0, tmp = 0;

   unsigned short data = _clip_parnl(ClipMachineMemory, 1);

   int       left = _clip_parni(ClipMachineMemory, 2);

   int       par = _clip_parinfo(ClipMachineMemory, 1);

   if (par != NUMERIC_type_of_ClipVarType && par != CHARACTER_type_of_ClipVarType)
    {
       _clip_retni(ClipMachineMemory, -1);
       return 0;
    }
   if (par == CHARACTER_type_of_ClipVarType)
      data = strtol(_clip_parc(ClipMachineMemory, 1), NULL, 16);

   if (_clip_parl(ClipMachineMemory, 3))
    {
       left = left % 8;
       ret = data & 0xFF;
       ret = ret << left;
       tmp = (ret & 0x0F00) >> 8;
       ret = (ret & 0x00FF) | tmp;
       _clip_retnl(ClipMachineMemory, ret | (data & 0xFF00));
       return 0;
    }
   else
    {
       left = left % 16;
       ret = data;
       ret = ret << left;
       tmp = (ret & 0x00FF0000) >> 16;
       ret = (ret & 0xFFFF) | tmp;
       _clip_retnl(ClipMachineMemory, ret);
       return 0;
    }
   return 0;
}
