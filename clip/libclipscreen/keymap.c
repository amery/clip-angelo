
/*
   $Log: keymap.c,v $
   Revision 1.1  2006/06/22 19:35:31  itk
   uri: init sf.net repository

   Revision 1.4  2005/07/06 15:30:28  clip
   uri: strange trouble with new util loadkeys and old kernel. not fixed goodly.

   Revision 1.3  2001/05/15 10:34:28  clip
   memdebug
   paul

   Revision 1.2  2000/12/02 12:46:57  clip
   scancodes
   paul

   Revision 1.1  2000/11/30 14:57:15  clip
   scancode support
   paul

 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ci_clip.h>
#include <ci_scankey.h>

#define MAX_NR_FUNC	256	/* max nr of strings assigned to keys */
#define MAX_DIACR	256
#define MAX_NR_KEYMAPS	256
#define NR_KEYS		128

#include <keymap/struct_kbdiacr.h>

static char *funcbuf = 0;

static int funcbufsize = 0;

char *func_table[MAX_NR_FUNC];

unsigned int keymap_count;

unsigned short *key_maps[MAX_NR_KEYMAPS];

unsigned short plain_map[NR_KEYS];

struct kbdiacr accent_table[MAX_DIACR];

unsigned int accent_table_size;

#ifndef LIBROOT
#define LIBROOT "/usr/local/lib/clip"
#endif

#ifndef CLIP_KEYMAP_ROOT
#define CLIP_KEYMAP_ROOT  LIBROOT "/keymaps"
#endif

static int line_no = 0;

#include <keymap/static_get_str.c>
#include <keymap/load_keymap.c>
