/*
	Copyright (C) 2001  ITK
	Author   : Uri (uri@itk.ru)
	License : (GPL) http://www.itk.ru/clipper/license.html

	��������� � ��������
	    refreshCurrent
	    refreshAll
	    cleft
	    cright
	    stabilize
	    �������  panRight �������� �� ������ ���������

	20.07.2004 john (sneshka@online.bryansk.ru)

*/
**********************************************************************
#include "ci_tbrowse.ch"
#include "ci_inkey.ch"
#include "ci_button.ch"

//#define TBR_CACHED
//#define DEBUG_CALL
**********************************************************************
#include "tb_obj/static___applykey.ch"
#include "tb_obj/static___delAllColumns.ch"
#include "tb_obj/static___dummyRow.ch"
#include "tb_obj/static___getcolor.ch"
#include "tb_obj/static___lenColSep.ch"
#include "tb_obj/static___remakeColumns.ch"
#include "tb_obj/static___sayTable.ch"
#include "tb_obj/static___setcolor.ch"
#include "tb_obj/static___tdbskip.ch"
#include "tb_obj/static___whoVisible.ch"
#include "tb_obj/static__outCurrent.ch"
#include "tb_obj/static_addColumn.ch"
#include "tb_obj/static_cleft.ch"
#include "tb_obj/static_colorRect.ch"
#include "tb_obj/static_colWidth.ch"
#include "tb_obj/static_configure.ch"
#include "tb_obj/static_cright.ch"
#include "tb_obj/static_deHilite.ch"
#include "tb_obj/static_delColumn.ch"
#include "tb_obj/static_down.ch"
#include "tb_obj/static_end.ch"
#include "tb_obj/static_forceStable.ch"
#include "tb_obj/static_getColumn.ch"
#include "tb_obj/static_goBottom.ch"
#include "tb_obj/static_goTop.ch"
#include "tb_obj/static_hilite.ch"
#include "tb_obj/static_home.ch"
#include "tb_obj/static_insColumn.ch"
#include "tb_obj/static_invalidate.ch"
#include "tb_obj/static_pageDown.ch"
#include "tb_obj/static_pageUp.ch"
#include "tb_obj/static_panEnd.ch"
#include "tb_obj/static_panHome.ch"
#include "tb_obj/static_panLeft.ch"
#include "tb_obj/static_panRight.ch"
#include "tb_obj/static_refreshAll.ch"
#include "tb_obj/static_refreshCurrent.ch"
#include "tb_obj/static_setColumn.ch"
#include "tb_obj/static_stabilize.ch"
#include "tb_obj/static_tb_hitTest.ch"
#include "tb_obj/static_tb_set_default_keys.ch"
#include "tb_obj/static_tb_setkey.ch"
#include "tb_obj/static_up.ch"
**********************************************************************
#include "tb_obj/_recover_tbrowse.ch"
#include "tb_obj/TBrowseDB.ch"
#include "tb_obj/TBrowseNew.ch"
**********************************************************************
