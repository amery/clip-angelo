/*   MEDIT class                                                */
/*                                                              */
/*   Copyright (C) 2001 - 2004  IT                              */
/*   Author  : Elena Kornilova (alena@itk.ru)                   */
/*   Licence : (GPL) http://www.itk.ru/clipper/licence.html     */

******************************************************************
#include "ci_config.ch"
#include "ci_set.ch"
#include "ci_button.ch"   // Menu definitions
#include "ci_inkey.ch"    // Key definitions
#include "ci_edit.ch"
#include "ci_box.ch"
#include <ci_clipcfg.h>
#include <ci_setcurs.ch>
******************************************************************
#define FA_NORMAL       0
#define FA_READONLY     1
#define FA_HIDDEN       2
#define FA_SYSTEM       4
#define FA_VOLUME       8
#define FA_DIRECTORY    16
#define FA_ARCHIVE      32
#define ME_VERSION      6
******************************************************************
static __CurrentDir
******************************************************************
#include "edit/static_fget.ch"
#include "edit/static_me___addmacrotomenu.ch"
#include "edit/static_me___check_share.ch"
#include "edit/static_me___checkwindow.ch"
#include "edit/static_me___setdefaulthash.ch"
#include "edit/static_me___setfocus.ch"
#include "edit/static_me___setmacros.ch"
#include "edit/static_me__initusermacro.ch"
#include "edit/static_me_about.ch"
#include "edit/static_me_applyhash.ch"
#include "edit/static_me_backspace.ch"
#include "edit/static_me_callmenu.ch"
#include "edit/static_me_callpopupmenu.ch"
#include "edit/static_me_cancelblock.ch"
#include "edit/static_me_centerline.ch"
#include "edit/static_me_changecharset.ch"
#include "edit/static_me_choosecodepage.ch"
#include "edit/static_me_closewindow.ch"
#include "edit/static_me_copyblock.ch"
#include "edit/static_me_copytoclipboard.ch"
#include "edit/static_me_createnewwindow.ch"
#include "edit/static_me_delete.ch"
#include "edit/static_me_deleteblock.ch"
#include "edit/static_me_deleteend.ch"
#include "edit/static_me_deletehome.ch"
#include "edit/static_me_deleteline.ch"
#include "edit/static_me_drawhead.ch"
#include "edit/static_me_editopt.ch"
#include "edit/static_me_enableblockmenu.ch"
#include "edit/static_me_enablemenu.ch"
#include "edit/static_me_findnext.ch"
#include "edit/static_me_findprev.ch"
#include "edit/static_me_findreplace.ch"
#include "edit/static_me_findstring.ch"
#include "edit/static_me_format.ch"
#include "edit/static_me_getfocus.ch"
#include "edit/static_me_getreadonly.ch"
#include "edit/static_me_getstatus.ch"
#include "edit/static_me_goto.ch"
#include "edit/static_me_helpedit.ch"
#include "edit/static_me_initme.ch"
#include "edit/static_me_insmacro.ch"
#include "edit/static_me_instemplate.ch"
#include "edit/static_me_listfiles.ch"
#include "edit/static_me_loadblock.ch"
#include "edit/static_me_markblock.ch"
#include "edit/static_me_matchstructure.ch"
#include "edit/static_me_matchsymbol.ch"
#include "edit/static_me_menu_create.ch"
#include "edit/static_me_newline.ch"
#include "edit/static_me_nextwindow.ch"
#include "edit/static_me_openfile.ch"
#include "edit/static_me_pastefromclipboard.ch"
#include "edit/static_me_prevwindow.ch"
#include "edit/static_me_print.ch"
#include "edit/static_me_reloadfile.ch"
#include "edit/static_me_removewindow.ch"
#include "edit/static_me_runme.ch"
#include "edit/static_me_save_options.ch"
#include "edit/static_me_saveblock.ch"
#include "edit/static_me_savefile.ch"
#include "edit/static_me_savepos.ch"
#include "edit/static_me_savewins.ch"
#include "edit/static_me_set_options.ch"
#include "edit/static_me_setdraw.ch"
#include "edit/static_me_sethash.ch"
#include "edit/static_me_setinsertmode.ch"
#include "edit/static_me_setreadonly.ch"
#include "edit/static_me_showfiles.ch"
#include "edit/static_me_undo.ch"
#include "edit/static_me_writestatus.ch"
#include "edit/static_msg.ch"
******************************************************************
#include "edit/medit.ch"
#include "edit/medit_new.ch"
#include "edit/_medit.ch"
******************************************************************
