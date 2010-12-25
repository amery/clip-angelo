
/*
   $Log: scankey.c,v $
   Revision 1.1  2006/06/22 19:35:31  itk
   uri: init sf.net repository

   Revision 1.35  2005/08/08 09:00:32  clip
   alena: fix for gcc 4

   Revision 1.34  2005/02/03 17:21:29  clip
   uri: kbdstat(), bit #18, for En/Ru detection. Only for scan mode keyboard.

   Revision 1.33  2004/12/15 07:05:37  clip
   uri: add KSET*()

   Revision 1.32  2004/07/08 10:28:37  clip
   uri: small fix

   Revision 1.31  2004/07/08 09:15:35  clip
   uri: small fix

   Revision 1.30  2004/07/08 07:21:05  clip
   uri: small fix for kbdstat()

   Revision 1.29  2003/09/02 14:27:43  clip
   changes for MINGW from
   Mauricio Abre <maurifull@datafull.com>
   paul

   Revision 1.28  2003/02/18 10:17:53  clip
   possible fixes scancode priority to ALT-CTRL-SHIFT
   paul

   Revision 1.27  2003/02/07 09:14:53  clip
   leds on linux console in scan mode
   closes #104
   paul

   Revision 1.26  2002/12/20 11:21:46  clip
   fix for ^2, ^6, Ctrl+numpads in scan mode
   closes #88
   paul

   Revision 1.25  2002/12/20 08:53:50  clip
   fix enhanced scancodes in Cygwin
   closes #12
   asdf

   Revision 1.24  2002/12/17 12:08:46  clip
   fix scancodes for keypad '/' '*' '-' '+' + alt/ctrl
   paul

   Revision 1.23  2002/09/20 10:41:45  clip
   fix for shift-Home/End/PgUn/PgDn/Up/Down/Left/Right
   paul

   Revision 1.22  2002/07/30 11:05:14  clip
   scancodes
   paul

   Revision 1.21  2002/07/30 10:44:10  clip
   preproc: fix for delim
   paul

   Revision 1.20  2002/07/30 08:06:16  clip
   Alt0-9 in scanmode
   paul

   Revision 1.19  2002/05/28 06:48:27  clip
   scankey tricks
   paul

   Revision 1.18  2002/05/27 14:10:59  clip
   scankey fix
   paul

   Revision 1.17  2002/03/15 11:49:42  clip
   fix for Numlock key scancode in win >= 2k
   paul

   Revision 1.16  2002/01/30 08:20:07  clip
   cigwin PG_CHARS fixes
   bzip2 func prefix auto-configuration
   paul

   Revision 1.15  2001/05/30 10:49:27  clip
   switch
   paul

   Revision 1.14  2001/05/30 10:37:19  clip
   vt switch
   paul

   Revision 1.13  2001/05/30 10:31:21  clip
   vt switch -- no - 1
   paul

   Revision 1.12  2001/05/30 10:19:43  clip
   VT switch in scan mode on console
   paul

   Revision 1.11  2001/05/15 10:34:28  clip
   memdebug
   paul

   Revision 1.10  2000/12/08 12:59:50  clip
   rus kbd and caps_lock flags

   Revision 1.9  2000/12/06 07:37:54  clip
   adsf
   paul

   Revision 1.8  2000/12/05 07:22:07  clip
   alt=
   paul

   Revision 1.7  2000/12/04 14:12:09  clip
   tab
   paul

   Revision 1.6  2000/12/04 10:09:49  clip
   keymaps
   paul

   Revision 1.5  2000/12/03 13:00:41  clip
   scankeys
   paul

   Revision 1.4  2000/12/03 12:42:34  clip
   scancodes
   paul

   Revision 1.3  2000/12/03 10:52:56  clip
   scancodes
   paul

   Revision 1.2  2000/12/02 12:46:57  clip
   scancodes
   paul

   Revision 1.1  2000/11/30 14:57:15  clip
   scancode support
   paul

 */

#include <ci_clipcfg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef OS_MINGW
#include <sys/ioctl.h>
#endif

#include <ci_clip.h>
#include <ci_scankey.h>

/* #define DBG  */
/* #define DBG0 */
/* #define DBG1 */
/* #define DBG2 */

#define KG_SHIFT	0
#define KG_CTRL		2
#define KG_ALT		3
#define KG_ALTGR	1
#define KG_SHIFTL	4
#define KG_SHIFTR	5
#define KG_CTRLL	6
#define KG_CTRLR	7
#define KG_CAPSSHIFT	8

#define K_SHIFTLOCK	K(KT_LOCK,KG_SHIFT)
#define K_CTRLLOCK	K(KT_LOCK,KG_CTRL)
#define K_ALTLOCK	K(KT_LOCK,KG_ALT)
#define K_ALTGRLOCK	K(KT_LOCK,KG_ALTGR)
#define K_SHIFTLLOCK	K(KT_LOCK,KG_SHIFTL)
#define K_SHIFTRLOCK	K(KT_LOCK,KG_SHIFTR)
#define K_CTRLLLOCK	K(KT_LOCK,KG_CTRLL)
#define K_CTRLRLOCK	K(KT_LOCK,KG_CTRLR)

#define NR_SHIFT	9
#define NR_PAD		20
#define NR_DEAD		6
#define NR_ASCII	26
#define NR_LOCK		8

#define KT_LATIN	0	/* we depend on this being zero */
#define KT_LETTER	11	/* symbol that can be acted upon by CapsLock */
#define KT_FN		1
#define KT_SPEC		2
#define KT_PAD		3
#define KT_DEAD		4
#define KT_CONS		5
#define KT_CUR		6
#define KT_SHIFT	7
#define KT_META		8
#define KT_ASCII	9
#define KT_LOCK		10
#define KT_SLOCK	12

#define K(t,v)		(((t)<<8)|(v))
#define KTYP(x)		((x) >> 8)
#define KVAL(x)		((x) & 0xff)

#define K_SHIFT		K(KT_SHIFT,KG_SHIFT)
#define K_CTRL		K(KT_SHIFT,KG_CTRL)
#define K_ALT		K(KT_SHIFT,KG_ALT)
#define K_ALTGR		K(KT_SHIFT,KG_ALTGR)
#define K_SHIFTL	K(KT_SHIFT,KG_SHIFTL)
#define K_SHIFTR	K(KT_SHIFT,KG_SHIFTR)
#define K_CTRLL		K(KT_SHIFT,KG_CTRLL)
#define K_CTRLR		K(KT_SHIFT,KG_CTRLR)
#define K_CAPSSHIFT	K(KT_SHIFT,KG_CAPSSHIFT)

#define K_P0		K(KT_PAD,0)
#define K_P1		K(KT_PAD,1)
#define K_P2		K(KT_PAD,2)
#define K_P3		K(KT_PAD,3)
#define K_P4		K(KT_PAD,4)
#define K_P5		K(KT_PAD,5)
#define K_P6		K(KT_PAD,6)
#define K_P7		K(KT_PAD,7)
#define K_P8		K(KT_PAD,8)
#define K_P9		K(KT_PAD,9)
#define K_PPLUS		K(KT_PAD,10)	/* key-pad plus */
#define K_PMINUS	K(KT_PAD,11)	/* key-pad minus */
#define K_PSTAR		K(KT_PAD,12)	/* key-pad asterisk (star) */
#define K_PSLASH	K(KT_PAD,13)	/* key-pad slash */
#define K_PENTER	K(KT_PAD,14)	/* key-pad enter */
#define K_PCOMMA	K(KT_PAD,15)	/* key-pad comma: kludge... */
#define K_PDOT		K(KT_PAD,16)	/* key-pad dot (period): kludge... */
#define K_PPLUSMINUS	K(KT_PAD,17)	/* key-pad plus/minus */
#define K_PPARENL	K(KT_PAD,18)	/* key-pad left parenthesis */
#define K_PPARENR	K(KT_PAD,19)	/* key-pad right parenthesis */

#define K_DOWN		K(KT_CUR,0)
#define K_LEFT		K(KT_CUR,1)
#define K_RIGHT		K(KT_CUR,2)
#define K_UP		K(KT_CUR,3)

#define K_F1		K(KT_FN,0)
#define K_F2		K(KT_FN,1)
#define K_F3		K(KT_FN,2)
#define K_F4		K(KT_FN,3)
#define K_F5		K(KT_FN,4)
#define K_F6		K(KT_FN,5)
#define K_F7		K(KT_FN,6)
#define K_F8		K(KT_FN,7)
#define K_F9		K(KT_FN,8)
#define K_F10		K(KT_FN,9)
#define K_F11		K(KT_FN,10)
#define K_F12		K(KT_FN,11)
#define K_F13		K(KT_FN,12)
#define K_F14		K(KT_FN,13)
#define K_F15		K(KT_FN,14)
#define K_F16		K(KT_FN,15)
#define K_F17		K(KT_FN,16)
#define K_F18		K(KT_FN,17)
#define K_F19		K(KT_FN,18)
#define K_F20		K(KT_FN,19)
#define K_FIND		K(KT_FN,20)
#define K_INSERT	K(KT_FN,21)
#define K_REMOVE	K(KT_FN,22)
#define K_SELECT	K(KT_FN,23)
#define K_PGUP		K(KT_FN,24)	/* PGUP is a synonym for PRIOR */
#define K_PGDN		K(KT_FN,25)	/* PGDN is a synonym for NEXT */
#define K_MACRO		K(KT_FN,26)
#define K_HELP		K(KT_FN,27)
#define K_DO		K(KT_FN,28)
#define K_PAUSE		K(KT_FN,29)

#define SIZE(x) (sizeof(x)/sizeof((x)[0]))
#define BITS_PER_LONG (sizeof(long)*8)

/* clip/screen.h keycodes */
#define META_MASK   0x10000000
#define META1_MASK  0x20000000
#define META2_MASK  0x40000000
#define META(x)    ((x)|META_MASK)
#define META1(x)    ((x)|META1_MASK)
#define META2(x)    ((x)|META2_MASK)
#define IS_META1(x) ((x)&META1_MASK)
#define IS_META2(x) ((x)&META2_MASK)
#define CNTRL(x) ((x) & 037)

#define KEY_LEFT    META('l')
#define KEY_RIGHT   META('r')
#define KEY_UP      META('u')
#define KEY_DOWN    META('d')
#define KEY_PGDN    META('n')
#define KEY_PGUP    META('p')
#define KEY_HOME    META('h')
#define KEY_END     META('e')
#define KEY_INS     META('i')
#define KEY_BS      META('s')
#define KEY_DEL     META('b')
#define KEY_ENTER   '\r'
#define KEY_ESC	    '\033'

#define KEY_F1      META('1')
#define KEY_F2      META('2')
#define KEY_F3      META('3')
#define KEY_F4      META('4')
#define KEY_F5      META('5')
#define KEY_F6      META('6')
#define KEY_F7      META('7')
#define KEY_F8      META('8')
#define KEY_F9      META('9')
#define KEY_F10     META('0')
#define KEY_F11     META('A')
#define KEY_F12     META('B')

#define MAX_NR_FUNC	256	/* max nr of strings assigned to keys */
#define MAX_DIACR	256
#define MAX_NR_KEYMAPS	256
#define NR_KEYS		128

#include <scankey/struct_kbdiacr.h>

#include <scankey/typedef_enum_ScanMode.h>

extern ScanMode scr_scan_mode;

extern char *func_table[MAX_NR_FUNC];

extern unsigned int keymap_count;

extern unsigned short *key_maps[MAX_NR_KEYMAPS];

extern unsigned short plain_map[NR_KEYS];

extern struct kbdiacr accent_table[MAX_DIACR];

extern unsigned int accent_table_size;

/* shift state counters.. */
static unsigned char k_down[NR_SHIFT] = { 0, };

/* keyboard key bitmap */
static unsigned long key_down[256 / BITS_PER_LONG] = {
   0,
};

static void compute_shiftstate(void);

static int chg_bit(int bit, void *addr);

static int test_bit(int bit, void *addr);

static int test_and_set_bit(int bit, void *addr);

static int test_and_clear_bit(int bit, void *addr);

static long shift_state = 0;

static long lockstate = 0;

static long slockstate = 0;

static int capslock_state = 0;

static int insert_state = 0;

int scan_numlock_state = 0;

/*static int meta_state = 0; */
static int applic_mode = 0;

static int dead_key_next = 0;

static int diacr = 0;

static int npadch = -1;		/* -1 or number assembled on pad */

static int rep = 0;

static int kbd_translate(unsigned char scancode, unsigned char *keycode);

static char kbd_unexpected_up(unsigned char keycode);

static void put_queue(long key);

static void put_fqueue(long key);

static void put_aqueue(long key);

static void put_acqueue(long key);

static void puts_queue(char *str);

static long get_queue(void);

static unsigned char handle_diacr(unsigned char);

static void to_utf8(unsigned short c);

static void applkey(int key, char mode);

static void set_console(int no, int diff);

static void set_kbd_led(void);

typedef void (*k_hand) (unsigned char value, char up_flag);

typedef void (k_handfn) (unsigned char value, char up_flag);

static k_handfn do_self, do_fn, do_spec, do_pad, do_dead, do_cons, do_cur, do_shift, do_meta, do_ascii, do_lock, do_lowercase, do_slock, do_dead2, do_ignore;

static k_hand key_handler[16] = {
   do_self, do_fn, do_spec, do_pad, do_dead, do_cons, do_cur, do_shift,
   do_meta, do_ascii, do_lock, do_lowercase, do_slock, do_dead2,
   do_ignore, do_ignore
};

#ifdef DBG
static char *key_handler_names[16] = {
   "do_self", "do_fn", "do_spec", "do_pad",
   "do_dead", "do_cons", "do_cur", "do_shift",
   "do_meta", "do_ascii", "do_lock", "do_lowercase",
   "do_slock", "do_dead2", "do_ignore", "do_ignore"
};

#endif

typedef void (*void_fnp) (void);

typedef void (void_fn) (void);

static void_fn do_null, enter, show_ptregs, send_intr, lastcons, caps_toggle,
   num, hold, scroll_forw, scroll_back, boot_it, caps_on, compose, SAK, decr_console, incr_console, spawn_console, bare_num, show_mem, show_state;

static void_fnp spec_fn_table[] = {
   do_null, enter, show_ptregs, show_mem,
   show_state, send_intr, lastcons, caps_toggle,
   num, hold, scroll_forw, scroll_back,
   boot_it, caps_on, compose, SAK,
   decr_console, incr_console, spawn_console, bare_num
};

#ifdef DBG
static char *spec_fn_names[] = {
   "do_null", "enter", "show_ptregs", "show_mem",
   "show_state", "send_intr", "lastcons", "caps_toggle",
   "num", "hold", "scroll_forw", "scroll_back",
   "boot_it", "caps_on", "compose", "SAK",
   "decr_console", "incr_console", "spawn_console", "bare_num"
};

#endif

/* maximum values each key_handler can handle */
static const int max_vals[] = {
   255, SIZE(func_table) - 1, SIZE(spec_fn_table) - 1, NR_PAD - 1,
   NR_DEAD - 1, 255, 3, NR_SHIFT - 1,
   255, NR_ASCII - 1, NR_LOCK - 1, 255,
   NR_LOCK - 1, 255
};

const int NR_TYPES = SIZE(max_vals);

static long *queue_beg = 0, *queue_end = 0, *queue_max = 0;

#include <scankey/static_put_fqueue.c>
#include <scankey/static_put_aqueue.c>
#include <scankey/static_put_acqueue.c>
#include <scankey/static_put_queue.c>
#include <scankey/static_puts_queue.c>
#include <scankey/static_get_queue.c>
#include <scankey/scan_state.c>
#include <scankey/set_scan_state.c>
#include <scankey/scan_check.c>
#include <scankey/scan_key.c>

static int first_key = 0;

#include <scankey/scan_reset.c>
#include <scankey/scan_push.c>

/*
 * Translation of escaped scancodes to keycodes.
 * This is now user-settable.
 * The keycodes 1-88,96-111,119 are fairly standard, and
 * should probably not be changed - changing might confuse X.
 * X also interprets scancode 0x5d (KEY_Begin).
 *
 * For 1-88 keycode equals scancode.
 */

#define E0_KPENTER 96
#define E0_RCTRL   97
#define E0_KPSLASH 98
#define E0_PRSCR   99
#define E0_RALT    100
#define E0_BREAK   101		/* (control-pause) */
#define E0_HOME    102
#define E0_UP      103
#define E0_PGUP    104
#define E0_LEFT    105
#define E0_RIGHT   106
#define E0_END     107
#define E0_DOWN    108
#define E0_PGDN    109
#define E0_INS     110
#define E0_DEL     111

#define E1_PAUSE   119

/*
 * The keycodes below are randomly located in 89-95,112-118,120-127.
 * They could be thrown away (and all occurrences below replaced by 0),
 * but that would force many users to use the `setkeycodes' utility, where
 * they needed not before. It does not matter that there are duplicates, as
 * long as no duplication occurs for any single keyboard.
 */
#define SC_LIM 89

#define FOCUS_PF1 85		/* actual code! */
#define FOCUS_PF2 89
#define FOCUS_PF3 90
#define FOCUS_PF4 91
#define FOCUS_PF5 92
#define FOCUS_PF6 93
#define FOCUS_PF7 94
#define FOCUS_PF8 95
#define FOCUS_PF9 120
#define FOCUS_PF10 121
#define FOCUS_PF11 122
#define FOCUS_PF12 123

#define JAP_86     124
/* tfj@olivia.ping.dk:
 * The four keys are located over the numeric keypad, and are
 * labelled A1-A4. It's an rc930 keyboard, from
 * Regnecentralen/RC International, Now ICL.
 * Scancodes: 59, 5a, 5b, 5c.
 */
#define RGN1 124
#define RGN2 125
#define RGN3 126
#define RGN4 127

/* BTC */
#define E0_MACRO   112
/* LK450 */
#define E0_F13     113
#define E0_F14     114
#define E0_HELP    115
#define E0_DO      116
#define E0_F17     117
#define E0_KPMINPLUS 118
/*
 * My OmniKey generates e0 4c for  the "OMNI" key and the
 * right alt key does nada. [kkoller@nyx10.cs.du.edu]
 */
#define E0_OK	124
/*
 * New microsoft keyboard is rumoured to have
 * e0 5b (left window button), e0 5c (right window button),
 * e0 5d (menu button). [or: LBANNER, RBANNER, RMENU]
 * [or: Windows_L, Windows_R, TaskMan]
 */
#define E0_MSLW	125
#define E0_MSRW	126
#define E0_MSTM	127

static unsigned char high_keys[128 - SC_LIM] = {
   RGN1, RGN2, RGN3, RGN4, 0, 0, 0,	/* 0x59-0x5f */
   0, 0, 0, 0, 0, 0, 0, 0,	/* 0x60-0x67 */
   0, 0, 0, 0, 0, FOCUS_PF11, 0, FOCUS_PF12,	/* 0x68-0x6f */
   0, 0, 0, FOCUS_PF2, FOCUS_PF9, 0, 0, FOCUS_PF3,	/* 0x70-0x77 */
   FOCUS_PF4, FOCUS_PF5, FOCUS_PF6, FOCUS_PF7,	/* 0x78-0x7b */
   FOCUS_PF8, JAP_86, FOCUS_PF10, 0	/* 0x7c-0x7f */
};

static unsigned char e0_keys[128] = {
   0, 0, 0, 0, 0, 0, 0, 0,	/* 0x00-0x07 */
   0, 0, 0, 0, 0, 0, 0, 0,	/* 0x08-0x0f */
   0, 0, 0, 0, 0, 0, 0, 0,	/* 0x10-0x17 */
   0, 0, 0, 0, E0_KPENTER, E0_RCTRL, 0, 0,	/* 0x18-0x1f */
   0, 0, 0, 0, 0, 0, 0, 0,	/* 0x20-0x27 */
   0, 0, 0, 0, 0, 0, 0, 0,	/* 0x28-0x2f */
   0, 0, 0, 0, 0, E0_KPSLASH, 0, E0_PRSCR,	/* 0x30-0x37 */
   E0_RALT, 0, 0, 0, 0, E0_F13, E0_F14, E0_HELP,	/* 0x38-0x3f */
   E0_DO, E0_F17, 0, 0, 0, 0, E0_BREAK, E0_HOME,	/* 0x40-0x47 */
   E0_UP, E0_PGUP, 0, E0_LEFT, E0_OK, E0_RIGHT, E0_KPMINPLUS, E0_END,	/* 0x48-0x4f */
   E0_DOWN, E0_PGDN, E0_INS, E0_DEL, 0, 0, 0, 0,	/* 0x50-0x57 */
   0, 0, 0, E0_MSLW, E0_MSRW, E0_MSTM, 0, 0,	/* 0x58-0x5f */
   0, 0, 0, 0, 0, 0, 0, 0,	/* 0x60-0x67 */
   0, 0, 0, 0, 0, 0, 0, E0_MACRO,	/* 0x68-0x6f */
   0, 0, 0, 0, 0, 0, 0, 0,	/* 0x70-0x77 */
   0, 0, 0, 0, 0, 0, 0, 0	/* 0x78-0x7f */
};

#include <scankey/static_kbd_translate.c>
#include <scankey/static_test_bit.c>
#include <scankey/static_test_and_set_bit.c>
#include <scankey/static_chg_bit.c>
#include <scankey/static_test_and_clear_bit.c>
#include <scankey/static_kbd_unexpected_up.c>
#include <scankey/static_compute_shiftstate.c>
#include <scankey/static_do_self.c>
#include <scankey/static_handle_diacr.c>
#include <scankey/static_to_utf8.c>
#include <scankey/static_do_fn.c>
#include <scankey/static_do_spec.c>
#include <scankey/static_applkey.c>
#include <scankey/static_do_pad.c>
#include <scankey/static_do_cur.c>

#define A_GRAVE  '`'
#define A_ACUTE  '\''
#define A_CFLEX  '^'
#define A_TILDE  '~'
#define A_DIAER  '"'
#define A_CEDIL  ','
static unsigned char ret_diacr[NR_DEAD] = { A_GRAVE, A_ACUTE, A_CFLEX, A_TILDE, A_DIAER, A_CEDIL };

#include <scankey/static_do_dead.c>
#include <scankey/static_do_dead2.c>
#include <scankey/static_do_cons.c>
#include <scankey/static_set_console.c>
#include <scankey/static_do_shift.c>
#include <scankey/static_do_meta.c>
#include <scankey/static_do_ascii.c>
#include <scankey/static_do_lock.c>
#include <scankey/static_do_slock.c>
#include <scankey/static_do_lowercase.c>
#include <scankey/static_do_ignore.c>
#include <scankey/static_do_null.c>
#include <scankey/static_enter.c>
#include <scankey/static_show_ptregs.c>
#include <scankey/static_show_mem.c>
#include <scankey/static_show_state.c>
#include <scankey/static_send_intr.c>
#include <scankey/static_hold.c>
#include <scankey/static_num.c>
#include <scankey/static_bare_num.c>
#include <scankey/static_lastcons.c>
#include <scankey/static_decr_console.c>
#include <scankey/static_incr_console.c>
#include <scankey/static_caps_toggle.c>
#include <scankey/static_caps_on.c>
#include <scankey/static_scroll_forw.c>
#include <scankey/static_scroll_back.c>
#include <scankey/static_boot_it.c>
#include <scankey/static_compose.c>
#include <scankey/static_SAK.c>
#include <scankey/static_spawn_console.c>

#ifdef OS_LINUX
#define KDGETLED	0x4B31	/* return current led state */
#define KDSETLED	0x4B32	/* set led state [lights, not flags] */
#define 	LED_SCR		0x01	/* scroll lock led */
#define 	LED_NUM		0x02	/* num lock led */
#define 	LED_CAP		0x04	/* caps lock led */
#endif

#include <scankey/static_set_kbd_led.c>
