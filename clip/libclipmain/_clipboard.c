/*
	Copyright (C) 2001  ITK
	Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>, Sergey Rosenfeld <serg@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include <stdlib.h>
#include "ci_clip.h"
#include "ci_error.ch"

#ifdef _WIN32
#ifdef OS_CYGWIN		/* MingW port automatically #includes <windows.h> */
#include <w32api/windows.h>
#endif
#else
#include <unistd.h>
#include "ci_clipbrd.ch"
static unsigned char *_clip_clipboard_data[CF_MAX];

static unsigned char _clip_clipboard_len[CF_MAX];
#endif
#include "_clipboard/static_from_clipboard.h"
#include "_clipboard/static_to_clipboard.h"

#include "_clipboard/clip_GETCLIPBOARDDATA.h"
#include "_clipboard/clip_SETCLIPBOARDDATA.h"
#include "_clipboard/clip_ISCLIPBOARDFORMATAVAILABLE.h"
/* win32 */
#include "_clipboard/static__clip_win_cp.h"

#include "_clipboard/clip_GETCONSOLECP.h"
#include "_clipboard/clip_GETANSICP.h"
#include "_clipboard/clip_GETACP.h"
#include "_clipboard/clip_GETOEMCP.h"
#include "_clipboard/clip_GETCONSOLEOUTPUTCP.h"
