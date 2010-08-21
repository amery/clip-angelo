/*   TEXTEDIT class                                             */
/*                                                              */
/*   Copyright (C) 2001-2204  ITK                               */
/*   Author  : Elena Kornilova (alena@itk.ru)                   */
/*   Licence : (GPL) http://www.itk.ru/clipper/licence.html     */

**********************************************************************
#include <ci_edit.ch>
#include <ci_box.ch>
#include <ci_fileio.ch>
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
#include <tedit_obj/static___te_nullInit.prg>
#include <tedit_obj/static___te_setcolor.prg>
#include <tedit_obj/static_stripSpace.prg>
#include <tedit_obj/static_te___autoWrap.prg>
#include <tedit_obj/static_te___setDefaultKey.prg>
#include <tedit_obj/static_te_addToClipboard.prg>
#include <tedit_obj/static_te_applyKey.prg>
#include <tedit_obj/static_te_around_check.prg>
#include <tedit_obj/static_te_backSpace.prg>
#include <tedit_obj/static_te_beginBlock.prg>
#include <tedit_obj/static_te_cancelBlock.prg>
#include <tedit_obj/static_te_centerLine.prg>
#include <tedit_obj/static_te_check_clipchs.prg>
#include <tedit_obj/static_te_check_line.prg>
#include <tedit_obj/static_te_clear.prg>
#include <tedit_obj/static_te_cleft.prg>
#include <tedit_obj/static_te_copyBlock.prg>
#include <tedit_obj/static_te_copyToClipboard.prg>
#include <tedit_obj/static_te_cright.prg>
#include <tedit_obj/static_te_curWord.prg>
#include <tedit_obj/static_te_delEnd.prg>
#include <tedit_obj/static_te_deleteBlock.prg>
#include <tedit_obj/static_te_deleteLine.prg>
#include <tedit_obj/static_te_delHome.prg>
#include <tedit_obj/static_te_delRight.prg>
#include <tedit_obj/static_te_delWordLeft.prg>
#include <tedit_obj/static_te_delWordRight.prg>
#include <tedit_obj/static_te_down.prg>
#include <tedit_obj/static_te_draw.prg>
#include <tedit_obj/static_te_end.prg>
#include <tedit_obj/static_te_endBlock.prg>
#include <tedit_obj/static_te_find.prg>
#include <tedit_obj/static_te_findNext.prg>
#include <tedit_obj/static_te_findPrev.prg>
#include <tedit_obj/static_te_formatLine.prg>
#include <tedit_obj/static_te_formatPart.prg>
#include <tedit_obj/static_te_Frmt.prg>
#include <tedit_obj/static_te_goBottom.prg>
#include <tedit_obj/static_te_gotoLine.prg>
#include <tedit_obj/static_te_goTop.prg>
#include <tedit_obj/static_te_gotoPos.prg>
#include <tedit_obj/static_te_handleKey.prg>
#include <tedit_obj/static_te_highLightAdd.prg>
#include <tedit_obj/static_te_highLightDel.prg>
#include <tedit_obj/static_te_Home.prg>
#include <tedit_obj/static_te_Identity.prg>
#include <tedit_obj/static_te_insert.prg>
#include <tedit_obj/static_te_insertLine.prg>
#include <tedit_obj/static_te_insMacro.prg>
#include <tedit_obj/static_te_insTempl.prg>
#include <tedit_obj/static_te_killNumStyle.prg>
#include <tedit_obj/static_te_loadBlock.prg>
#include <tedit_obj/static_te_loadFile.prg>
#include <tedit_obj/static_te_loadString.prg>
#include <tedit_obj/static_te_matchStruct.prg>
#include <tedit_obj/static_te_mgoto.prg>
#include <tedit_obj/static_te_moveBlock.prg>
#include <tedit_obj/static_te_moveToClipboard.prg>
#include <tedit_obj/static_te_newLine.prg>
#include <tedit_obj/static_te_nextPage.prg>
#include <tedit_obj/static_te_nextWord.prg>
#include <tedit_obj/static_te_overStrike.prg>
#include <tedit_obj/static_te_PageDown.prg>
#include <tedit_obj/static_te_PageUp.prg>
#include <tedit_obj/static_te_panDown.prg>
#include <tedit_obj/static_te_panEnd.prg>
#include <tedit_obj/static_te_panHome.prg>
#include <tedit_obj/static_te_panLeft.prg>
#include <tedit_obj/static_te_panRight.prg>
#include <tedit_obj/static_te_panUp.prg>
#include <tedit_obj/static_te_pasteFromClipboard.prg>
#include <tedit_obj/static_te_prevPage.prg>
#include <tedit_obj/static_te_prevWord.prg>
#include <tedit_obj/static_te_print.prg>
#include <tedit_obj/static_te_printBlock.prg>
#include <tedit_obj/static_te_refresh.prg>
#include <tedit_obj/static_te_refreshStr.prg>
#include <tedit_obj/static_te_reLoadFile.prg>
#include <tedit_obj/static_te_replace.prg>
#include <tedit_obj/static_te_saveBlock.prg>
#include <tedit_obj/static_te_saveFile.prg>
#include <tedit_obj/static_te_saveString.prg>
#include <tedit_obj/static_te_setCharset.prg>
#include <tedit_obj/static_te_setFocus.prg>
#include <tedit_obj/static_te_setKey.prg>
#include <tedit_obj/static_te_setNewColor.prg>
#include <tedit_obj/static_te_setNumStyle.prg>
#include <tedit_obj/static_te_setTabSize.prg>
#include <tedit_obj/static_te_undo.prg>
#include <tedit_obj/static_te_up.prg>
#include <tedit_obj/static_te_wordLeft.prg>
#include <tedit_obj/static_te_wordRight.prg>
#include <tedit_obj/static_te_writeundo.prg>
**********************************************************************
#include <tedit_obj/_recover_textedit.prg>
#include <tedit_obj/textEditNew.prg>
**********************************************************************
