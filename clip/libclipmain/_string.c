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

#include <ci_clip.h>
#include <ci_error.ch>
#include <ci_rational.h>
#include <ci_hash.h>

#include <ci_koi2alt.h>
#include <ci_koi2win.h>
#include <ci_alt2koi.h>
#include <ci_win2koi.h>

#define CS_KOI	1
#define CS_ALT	2
#define CS_WIN	3
#define CS_MAC	4

#define NEW(type)	((type*)calloc(sizeof(type),1))

#include <_string/_atl.c>
#include <_string/_atr.c>
#include <_string/clip_ALLTRIM.c>
#include <_string/clip_LTRIM.c>
#include <_string/clip_RTRIM.c>
#include <_string/clip_TRIM.c>
#include <_string/clip_AT.c>
#include <_string/clip_ATL.c>
#include <_string/clip_RAT.c>
#include <_string/clip_ATR.c>
#include <_string/clip_UAT.c>
#include <_string/clip_ATALPHA.c>
#include <_string/clip_STRFINDEOL.c>
#include <_string/clip_STRFINDBOL.c>
#include <_string/clip_BIN2I.c>
#include <_string/clip_I2BIN.c>
#include <_string/clip_BIN2B.c>
#include <_string/clip_B2BIN.c>
#include <_string/clip_L2BIN.c>
#include <_string/clip_BIN2L.c>
#include <_string/clip_BIN2W.c>
#include <_string/clip_BIN2DW.c>
#include <_string/clip_HARDCR.c>
#include <_string/_clip_isalpha.c>
#include <_string/clip_ISALPHA.c>
#include <_string/clip_ISDIGIT.c>
#include <_string/static_translate.c>
#include <_string/clip_UPPER.c>
#include <_string/clip_UPPE.c>
#include <_string/clip_XUPPER.c>
#include <_string/clip_LOWER.c>
#include <_string/clip_LOWE.c>
#include <_string/clip_ISUPPER.c>
#include <_string/clip_ISLOWER.c>
#include <_string/clip_ISWORD.c>
#include <_string/clip_MAX.c>
#include <_string/clip_MIN.c>
#include <_string/clip_MEMOLINE.c>
#include <_string/clip_MLCOUNT.c>
#include <_string/clip_MLPOS.c>
#include <_string/clip_MLCTOPOS.c>
#include <_string/clip_PADR.c>
#include <_string/clip_PAD.c>
#include <_string/clip_PADL.c>
#include <_string/clip_PADC.c>
#include <_string/clip_REPLICATE.c>
#include <_string/clip_REPL.c>
#include <_string/clip_REPLI.c>
#include <_string/clip_REPLIC.c>
#include <_string/clip_REPLICA.c>
#include <_string/clip_REPLICAT.c>
#include <_string/clip_LEFT.c>
#include <_string/clip_RIGHT.c>
#include <_string/clip_SPACE.c>
#include <_string/clip_SPAC.c>
#include <_string/clip_STRTRAN.c>
#include <_string/clip_STUFF.c>
#include <_string/clip_SUBSTR.c>
#include <_string/clip_SUBS.c>
#include <_string/clip_SUBST.c>
#include <_string/clip_HASHSTR.c>
#include <_string/static__clip_calc_crc.c>
#include <_string/clip_CRC32.c>
#include <_string/clip_CRC16.c>
#include <_string/clip_HASHNAME.c>
#include <_string/clip_STRFORMAT.c>
#include <_string/clip_STRUNFORMAT.c>
#include <_string/clip_OCCURS.c>
#include <_string/clip_DESCEND.c>
#include <_string/_clip_recodeString.c>
#include <_string/clip_ALT2KOI.c>
#include <_string/clip_KOI2ALT.c>
#include <_string/clip_ADDSLASH.c>
#include <_string/clip_SPLITDELIM.c>
#include <_string/clip_BASE64ENCODE.c>
#include <_string/clip_CHARSPLIT.c>
