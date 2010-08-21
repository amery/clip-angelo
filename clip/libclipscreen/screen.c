
/*
   $Log: screen.c,v $
   Revision 1.2  2007/03/09 14:42:13  itk
   uri: many fix for AMD64

   Revision 1.1  2006/06/22 19:35:31  itk
   uri: init sf.net repository

   Revision 1.108  2006/05/26 21:42:08  clip
   uri: small fix for utf terminal

   Revision 1.107  2005/03/01 11:44:58  clip
   uri: small fix

   Revision 1.106  2005/02/22 08:09:43  clip
   uri: small fix

   Revision 1.105  2005/02/22 08:05:49  clip
   uri: small fix

   Revision 1.104  2004/12/15 07:05:37  clip
   uri: add KSET*()

   Revision 1.103  2004/10/20 17:22:17  clip
   uri: add set(_SET_UTF8TERM) for terminal with UTF-8

   Revision 1.102  2004/06/21 04:43:03  clip
   uri: small fix

   Revision 1.101  2004/06/17 07:23:48  clip
   uri:small fix for utf-8 && xterm

   Revision 1.100  2004/05/19 08:32:18  clip
   rust: fix for ./configure -m

   Revision 1.99  2004/03/01 13:01:57  clip
   uri: add some support UTF-8 to screen output

   Revision 1.98  2003/12/19 09:35:38  clip
   uri: small fix for WIN32 about chars with codes <32

   Revision 1.97  2003/12/11 16:19:08  clip
   uri: small fix in scroll

   Revision 1.96  2003/11/21 09:34:18  clip
   ifdef USE_NATIONAL_KEY for CtrlN
   paul

   Revision 1.95  2003/11/12 09:43:16  clip
   fix for Ctrl-Z signal
   paul

   Revision 1.94  2003/10/29 11:40:03  clip
   small fix for #160 (clear screen)
   paul

   Revision 1.93  2003/10/29 08:49:29  clip
   fix terminal restoring in scanmode
   fix restore screen after run
   closes #160
   paul

   Revision 1.92  2003/09/09 15:36:20  clip
   uri: small fix for cygwin

   Revision 1.91  2003/09/09 14:36:15  clip
   uri: fixes for mingw from Mauricio and Uri

   Revision 1.88  2003/07/02 09:41:59  clip
   do not send IS termcap string to avoid konsole bug
   paul

   Revision 1.87  2003/02/07 08:44:03  clip
   fetch SIGINT in scanmode
   closes #117
   paul

   Revision 1.86  2003/01/10 12:19:53  clip
   set cursor on after exit from fullscreen program
   paul

   Revision 1.85  2002/12/20 11:21:46  clip
   fix for ^2, ^6, Ctrl+numpads in scan mode
   closes #88
   paul

   Revision 1.84  2002/12/20 08:53:50  clip
   fix enhanced scancodes in Cygwin
   closes #12
   asdf

   Revision 1.83  2002/11/20 10:22:52  clip
   include beep patch from Przemyslaw Czerpak <druzus@acn.waw.pl>
   paul

   Revision 1.82  2002/11/20 09:18:16  clip
   SET(_SET_ESC_DELAY[, <nMilliseconds>]) -> nOldMilliseconds
   get/set Esc timeout in milliseconds; default == 300 ms
   closes #50
   paul

   Revision 1.81  2002/11/13 12:58:13  clip
   auto set charset in fullscreen mode
   asdf

   Revision 1.80  2002/06/22 09:34:16  clip
   uri: shift & alt �����
   Revision 1.79  2002/05/31 09:32:20  clip
   again label under CYGWIN

   Revision 1.78  2002/05/28 10:54:59  clip
   uri: trapshift(),trapanykey(),keysend() added

   Revision 1.77  2002/05/28 06:48:27  clip
   scankey tricks
   paul

   Revision 1.76  2002/05/27 14:10:59  clip
   scankey fix
   paul

   Revision 1.75  2002/05/27 11:17:58  clip
   scankey() fix
   paul

   Revision 1.74  2002/05/27 09:52:41  clip
   uri: added "set fopen mode DOS|POSIX"

   Revision 1.73  2002/05/18 11:59:06  clip
   fix for f13-f20 func keys, as shift-F
   paul

   Revision 1.72  2002/04/24 09:51:14  clip
   _SET_DISPBOX turn off pg char translation
   paul

   Revision 1.71  2002/04/09 12:14:04  clip
   screen patches by Przemyslaw Czerpak
   paul

   Revision 1.70  2002/03/27 14:41:11  clip
   fix for Blink attribute
   paul

   Revision 1.69  2002/03/25 10:53:46  clip
   wrong i18n messages recoding
   paul

   Revision 1.68  2002/03/25 09:10:59  clip
   full move locale.po/ru to locale.po/ru_RU.KOI8-R
   paul

   Revision 1.67  2002/03/15 11:49:42  clip
   fix for Numlock key scancode in win >= 2k
   paul

   Revision 1.66  2002/03/05 11:52:44  clip
   redrawing on console fix '�
   paul

   Revision 1.65  2002/03/05 09:39:47  clip
   box drawing in scan mode
   paul

   Revision 1.64  2002/02/01 10:00:54  clip
   charset improvments
   paul

   Revision 1.63  2002/01/30 08:20:07  clip
   cigwin PG_CHARS fixes
   bzip2 func prefix auto-configuration
   paul

   Revision 1.62  2002/01/16 15:29:08  clip
   chinese patch
   preproc bugs
   some pseudographic chars < 32
   paul

   Revision 1.61  2001/12/11 07:48:25  clip
   middle mouse button
   paul

   Revision 1.60  2001/11/30 14:09:35  clip
   mouse & f1 esc conflict
   paul

   Revision 1.59  2001/11/28 12:41:38  clip
   mouse under cygwin
   paul

   Revision 1.58  2001/11/28 09:46:29  clip
   scanmode mouse events
   paul

   Revision 1.57  2001/11/27 16:02:55  clip
   mouse fixes
   paul

   Revision 1.56  2001/11/27 15:45:41  clip
   fixes for mouse
   paul

   Revision 1.55  2001/11/27 15:03:42  clip
   thish fucked comment

   Revision 1.54  2001/11/27 15:02:42  clip
   XTERM_MOUSE envar

   Revision 1.53  2001/11/27 13:46:15  clip
   mouse functions now work in xterm/rxvt terminals
   paul

   Revision 1.51  2001/11/26 15:39:50  clip
   Linux console GPM mouse handling
   paul

   Revision 1.50  2001/11/16 13:06:26  clip
   CYGWIN w32_hStdIn initialsation
   paul

   Revision 1.49  2001/11/16 11:40:01  clip
   export w32_hStdIn for CYGWIN tasks
   paul

   Revision 1.48  2001/11/16 10:15:59  clip
   CYGWIN & tasks
   paul

   Revision 1.47  2001/11/09 06:38:28  clip
   add terminfo catalog to $CLIPROOT/etc and it's usage in screen.c
   paul

   Revision 1.46  2001/11/08 09:30:40  clip
   high-color background fix
   paul

   Revision 1.45  2001/11/08 08:26:56  clip
   pseudographics & colors redraw fix
   paul

   Revision 1.44  2001/11/07 12:21:19  clip
   now absent termcap/terminfo under CYGWIN in w32 console mode is not error
   paul

   Revision 1.43  2001/10/29 11:56:34  clip
   fix for BLINK(intensitive background)
   paul

   Revision 1.42  2001/10/29 06:51:21  clip
   fix text fopen modes
   paul

   Revision 1.41  2001/09/12 10:13:12  clip
   export some funcs
   paul

   Revision 1.40  2001/09/11 12:28:10  clip
   changes
   paul

   Revision 1.39  2001/09/05 07:37:45  clip
   cygwin
   paul

   Revision 1.38  2001/07/26 13:31:05  clip
   last position in scroll
   paul

   Revision 1.37  2001/05/30 10:19:44  clip
   VT switch in scan mode on console
   paul

   Revision 1.36  2001/05/15 10:34:28  clip
   memdebug
   paul

   Revision 1.35  2001/04/10 10:53:19  clip
   scroll
   paul

   Revision 1.34  2001/02/22 13:43:24  clip
   fix
   paul

   Revision 1.33  2001/02/22 13:26:50  clip
   inkey() bug
   paul

   Revision 1.32  2001/02/22 13:03:48  clip
   inkey()
   paul

   Revision 1.31  2001/02/01 13:05:06  clip
   debug links
   paul

   Revision 1.30  2000/12/21 07:41:50  clip
   termcap init
   paul

   Revision 1.29  2000/12/06 13:13:01  clip
   loadable charset tables. size 256
   paul

   Revision 1.28  2000/12/04 14:12:10  clip
   tab
   paul

   Revision 1.27  2000/12/04 12:39:03  clip
   pgchars
   paul

   Revision 1.26  2000/12/04 11:43:06  clip
   pg_chars
   paul

   Revision 1.25  2000/12/04 11:09:59  clip
   del scancode
   paul

   Revision 1.24  2000/12/04 10:49:23  clip
   keymaps
   paul

   Revision 1.23  2000/12/04 10:09:49  clip
   keymaps
   paul

   Revision 1.22  2000/12/03 13:18:23  clip
   getRaw now push scancodes in handler
   paul

   Revision 1.21  2000/12/03 10:52:56  clip
   scancodes
   paul

   Revision 1.20  2000/12/02 13:47:40  clip
   CLIPROOT now may be envar
   paul

   Revision 1.19  2000/12/02 12:46:57  clip
   scancodes
   paul

   Revision 1.18  2000/11/21 11:31:13  clip
   io
   paul

   Revision 1.17  2000/11/08 08:11:06  clip
   ctrlbreak fix
   paul

   Revision 1.16  2000/10/31 11:50:07  clip
   setcancel and set(_SET_CANCEL)
   paul

   Revision 1.15  2000/05/23 23:07:30  clip
   scroll
   Paul Lasarev <paul@itk.ru>

   Revision 1.14  2000/05/16 15:44:22  clip
   add kend
   Paul Lasarev <paul@itk.ru>

   Revision 1.13  2000/05/03 22:06:40  clip
   *** empty log message ***

   Revision 1.12  2000/05/03 21:40:27  clip
   *** empty log message ***

   Revision 1.11  2000/05/03 21:39:22  clip
   add set cursor shape

   Revision 1.10  2000/05/01 20:12:43  clip
   VMIN=1 VTIME=0

   Revision 1.9  2000/05/01 19:40:58  clip
   SunOS port

   Revision 1.8  2000/04/20 19:39:52  paul
   take lines & columns from termcap/terminfo, defaults to 25x80

   Revision 1.7  2000/04/13 17:38:47  paul
   f10-f12 keys

   Revision 1.6  2000/03/10 15:56:10  paul
   xon/xoff gluk

   Revision 1.5  2000/03/07 13:35:03  paul
   fix bug with repeated GetKey_Wait

   Revision 1.4  1999/12/10 19:39:44  paul
   add correct terminal init/resume

   Revision 1.3  1999/12/04 19:15:13  uri
   ���gcc

   Revision 1.2  1999/11/02 23:05:52  axl
   start of logging CVS
*/

#include <string.h>
#include <ci_clip.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#ifndef OS_MINGW
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <ci_screen.h>
#include <ci_tcaps.h>
#include <ci_charset.h>
#include <ci_scankey.h>

#ifdef OS_CYGWIN
#include <w32api/windows.h>
#include <sys/cygwin.h>
#endif

/* use CtrlN as internal rus/lat switch */
/*#define USE_NATIONAL_KEY*/

/* #define DBG */
#ifdef _WIN32
static int w32_console = 0;

static CHAR_INFO *w32_screen = 0;

static COORD w32_size, w32_beg, w32_end;

HANDLE w32_hStdIn = INVALID_HANDLE_VALUE, w32_hStdOut = INVALID_HANDLE_VALUE;	/* standard input, output handles */

static unsigned char *w32_scan_buf = 0;

static int w32_scan_buf_len = 0;

static int w32_scan_buf_size = 0;

static int w32_scrsize = 0;

static void w32_flush(void);

static void w32_clear(void);

static void w32_set_cursor_shape(int cursor);

static void w32_set_cursor(int r, int c);

static void w32_beep(void);

static int w32_readch(void);
#endif

#ifdef USE_TASKS
#include <ci_task.h>
#endif

#ifndef timercmp
#define	timercmp(a, b, CMP) 							  \
  (((a)->tv_sec == (b)->tv_sec) ? 						  \
   ((a)->tv_usec CMP (b)->tv_usec) : 						  \
   ((a)->tv_sec CMP (b)->tv_sec))
#endif

#ifdef HAVE_GPM_H

#include <gpm.h>

static Gpm_Connect conn;

#endif

#define RAWMODE_ESC 117

int esc_delay_Screen = 300;

static int has_esc = 0;

static int xterm_mouse = 0, xterm_pos = 0, xterm_buttons = 0, xterm_x = 0, xterm_y = 0;

#if 0
#define GPM_XTERM_ON \
  printf("%c[?1001s",27), fflush(stdout), /* save old hilit tracking */ \
  printf("%c[?1000h",27), fflush(stdout)	/* enable mouse tracking */

#define GPM_XTERM_OFF \
  printf("%c[?1000l",27), fflush(stdout), /* disable mouse tracking */ \
  printf("%c[?1001r",27), fflush(stdout)	/* restore old hilittracking */
#endif

extern char *CLIPROOT;

static void init_mouse(ScreenBase * base, char **envp);

#define Meta1_key 0x10000
#define Meta2_key 0x20000
#define National_key 0x30000

static char *FNAMES[] = {
   "/etc/terminfo",
   "/usr/lib/terminfo",
   "/usr/share/terminfo",
   "/usr/share/lib/terminfo",
   "/usr/share/misc/termcap",
   "/etc/termcap",
};

#define FSTDNUM  4
#define FNUM  6

/* cursor goto handling with padding/delay checking */
void scr_tgoto(char *CM, int destcol, int destline, char *buf, int buflen);

int scr_tputs(const char *str, int affcnt, int (*put_c) (int, void *), void *par);

static char *scan_start = 0;

static char *scan_stop = 0;

#include <screen/typedef_enum_ScanMode.h>

ScanMode scr_scan_mode = ScanNone;

static void start_scan_mode(int fd);

static void stop_scan_mode(int fd);

static unsigned long get_wait_key(ScreenBase * base, long milliseconds, int raw);

#include <screen/typedef_struct_Keytab.h>

static const Keytab init_keytab[] = {
   {0, Meta1_key,},		/* meta1 */
   {0, Meta2_key,},		/* meta2 */
   {0, National_key,},		/* national */

   /* 3 */ {0, KEY_LEFT,},
   /* "kl" */
   {0, KEY_RIGHT,},		/* "kr" */
   {0, KEY_UP,},		/* "ku" */
   {0, KEY_DOWN,},		/* "kd" */
   /*7 */ {0, KEY_PGDN,},
   /* "kN" */
   {0, KEY_PGUP,},		/* "kP" */
   {0, KEY_HOME,},		/* "kh" */
   /*10 */ {0, KEY_END,},
   /* "kH" */
   {0, KEY_INS,},		/* "kI" */
   {0, KEY_BS,},		/* "kb" */
   {0, KEY_DEL,},		/* "kD" */

   /* 14 */ {0, KEY_F1},
   /* "k1" */
   {0, KEY_F2},			/* "k2" */
   {0, KEY_F3},			/* "k3" */
   {0, KEY_F4},			/* "k4" */
   {0, KEY_F5},			/* "k5" */
   {0, KEY_F6},			/* "k6" */
   {0, KEY_F7},			/* "k7" */
   {0, KEY_F8},			/* "k8" */
   {0, KEY_F9},			/* "k9" */
   /*23 */ {0, KEY_F10},
   /* "k0" */

   /*24 */ {0, KEY_F11},
   /* "k11" */
   /*25 */ {0, KEY_F12},
   /* "k12" */

   {0, KEY_F13},		/* "k13" */
   {0, KEY_F14},		/* "k14" */
   {0, KEY_F15},		/* "k15" */
   {0, KEY_F16},		/* "k16" */
   {0, KEY_F17},		/* "k17" */
   {0, KEY_F18},		/* "k18" */
   {0, KEY_F19},		/* "k19" */
   {0, KEY_F20},		/* "k20" */
/* 36 */
   /* ANSI  alternatives */
#if 0
   {"\033[M", KEY_F1,},
#else
   {"\033[M", KEY_MOUSE,},	/* start of XTERM mouse sequence \e[MCbCxCy */
#endif

   {"\033[N", KEY_F2,},
   {"\033[O", KEY_F3,},
   {"\033[P", KEY_F4,},
   {"\033[Q", KEY_F5,},
   {"\033[R", KEY_F6,},
   {"\033[S", KEY_F7,},
   {"\033[T", KEY_F8,},
   {"\033[U", KEY_F9,},
   {"\033[V", KEY_F10,},

   {"\033[D", KEY_LEFT,},
   {"\033[C", KEY_RIGHT,},
   {"\033[A", KEY_UP,},
   {"\033[B", KEY_DOWN,},
   {"\033[G", KEY_PGDN,},
   {"\033[I", KEY_PGUP,},
   {"\033[H", KEY_HOME,},
   {"\033[F", KEY_END,},
   {"\033[L", KEY_INS,},
   {"\010", KEY_BS,},
   {"\177", KEY_DEL,},

   /* vt100/linux  alternatives */

   {"\033[[A", KEY_F1,},
   {"\033[[B", KEY_F2,},
   {"\033[[C", KEY_F3,},
   {"\033[[D", KEY_F4,},
   {"\033[[E", KEY_F5,},

   /* some strange xterms */
   {"\033[11~", KEY_F1,},
   {"\033[12~", KEY_F2,},
   {"\033[13~", KEY_F3,},
   {"\033[14~", KEY_F4,},
   {"\033[15~", KEY_F5,},

   {"\033[17~", KEY_F6,},
   {"\033[18~", KEY_F7,},
   {"\033[19~", KEY_F8,},
   {"\033[20~", KEY_F9,},
   {"\033[21~", KEY_F10,},

   {"\033[23~", KEY_F11,},
   {"\033[24~", KEY_F12,},
   {"\033[25~", KEY_F13,},
   {"\033[26~", KEY_F14,},

   {"\033[28~", KEY_F15,},
   {"\033[29~", KEY_F16,},
   {"\033[30~", KEY_F17,},
   {"\033[31~", KEY_F18,},

   {"\033[33~", KEY_F19,},
   {"\033[34~", KEY_F20,},

/*
   { "\033[D",  KEY_LEFT ,   },
   { "\033[C",  KEY_RIGHT,  },
   { "\033[A",  KEY_UP   ,     },
   { "\033[B",  KEY_DOWN ,   },
 */
   {"\033[6~", KEY_PGDN,},
   {"\033[5~", KEY_PGUP,},
   {"\033[1~", KEY_HOME,},
   {"\033[4~", KEY_END,},
   {"\033[2~", KEY_INS,},
/*      { "\177",       KEY_BS   ,   }, */
   {"\033[3~", KEY_DEL,},

   /* Esc-key  alternatives */

   {"\033\061", KEY_F1,},
   {"\033\062", KEY_F2,},
   {"\033\063", KEY_F3,},
   {"\033\064", KEY_F4,},
   {"\033\065", KEY_F5,},
   {"\033\066", KEY_F6,},
   {"\033\067", KEY_F7,},
   {"\033\070", KEY_F8,},
   {"\033\071", KEY_F9,},
   {"\033\060", KEY_F10,},

   {"\033l", KEY_LEFT,},
   {"\033r", KEY_RIGHT,},
   {"\033u", KEY_UP,},
   {"\033d", KEY_DOWN,},
   {"\033n", KEY_PGDN,},
   {"\033p", KEY_PGUP,},
   {"\033h", KEY_HOME,},
   {"\033e", KEY_END,},
   {"\033i", KEY_INS,},
   {"\033s", KEY_BS,},
   {"\033b", KEY_DEL,},

   {"\033\033", 27,},

   {0, 0},
};

#define KEYTAB_SIZE (sizeof(init_keytab)/sizeof(Keytab))

#include <screen/typedef_struct_ScreenData.h>

static ScreenData *screen_data = 0;

static struct termios start_mode;

static const char ascii_pg_chars[PG_SIZE] = " fv<>^#o:\\+>*-#+<++!#~--_#++++++|-++++++++|";

static void init_ScreenData(ScreenData * dp);

static void destroy_ScreenData(ScreenData * dp);

/* termcap statics { */

static void initKey(ScreenData * dp);

static void termcap_clear_screen(ScreenData * dp);

static void termcap_scroll(ScreenBase * base, int top, int bottom, int n);

static void termcap_put_char(ScreenData * dp, int ch, int utf8term);

static void termcap_put_graph_char(ScreenData * dp, int ch);

static int termcap_put_raw_char(int c, void *p);

static void termcap_put_raw_str(ScreenData * dp, const char *str);

static void termcap_flush(ScreenData * dp);

static void termcap_set_color(ScreenData * dp, int color);

static void termcap_set_fgbg(ScreenData * dp, int fg, int bg);

static void termcap_set_cursor(ScreenData * dp, int y, int x);

static void termcap_beep(ScreenData * dp);

static void termcap_set_cursor_shape(ScreenData * dp, int cursor);

#define VisualBold	1
#define VisualBlink	2
#define VisualDim	4
#define VisualInverse	8
#define VisualColors	16
#define VisualGraph	32

/* termcap statics }[ termcap init  */

#include <screen/static_skipDelay.c>

static const int cga_to_ansi[16] = {	/* ansi colors: */
   0,				/* 0black */
   4,				/* 1red */
   2,				/* 2green */
   6,				/* 3brown */
   1,				/* 4blue */
   5,				/* 5magenta */
   3,				/* 6cyan */
   7,				/* 7gray */
   0,
   4,
   2,
   6,
   1,
   5,
   3,
   7,
};

#include <screen/static_setColorMap.c>
#include <screen/restore_tty.c>
#include <screen/restart_tty.c>
#include <screen/static_exit_tty.c>
#include <screen/static_squeesh_entry.c>
#include <screen/static_squeesh_if_need.c>
#include <screen/static_get_env.c>
#include <screen/static_read_oct.c>
#include <screen/init_tty.c>
#include <screen/destroy_tty.c>

static const unsigned char koi8_nationalTable[128] = {
   0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
   0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
   0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,

   ' ', '!', 252, '/', 44, ':', '.', 218,
   '?', '%', ';', '+', 194, '-', 192, '/',
   '0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', 246, 214, 226, '=', 224, '?',
   '"', 230, 233, 243, 247, 245, 225, 240,
   242, 251, 239, 236, 228, 248, 244, 253,
   250, 234, 235, 249, 229, 231, 237, 227,
   254, 238, 241, 200, '\\', 223, ',', '_',
   '`', 198, 201, 211, 215, 213, 193, 208,
   242 - 32, 251 - 32, 239 - 32, 236 - 32, 228 - 32, 248 - 32, 244 - 32,
   253 - 32,
   250 - 32, 234 - 32, 235 - 32, 249 - 32, 229 - 32, 231 - 32, 237 - 32,
   227 - 32,
   222, 206, 209, 232, '|', 255, '~', 127,
};

#include <screen/static_destroy_ScreenData.c>
#include <screen/static_get_str.c>
#include <screen/static_get_key.c>
#include <screen/static_init_ScreenData.c>
#include <screen/clear_Screen.c>
#include <screen/setCtrlBreak_Screen.c>
#include <screen/static_compkeys.c>
#include <screen/static_initKey.c>

/* termcap init ][ termcap capabilities  */

#include <screen/static_termcap_put_char.c>
#include <screen/static_termcap_put_graph_char.c>
#include <screen/static_termcap_put_raw_char.c>
#include <screen/static_termcap_put_raw_str.c>
#include <screen/static_termcap_flush.c>
#include <screen/static_termcap_set_color.c>
#include <screen/static_termcap_set_cursor_shape.c>
#include <screen/static_termcap_set_fgbg.c>
#include <screen/static_termcap_set_cursor.c>
#include <screen/static_termcap_clear_screen.c>
#include <screen/static_termcap_scroll.c>
#include <screen/static_termcap_beep.c>
#include <screen/newMatch_Key.c>
#include <screen/static_cmp_first.c>
#include <screen/static_term_match_Key.c>
#include <screen/getState_Key.c>
#include <screen/setState_Key.c>
#include <screen/static_scan_match_Key.c>
#include <screen/match_Key.c>
#include <screen/getRaw_Key.c>
#include <screen/getRawWait_Key.c>

#ifdef HAVE_GPM_H

/*-------------------------------------------------------------------*/
#include <screen/static_gpm_get_event.c>
#include <screen/static_get_gpm_mouse.c>

#endif

#include <screen/get_Key.c>

#define tv_cmp(a,b,CMP) \
	  (((a)->tv_sec == (b)->tv_sec) ?            \
	   ((a)->tv_usec CMP (b)->tv_usec) :            \
	   ((a)->tv_sec CMP (b)->tv_sec))

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

#include <screen/getWait_Key.c>
#include <screen/static_get_wait_key.c>

/* termcap capabilities ][ Screen  */

#include <screen/new_Screen.c>
#include <screen/delete_Screen.c>
#include <screen/beep_Screen.c>
#include <screen/addLine_Screen.c>
#include <screen/delLine_Screen.c>
#include <screen/scroll_Screen.c>
#include <screen/scrollw_Screen.c>
#include <screen/static_syncLine.c>
#include <screen/sync_Screen.c>
#include <screen/redraw_Screen.c>
#include <screen/iscolor_tty.c>
#include <screen/setAttr_Screen.c>
#include <screen/static_set_rawmode.c>
#include <screen/static_start_scan_mode.c>
#include <screen/static_stop_scan_mode.c>

#ifdef _WIN32

#include <screen/static_w32_clear.c>
#include <screen/static_w32_set_cursor.c>
#include <screen/static_w32_set_cursor_shape.c>
#include <screen/static_w32_beep.c>
#include <screen/static_w32_flush.c>
#include <screen/static_w32_get_scan_buf.c>
#include <screen/static_w32_put_scan_buf.c>
#include <screen/static_w32_push_key_event.c>
#include <screen/static_w32_push_mouse_event.c>
#include <screen/static_w32_readch.c>

#endif

#include <screen/static_init_mouse.c>
#include <screen/setPgMode_Screen.c>
#include <screen/setUtf8Mode_Screen.c>

/* Screen ] */
