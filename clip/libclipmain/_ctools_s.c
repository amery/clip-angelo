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
#include <stdarg.h>
#include <errno.h>

#include <ci_screen.h>
#include <ci_error.ch>
#include <ci_hashcode.h>

#ifdef HAVE_SYS_IO_H
#include <sys/io.h>
#endif

#ifdef OS_FREEBSD
#include <machine/cpufunc.h>
#include <machine/sysarch.h>
#endif

#define  CLIP_TOKEN_CHARACTERS " .,:;!?\\/<<>>()^#&%+-*\t\n\r\0x0"
#define  CLIP_TOKEN_CHARS_NUM  26
#define  CLIP_TOKEN_MAXLEN     1024

/* static data for TOKEN functions INIT, NEXT, END, AT, SEP */

#include <_ctools_s/clip_INIT_CTOOLS_STRING_SETS.c>
#include <_ctools_s/clip_ADDASCII.c>
#include <_ctools_s/clip_CSETATMUPA.c>
#include <_ctools_s/clip_ASCIISUM.c>
#include <_ctools_s/clip_ASCPOS.c>
#include <_ctools_s/_clip_atmupa.c>
#include <_ctools_s/clip_AFTERATNUM.c>
#include <_ctools_s/clip_ATADJUST.c>
#include <_ctools_s/clip_ATNUM.c>
#include <_ctools_s/clip_ATREPL.c>
#include <_ctools_s/clip_BEFORATNUM.c>
#include <_ctools_s/clip_CHARADD.c>
#include <_ctools_s/clip_CHARAND.c>
#include <_ctools_s/clip_CHAREVEN.c>
#include <_ctools_s/clip_CHARLIST.c>
#include <_ctools_s/clip_CHARMIRR.c>
#include <_ctools_s/clip_CHARMIX.c>
#include <_ctools_s/clip_CHARNOLIST.c>
#include <_ctools_s/clip_CHARNOT.c>
#include <_ctools_s/clip_CHARODD.c>
#include <_ctools_s/clip_CHARONE.c>
#include <_ctools_s/clip_CHARONLY.c>
#include <_ctools_s/clip_CHAROR.c>
#include <_ctools_s/clip_CHARRELA.c>
#include <_ctools_s/clip_CHARRELREP.c>
#include <_ctools_s/clip_CHARREM.c>
#include <_ctools_s/clip_CHARREPL.c>

static ClipMachine *_clip_charsort_mp;

#include <_ctools_s/_clip_charsort.c>
#include <_ctools_s/clip_CHARSORT.c>
#include <_ctools_s/clip_CHARSPREAD.c>
#include <_ctools_s/clip_CHARSWAP.c>
#include <_ctools_s/clip_CHARXOR.c>
#include <_ctools_s/clip_CHECKSUM.c>
#include <_ctools_s/clip_COUNTLEFT.c>
#include <_ctools_s/clip_COUNTRIGHT.c>
#include <_ctools_s/clip_CSETREF.c>
#include <_ctools_s/clip_EXPAND.c>
#include <_ctools_s/clip_JUSTLEFT.c>
#include <_ctools_s/clip_JUSTRIGHT.c>
#include <_ctools_s/clip_LIKE.c>
#include <_ctools_s/clip_LTOC.c>
#include <_ctools_s/clip_DSTRTON.c>
#include <_ctools_s/clip_FSTRTON.c>
#include <_ctools_s/clip_MAXLINE.c>
#include <_ctools_s/clip_NUMAT.c>
#include <_ctools_s/clip_NUMLINE.c>

int clip_PADL(ClipMachine * ClipMachineMemory);

#include <_ctools_s/clip_PADLEFT.c>

int clip_PADR(ClipMachine * ClipMachineMemory);

#include <_ctools_s/clip_PADRIGHT.c>
#include <_ctools_s/clip_POSALPHA.c>
#include <_ctools_s/clip_POSCHAR.c>
#include <_ctools_s/clip_POSDEL.c>
#include <_ctools_s/clip_POSDIFF.c>
#include <_ctools_s/clip_POSEQUAL.c>
#include <_ctools_s/clip_POSINS.c>
#include <_ctools_s/clip_POSLOWER.c>
#include <_ctools_s/clip_POSUPPER.c>
#include <_ctools_s/clip_POSRANGE.c>
#include <_ctools_s/clip_POSREPL.c>
#include <_ctools_s/clip_RANGEREM.c>
#include <_ctools_s/clip_RANGEREPL.c>
#include <_ctools_s/clip_REMALL.c>
#include <_ctools_s/clip_REMLEFT.c>
#include <_ctools_s/clip_REMRIGHT.c>
#include <_ctools_s/clip_REPLALL.c>
#include <_ctools_s/clip_REPLLEFT.c>
#include <_ctools_s/clip_REPLRIGHT.c>
#include <_ctools_s/clip_SETATLIKE.c>
#include <_ctools_s/clip_STRSWAP.c>
#include <_ctools_s/clip_TABEXPAND.c>
#include <_ctools_s/clip_TABPACK.c>
#include <_ctools_s/clip_VALPOS.c>
#include <_ctools_s/clip_WORDONE.c>
#include <_ctools_s/clip_WORDONLY.c>
#include <_ctools_s/clip_WORDREPL.c>
#include <_ctools_s/clip_WORDSWAP.c>
#include <_ctools_s/clip_WORDTOCHAR.c>

// token family function

#include <_ctools_s/_clip_attoken.c>
#include <_ctools_s/clip_ATTOKEN.c>
#include <_ctools_s/clip_NUMTOKEN.c>
#include <_ctools_s/_clip_token_case.c>
#include <_ctools_s/clip_TOKENLOWER.c>
#include <_ctools_s/clip_TOKENUPPER.c>
#include <_ctools_s/clip_TOKENSEP.c>
#include <_ctools_s/clip_TOKENAT.c>
#include <_ctools_s/clip_TOKENINIT.c>
#include <_ctools_s/clip_SAVETOKEN.c>
#include <_ctools_s/clip_RESTTOKEN.c>
#include <_ctools_s/clip_TOKEN.c>

#define D20070313

#include <_ctools_s/clip_TOKENNEXT.c>
#include <_ctools_s/clip_TOKENEND.c>
#include <_ctools_s/clip_BLANK.c>
#include <_ctools_s/clip_COMPLEMENT.c>
#include <_ctools_s/clip_NUL.c>
#include <_ctools_s/clip_TOOLVER.c>
#include <_ctools_s/clip_NUMCOUNT.c>
#include <_ctools_s/clip_CSCOUNT.c>
#include <_ctools_s/clip_WEIGHTTABLE.c>
#include <_ctools_s/clip_WEIGHTCHR.c>
#include <_ctools_s/clip_WEIGHTASC.c>

#if 1
/*
   variante from Konstantin Sukhorabski <kos@walk.chernigov.ua>
*/
#define BYTE char
#define ULONG unsigned long
#define USHORT unsigned short

#define HB_LOBYTE( w )          ( ( BYTE ) ( (w) & 0xFF ) )
#define HB_HIBYTE( w )          ( ( BYTE ) ( ( ( USHORT ) ( w ) >> 8 ) & 0xFF ) )
#define HB_MKUSHORT( lo, hi )   ( ( USHORT ) ( ( ( USHORT ) ( hi ) ) << 8 ) | ( (lo) & 0xFF ) )

#include <_ctools_s/clip_CRYPT.c>

#else

#define BYTE char
#define ULONG unsigned int
#define USHORT unsigned short

#define HB_LOBYTE( w )          ( ( BYTE ) ( w ) )
#define HB_HIBYTE( w )          ( ( BYTE ) ( ( ( USHORT ) ( w ) >> 8 ) & 0xFF ) )
#define HB_MKUSHORT( lo, hi )   ( ( USHORT ) ( ( ( USHORT ) ( hi ) ) << 8 ) | ( lo ) )

#include <_ctools_s/clip_CRYPT_not1_.c>
#endif

#ifdef OS_LINUX
static int plevel = 0;
#endif

#include <_ctools_s/static_take_permissions.c>
#include <_ctools_s/static_m_out_portb.c>
#include <_ctools_s/static_m_in_portb.c>
#include <_ctools_s/static_m_out_portw.c>
#include <_ctools_s/static_m_in_portw.c>
#include <_ctools_s/clip_OUTBYTE.c>
#include <_ctools_s/clip_OUTWORD.c>
#include <_ctools_s/clip_INBYTE.c>
#include <_ctools_s/clip_INWORD.c>
#include <_ctools_s/_clip_setxlat.c>
#include <_ctools_s/_clip_getxlat.c>
#include <_ctools_s/clip_SETTXLAT.c>
#include <_ctools_s/clip_GETTXLAT.c>
#include <_ctools_s/clip_SETPXLAT.c>
#include <_ctools_s/clip_GETPXLAT.c>
