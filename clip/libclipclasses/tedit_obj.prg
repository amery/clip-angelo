/*   TEXTEDIT class                                             */
/*                                                              */
/*   Copyright (C) 2001-2204  ITK                               */
/*   Author  : Elena Kornilova (alena@itk.ru)                   */
/*   Licence : (GPL) http://www.itk.ru/clipper/licence.html     */

**********************************************************************
#include "ci_edit.ch"
#include "ci_box.ch"
#include "ci_fileio.ch"
**********************************************************************

#define U_CMD           1
#define U_CYCLE         2
#define U_POS           3
#define U_LINE          4
#define U_LINES         5
#define U_COL           6
#define U_ROW           7
#define U_VALUE         8
#define U_BLOCK         9  //{type_block[0-no_block|1-str_block|2-rect_block], nt, nl, nb, nr}
#define U_MKBLOCK       10
#define U_FIND          11

#define FA_NORMAL       0
#define FA_READONLY     1
#define FA_HIDDEN       2
#define FA_SYSTEM       4
#define FA_VOLUME       8
#define FA_DIRECTORY    16
#define FA_ARCHIVE      32
**********************************************************************
#include "tedit_obj/static___te_nullInit.ch"
#include "tedit_obj/static___te_setcolor.ch"
#include "tedit_obj/static_stripSpace.ch"
#include "tedit_obj/static_te___autoWrap.ch"
#include "tedit_obj/static_te___setDefaultKey.ch"
#include "tedit_obj/static_te_addToClipboard.ch"
#include "tedit_obj/static_te_applyKey.ch"
#include "tedit_obj/static_te_around_check.ch"
#include "tedit_obj/static_te_backSpace.ch"
#include "tedit_obj/static_te_beginBlock.ch"
#include "tedit_obj/static_te_cancelBlock.ch"
#include "tedit_obj/static_te_centerLine.ch"
#include "tedit_obj/static_te_check_clipchs.ch"
#include "tedit_obj/static_te_check_line.ch"
#include "tedit_obj/static_te_clear.ch"
#include "tedit_obj/static_te_cleft.ch"
#include "tedit_obj/static_te_copyBlock.ch"
#include "tedit_obj/static_te_copyToClipboard.ch"
#include "tedit_obj/static_te_cright.ch"
#include "tedit_obj/static_te_curWord.ch"
#include "tedit_obj/static_te_delEnd.ch"
#include "tedit_obj/static_te_deleteBlock.ch"
#include "tedit_obj/static_te_deleteLine.ch"
#include "tedit_obj/static_te_delHome.ch"
#include "tedit_obj/static_te_delRight.ch"
#include "tedit_obj/static_te_delWordLeft.ch"
#include "tedit_obj/static_te_delWordRight.ch"
#include "tedit_obj/static_te_down.ch"
#include "tedit_obj/static_te_draw.ch"
#include "tedit_obj/static_te_end.ch"
#include "tedit_obj/static_te_endBlock.ch"
#include "tedit_obj/static_te_find.ch"
#include "tedit_obj/static_te_findNext.ch"
#include "tedit_obj/static_te_findPrev.ch"
#include "tedit_obj/static_te_formatLine.ch"
#include "tedit_obj/static_te_formatPart.ch"
#include "tedit_obj/static_te_Frmt.ch"
#include "tedit_obj/static_te_goBottom.ch"
#include "tedit_obj/static_te_gotoLine.ch"
#include "tedit_obj/static_te_goTop.ch"
#include "tedit_obj/static_te_gotoPos.ch"
#include "tedit_obj/static_te_handleKey.ch"
#include "tedit_obj/static_te_highLightAdd.ch"
#include "tedit_obj/static_te_highLightDel.ch"
#include "tedit_obj/static_te_Home.ch"
#include "tedit_obj/static_te_Identity.ch"
#include "tedit_obj/static_te_insert.ch"
#include "tedit_obj/static_te_insertLine.ch"
#include "tedit_obj/static_te_insMacro.ch"
#include "tedit_obj/static_te_insTempl.ch"
#include "tedit_obj/static_te_killNumStyle.ch"
#include "tedit_obj/static_te_loadBlock.ch"
#include "tedit_obj/static_te_loadFile.ch"
#include "tedit_obj/static_te_loadString.ch"
#include "tedit_obj/static_te_matchStruct.ch"
#include "tedit_obj/static_te_mgoto.ch"
#include "tedit_obj/static_te_moveBlock.ch"
#include "tedit_obj/static_te_moveToClipboard.ch"
#include "tedit_obj/static_te_newLine.ch"
#include "tedit_obj/static_te_nextPage.ch"
#include "tedit_obj/static_te_nextWord.ch"
#include "tedit_obj/static_te_overStrike.ch"
#include "tedit_obj/static_te_PageDown.ch"
#include "tedit_obj/static_te_PageUp.ch"
#include "tedit_obj/static_te_panDown.ch"
#include "tedit_obj/static_te_panEnd.ch"
#include "tedit_obj/static_te_panHome.ch"
#include "tedit_obj/static_te_panLeft.ch"
#include "tedit_obj/static_te_panRight.ch"
#include "tedit_obj/static_te_panUp.ch"
#include "tedit_obj/static_te_pasteFromClipboard.ch"
#include "tedit_obj/static_te_prevPage.ch"
#include "tedit_obj/static_te_prevWord.ch"
#include "tedit_obj/static_te_print.ch"
#include "tedit_obj/static_te_printBlock.ch"
#include "tedit_obj/static_te_refresh.ch"
#include "tedit_obj/static_te_refreshStr.ch"
#include "tedit_obj/static_te_reLoadFile.ch"
#include "tedit_obj/static_te_replace.ch"
#include "tedit_obj/static_te_saveBlock.ch"
#include "tedit_obj/static_te_saveFile.ch"
#include "tedit_obj/static_te_saveString.ch"
#include "tedit_obj/static_te_setCharset.ch"
#include "tedit_obj/static_te_setFocus.ch"
#include "tedit_obj/static_te_setKey.ch"
#include "tedit_obj/static_te_setNewColor.ch"
#include "tedit_obj/static_te_setNumStyle.ch"
#include "tedit_obj/static_te_setTabSize.ch"
#include "tedit_obj/static_te_undo.ch"
#include "tedit_obj/static_te_up.ch"
#include "tedit_obj/static_te_wordLeft.ch"
#include "tedit_obj/static_te_wordRight.ch"
#include "tedit_obj/static_te_writeundo.ch"
**********************************************************************
#include "tedit_obj/_recover_textedit.ch"
#include "tedit_obj/textEditNew.ch"
**********************************************************************
