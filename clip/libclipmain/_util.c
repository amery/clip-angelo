/*
	Copyright (C) 2001  ITK
	Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <ci_clip.h>
#include <ci_clipcfg.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/time.h>
#ifdef OS_MINGW
#include <ltdl.h>
#else
#include <dlfcn.h>
#include <netinet/in.h>
#include <sys/resource.h>
#endif
#include <errno.h>

#include <ci_clip.h>
#include <ci_error.ch>
#include <ci_rational.h>
#include <ci_coll.h>
#include <ci_hashcode.h>
#include <../libclipscreen/ci_charset.h>
#include <../libclipscreen/ci_screen.h>
#include <../libcliptask/ci_task.h>

extern char *CLIPROOT;

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))

#ifndef RTLD_NOW
#define RTLD_NOW DL_LAZY
#endif
#ifndef RTLD_GLOBAL
#define RTLD_GLOBAL DL_LAZY
#endif

#if 0
static void
decount(ClipVar * vp)
{
   switch (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
   {
   case F_MPTR:
   case F_MREF:
      vp->ClipRefVar_p_of_ClipVar.vp->ClipType_t_of_ClipVar.count_of_ClipType--;
   default:
   }
}
#endif

#include <_util/clip_INIT_CLIPINIT.c>
#include <_util/static__clip_cygwinroot.c>
#include <_util/clip_CYGWINROOT.c>
#include <_util/clip_CLIPROOT.c>
#include <_util/clip_CLIP_HOSTCS.c>
#include <_util/clip_ERRORBLOCK.c>
#include <_util/clip_BREAK.c>
#include <_util/clip_DATATYPE.c>
#include <_util/clip_VALTYPE.c>
#include <_util/clip_TYPE.c>
#include <_util/clip_PROCNAME.c>
#include <_util/clip_PROCLINE.c>
#include <_util/clip_FILENAME.c>
#include <_util/clip_COMPILE.c>
#include <_util/BLOCK2STR.c>
#include <_util/clip_LEN.c>
#include <_util/clip_DEC.c>
#include <_util/clip_ARRAY.c>
#include <_util/clip_MAP.c>
#include <_util/clip_MAPKEYS.c>
#include <_util/clip_MAPMODIFY.c>
#include <_util/clip_EVAL.c>
#include <_util/clip_EVALA.c>
#include <_util/clip_MAPEVAL.c>
#include <_util/clip_LOAD.c>
#include <_util/clip_LOADBLOCK.c>
#include <_util/clip_LOADLIB.c>
#include <_util/clip_EMPTY.c>
#include <_util/clip_AADD.c>
#include <_util/clip_ADEL.c>
#include <_util/clip_ASIZE.c>
#include <_util/clip_AINS.c>
#include <_util/clip_ATAIL.c>
#include <_util/clip_AFILL.c>
#include <_util/clip_ACOPY.c>
#include <_util/clip_ACLONE.c>
#include <_util/clip_AEVAL.c>
#include <_util/clip_ASCAN.c>

typedef int (*q_cmp) (ClipVar * p1, ClipVar * p2, ClipMachine * ClipMachineMemory, void *par);

static void q_sort(ClipVar * a, int n, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par);

static void swap_sort(ClipVar * a, int n, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par);

#include <_util/static_compare_var.c>
#include <_util/clip_ASORT.c>

/* [ */

#define q_swap(a,b) { ClipVar t = *(a); *(a)=*(b); *(b)=t; }

#include <_util/static_swap_sort.c>

/* ] */
/* [ */

/* standard qsort(3) realisation with additional parameters to cmp function */

#define q_min(a,b)	((a)<(b)?(a):(b))

#include <_util/static_q_vecswap.c>
#include <_util/static_med3.c>
#include <_util/static_q_sort.c>
#include <_util/clip___RUN.c>
#include <_util/clip_CLONE.c>
#include <_util/clip_ISFUNCTION.c>
#include <_util/clip_GLOB.c>
#include <_util/static_put_str.c>
#include <_util/static_put_var.c>
#include <_util/static_refcmp.c>
#include <_util/_clip_var2str.c>
#include <_util/clip_VAR2STR.c>
#include <_util/static_get_byte.c>
/*
static int
get_long(char **buf, long *buflen, long *resp)
{
	if (*buflen < 4)
		return 0;
	*resp = ntohl(*((long *) *buf));

	(*buf) += 4;
	(*buflen) -= 4;

	return 4;
}
*/

#include <_util/static_get_int32.c>
#include <_util/static_get_str.c>
#include <_util/static_null_func.c>
#include <_util/static_get_var.c>

#define RECOVER_PREFIX "_recover_"
#define RECOVER_PREFIX_LEN (sizeof(RECOVER_PREFIX)-1)

#include <_util/_clip_str2var.c>
#include <_util/clip_STR2VAR.c>

/* Single character decode.  */
#define DEC(ch) (((ch) - ' ') & 077)
/* ENC is the basic 1 character encoding function to make a char printing.  */
#define ENC(ch) (uu_std[(ch) & 077])

const char uu_std[64] = {
   '`', '!', '"', '#', '$', '%', '&', '\'',
   '(', ')', '*', '+', ',', '-', '.', '/',
   '0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', ':', ';', '<', '=', '>', '?',
   '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
   'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
   'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
   'X', 'Y', 'Z', '[', '\\', ']', '^', '_'
};

#include <_util/_clip_uuencode.c>
#include <_util/_clip_uudecode.c>
#include <_util/clip_UUENCODE.c>
#include <_util/clip_UUDECODE.c>
#include <_util/clip_ULIMIT.c>
#include <_util/clip_MEMVARGET.c>
#include <_util/clip_MEMVARSET.c>
#include <_util/clip_ISPRIVATE.c>
#include <_util/clip_ISPUBLIC.c>
#include <_util/clip_ISMEMVAR.c>
#include <_util/clip_HOST_CHARSET.c>
#include <_util/_clip_translate_toutf8.c>
#include <_util/_clip_translate_fromutf8.c>
#include <_util/clip_TRANSLATE_CHARSET.c>
#include <_util/clip_REFCOUNT.c>
#include <_util/clip_HELP.c>
#include <_util/clip_LOADMODULEMSG.c>
#include <_util/clip_GETTEXT.c>
#include <_util/clip_NGETTEXT.c>
#include <_util/clip_DOSPARAM.c>
