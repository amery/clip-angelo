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
#include <ci_clip.h>
#include <ci_error.ch>
#include <ci_rational.h>

#ifdef HAVE_FLOAT_H
#include <float.h>
#endif

#include <_math/clip_EXP.c>
#include <_math/clip_LOG.c>
#include <_math/clip_LOG10.c>
#include <_math/clip_POW.c>
#include <_math/clip_SQRT.c>
#include <_math/clip_INT.c>
#include <_math/clip_MOD.c>
#include <_math/clip_ROUND.c>
#include <_math/clip_ROUN.c>
#include <_math/clip_SIN.c>
#include <_math/clip_COS.c>
#include <_math/clip_TAN.c>
#include <_math/clip_ASIN.c>
#include <_math/clip_ACOS.c>
#include <_math/clip_ATAN.c>
#include <_math/clip_ATN2.c>
#include <_math/clip_ATAN2.c>
#include <_math/clip_ABS.c>
#include <_math/clip_CEILING.c>
#include <_math/clip_FLOOR.c>
#include <_math/clip_PI.c>
#include <_math/clip_SIGN.c>
#include <_math/clip_DTOR.c>
#include <_math/clip_COT.c>
#include <_math/clip_FACT.c>
#include <_math/clip_FV.c>
#include <_math/clip_GETPREC.c>
#include <_math/clip_SETPREC.c>
#include <_math/clip_RTOD.c>
#include <_math/clip_RANDOM.c>
#include <_math/clip_RAND.c>
#include <_math/clip_RANDOMIZE.c>
#include <_math/clip_BITTOC.c>
#include <_math/clip_CTOBIT.c>
#include <_math/clip_CELSIUS.c>
#include <_math/clip_FAHRENHEIT.c>
#include <_math/StrToInt.c>
#include <_math/clip_NUMNOT.c>
#include <_math/clip_NUMAND.c>
#include <_math/clip_NUMOR.c>
#include <_math/clip_NUMXOR.c>
#include <_math/clip_SETBIT.c>
#include <_math/clip_CLEARBIT.c>
#include <_math/clip_ISBIT.c>
#include <_math/clip_CTOF.c>
#include <_math/clip_FTOC.c>
#include <_math/clip_CTON.c>
#include <_math/clip_NTOC.c>
#include <_math/clip_INFINITY.c>
#include <_math/clip_EXPONENT.c>
#include <_math/clip_MANTISSA.c>
#include <_math/_clip_strtod_base.c>
#include <_math/clip_INTNEG.c>
#include <_math/clip_INTPOS.c>
#include <_math/clip_LTON.c>
#include <_math/clip_NUMHIGH.c>
#include <_math/clip_NUMLOW.c>
#include <_math/clip_NUMMIRR.c>
#include <_math/clip_NUMROL.c>
