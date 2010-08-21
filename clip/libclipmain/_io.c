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

#include <ci_clip.h>
#include <../libclipscreen/ci_screen.h>
#include <ci_hashcode.h>
#include <ci_error.ch>
#include <ci_ctwin.ch>
#include <ci_config.ch>
#include <ci_rational.h>
#include <ci_coll.h>

#ifdef USE_TASKS
#include <../libcliptask/ci_task.h>
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

#include <_io/_clip_dtos.c>
#include <_io/_clip_colorSelect.c>
#include <_io/_clip_sprintf.c>
#include <_io/_clip_vsprintf.c>
#include <_io/static_out_dev.c>
#include <_io/static_out_std.c>
#include <_io/static_out_err.c>
#include <_io/static_out_dbg.c>
#include <_io/static_out_log.c>
#include <_io/static_out_any.c>
#include <_io/_clip_printf.c>
#include <_io/_clip_print_dbg.c>
#include <_io/static_print_var.c>
#include <_io/clip_QQOUT.c>
#include <_io/clip_QOUT.c>
#include <_io/clip_OUTSTD.c>
#include <_io/clip_OUTERR.c>
#include <_io/clip_OUTLOG.c>
#include <_io/clip_SLEEP.c>
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

#include <_io/clip_MILLISEC.c>
#include <_io/clip_NDATE.c>
#include <_io/_clip_strFunc.c>
#include <_io/clip_STR.c>
#include <_io/clip_RSTR.c>
#include <_io/clip_STRZERO.c>

#ifdef CLIP_TRANSFORM_C

#include <_io/static_is_num.c>
#include <_io/static_do_templ.c>
#include <_io/static_transform.c>
#include <_io/clip_TRANSFORM.c>

#endif

#include <_io/_clip_dtostr.c>
#include <_io/_clip_strtod.c>
#include <_io/clip_VAL.c>
#include <_io/clip_FLOATVAL.c>
#include <_io/static_eject.c>
#include <_io/clip___EJECT.c>
#include <_io/clip_SETPOS.c>
#include <_io/clip_SETCURSOR.c>
#include <_io/clip_DEVPOS.c>
#include <_io/static_is_pgch.c>
#include <_io/static_out_scr.c>
#include <_io/_clip_str2attr.c>
#include <_io/_clip_attr2str.c>
#include <_io/static_get_color.c>
#include <_io/_clip_fullscreen.c>
#include <_io/clip_DEVOUT.c>

#ifdef CLIP_TRANSFORM_C

#include <_io/clip_DEVOUTPICT.c>

#endif

#include <_io/static_sync_mp.c>
#include <_io/clip_DISPOUT.c>
#include <_io/clip_DISPPOS.c>
#include <_io/clip_DISPOUTAT.c>
#include <_io/clip_DISPSTR.c>
#include <_io/clip_BLINKATTR.c>
#include <_io/clip_BOLDATTR.c>
#include <_io/clip_DISPATTR.c>
#include <_io/clip_FT_SHADOW.c>
#include <_io/clip_FT_SETATTR.c>
#include <_io/static_disp_box.c>
#include <_io/clip_DISPBOXTERM.c>

#ifdef USE_TASKS
int clip_TASKSTART(ClipMachine * ClipMachineMemory);

int clip_TASKSTOP(ClipMachine * ClipMachineMemory);
#endif

#include <_io/clip_DISPBEGIN.c>
#include <_io/clip_DISPEND.c>
#include <_io/clip_DISPCOUNT.c>
#include <_io/clip_ROW.c>
#include <_io/clip_COL.c>
#include <_io/clip_PROW.c>
#include <_io/clip_PCOL.c>
#include <_io/clip_SETPRC.c>
#include <_io/clip_MAXROW.c>
#include <_io/clip_MAXCOL.c>

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

#include <_io/static_key2clip.c>
#include <_io/clip___KEYBOARD.c>
#include <_io/clip_LASTKEY.c>
#include <_io/clip_SETLASTKEY.c>
#include <_io/clip_NEXTKEY.c>
#include <_io/clip_ISKBDEMPTY.c>
#include <_io/clip_CHR.c>
#include <_io/clip_ASC.c>

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

#include <_io/_clip_key.c>
#include <_io/clip_SCANKEY.c>
#include <_io/clip_KBDSTAT.c>
#include <_io/clip___KSETSTATE.c>
#include <_io/clip_KSETNUM.c>
#include <_io/clip_KSETCAPS.c>
#include <_io/clip_KSETSCROLL.c>
#include <_io/clip_KSETINS.c>
#include <_io/clip__INKEY.c>
#include <_io/clip_SETKEY.c>
#include <_io/clip___ACCEPTSTR.c>
#include <_io/clip___ACCEPT.c>
#include <_io/static_wind_region.c>
#include <_io/static_clip_region.c>
#include <_io/static_save_region.c>
#include <_io/static_rest_region.c>
#include <_io/clip_SAVESCREEN.c>
#include <_io/clip_RESTSCREEN.c>
#include <_io/clip_SCROLL.c>
#include <_io/clip_MEMORY.c>
#include <_io/clip_ALLOFREE.c>
#include <_io/clip_STACKFREE.c>

/* ][ window funcs */

#include <_io/typedef_struct_WindowSave.h>
#include <_io/static_save_window.c>
#include <_io/static_draw_shadow.c>
#include <_io/static_rest_window.c>
#include <_io/static_destroy_window.c>
#include <_io/static_select_window.c>
#include <_io/static_calc_center.c>
#include <_io/static_move_window.c>
#include <_io/static_adjust_cursor.c>
#include <_io/clip_WACLOSE.c>
#include <_io/clip_WBOARD.c>
#include <_io/clip_WBOXTERM.c>
#include <_io/clip_WCENTER.c>
#include <_io/clip_WCLOSE.c>
#include <_io/clip_WCOL.c>
#include <_io/clip_WFCOL.c>
#include <_io/clip_WFLASTCOL.c>
#include <_io/clip_WFLASTROW.c>
#include <_io/clip_WFORMAT.c>
#include <_io/clip_WFROW.c>
#include <_io/clip_WLASTCOL.c>
#include <_io/clip_WLASTROW.c>
#include <_io/clip_WMODE.c>
#include <_io/clip_WMOVE.c>
#include <_io/clip_WNUM.c>
#include <_io/clip_WOPEN.c>
#include <_io/clip_WREOPEN.c>
#include <_io/clip_WROW.c>
#include <_io/clip_WSELECT.c>
#include <_io/clip_WSETMOVE.c>
#include <_io/clip_WSETSHADOW.c>
#include <_io/clip_WSTEP.c>

/* window funcs ][ */

#include <_io/static_dialog_init.c>
#include <_io/clip___BEEP.c>
#include <_io/clip_SCREENMIX.c>
#include <_io/clip_MPRESENT.c>
#include <_io/clip_MHIDE.c>
#include <_io/clip_MSHOW.c>
#include <_io/clip_MSETCURSOR.c>
#include <_io/clip_MROW.c>
#include <_io/clip_MCOL.c>
#include <_io/clip_MLEFTDOWN.c>
#include <_io/clip_MRIGHTDOWN.c>
#include <_io/clip_MMIDDLEDOWN.c>
#include <_io/clip_MSETBOUNDS.c>
#include <_io/clip_MSAVESTATE.c>
#include <_io/clip_MRESTSTATE.c>
#include <_io/clip_MDBLCLK.c>
#include <_io/clip_MSETPOS.c>
#include <_io/clip_MSTATE.c>
#include <_io/clip_ISCOLOR.c>
#include <_io/clip_CENTER.c>
#include <_io/typedef_struct_WinBuf.h>

static Coll *winbufs = 0;

static WinBuf *new_WinBuf(int hi, int wi);

static void destroy_WinBuf(void *item);

static WinBuf *find_WinBuf(int no);

#include <_io/clip_WINBUF_CREATE.c>
#include <_io/clip_WINBUF_DESTROY.c>
#include <_io/static_new_WinBuf.c>
#include <_io/static_destroy_WinBuf.c>
#include <_io/static_find_WinBuf.c>
#include <_io/clip_WINBUF_OUT_AT.c>
#include <_io/clip_WINBUF_ATTR_AT.c>
#include <_io/clip_WINBUF_COPY.c>
#include <_io/clip_WINBUF_DISPLAY.c>
#include <_io/clip_WINBUF_OUT_TRANS.c>
#include <_io/clip_GETMEMBUF.c>
#include <_io/clip_SCREENCHAR.c>
#include <_io/clip_SCREENATTR.c>
#include <_io/clip_SCREENSTR.c>
#include <_io/clip_SCREENSTRING.c>
