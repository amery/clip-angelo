/*   MEDIT class                                                */
/*                                                              */
/*   Copyright (C) 2001 - 2004  IT                              */
/*   Author  : Elena Kornilova (alena@itk.ru)                   */
/*   Licence : (GPL) http://www.itk.ru/clipper/licence.html     */

******************************************************************
#include <ci_config.ch>
#include <ci_set.ch>
#include <ci_button.ch>   // Menu definitions
#include <ci_inkey.ch>    // Key definitions
#include <ci_edit.ch>
#include <ci_box.ch>
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
#include <edit/static_fget.prg>
#include <edit/static_me___addmacrotomenu.prg>
#include <edit/static_me___check_share.prg>
#include <edit/static_me___checkwindow.prg>
#include <edit/static_me___setdefaulthash.prg>
#include <edit/static_me___setfocus.prg>
#include <edit/static_me___setmacros.prg>
#include <edit/static_me__initusermacro.prg>
#include <edit/static_me_about.prg>
#include <edit/static_me_applyhash.prg>
#include <edit/static_me_backspace.prg>
#include <edit/static_me_callmenu.prg>
#include <edit/static_me_callpopupmenu.prg>
#include <edit/static_me_cancelblock.prg>
#include <edit/static_me_centerline.prg>
#include <edit/static_me_changecharset.prg>
#include <edit/static_me_choosecodepage.prg>
#include <edit/static_me_closewindow.prg>
#include <edit/static_me_copyblock.prg>
#include <edit/static_me_copytoclipboard.prg>
#include <edit/static_me_createnewwindow.prg>
#include <edit/static_me_delete.prg>
#include <edit/static_me_deleteblock.prg>
#include <edit/static_me_deleteend.prg>
#include <edit/static_me_deletehome.prg>
#include <edit/static_me_deleteline.prg>
#include <edit/static_me_drawhead.prg>
#include <edit/static_me_editopt.prg>
#include <edit/static_me_enableblockmenu.prg>
#include <edit/static_me_enablemenu.prg>
#include <edit/static_me_findnext.prg>
#include <edit/static_me_findprev.prg>
#include <edit/static_me_findreplace.prg>
#include <edit/static_me_findstring.prg>
#include <edit/static_me_format.prg>
#include <edit/static_me_getfocus.prg>
#include <edit/static_me_getreadonly.prg>
#include <edit/static_me_getstatus.prg>
#include <edit/static_me_goto.prg>
#include <edit/static_me_helpedit.prg>
#include <edit/static_me_initme.prg>
#include <edit/static_me_insmacro.prg>
#include <edit/static_me_instemplate.prg>
#include <edit/static_me_listfiles.prg>
#include <edit/static_me_loadblock.prg>
#include <edit/static_me_markblock.prg>
#include <edit/static_me_matchstructure.prg>
#include <edit/static_me_matchsymbol.prg>
#include <edit/static_me_menu_create.prg>
#include <edit/static_me_newline.prg>
#include <edit/static_me_nextwindow.prg>
#include <edit/static_me_openfile.prg>
#include <edit/static_me_pastefromclipboard.prg>
#include <edit/static_me_prevwindow.prg>
#include <edit/static_me_print.prg>
#include <edit/static_me_reloadfile.prg>
#include <edit/static_me_removewindow.prg>
#include <edit/static_me_runme.prg>
#include <edit/static_me_save_options.prg>
#include <edit/static_me_saveblock.prg>
#include <edit/static_me_savefile.prg>
#include <edit/static_me_savepos.prg>
#include <edit/static_me_savewins.prg>
#include <edit/static_me_set_options.prg>
#include <edit/static_me_setdraw.prg>
#include <edit/static_me_sethash.prg>
#include <edit/static_me_setinsertmode.prg>
#include <edit/static_me_setreadonly.prg>
#include <edit/static_me_showfiles.prg>
#include <edit/static_me_undo.prg>
#include <edit/static_me_writestatus.prg>
#include <edit/static_msg.prg>
******************************************************************
#include <edit/medit.prg>
#include <edit/medit_new.prg>
#include <edit/_medit.prg>
******************************************************************
