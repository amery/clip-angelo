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
#include <ci_tbrowse.ch>
#include <ci_inkey.ch>
#include <ci_button.ch>

//#define TBR_CACHED
//#define DEBUG_CALL
**********************************************************************
#include <tb_obj/static___applykey.prg>
#include <tb_obj/static___delAllColumns.prg>
#include <tb_obj/static___dummyRow.prg>
#include <tb_obj/static___getcolor.prg>
#include <tb_obj/static___lenColSep.prg>
#include <tb_obj/static___remakeColumns.prg>
#include <tb_obj/static___sayTable.prg>
#include <tb_obj/static___setcolor.prg>
#include <tb_obj/static___tdbskip.prg>
#include <tb_obj/static___whoVisible.prg>
#include <tb_obj/static__outCurrent.prg>
#include <tb_obj/static_addColumn.prg>
#include <tb_obj/static_cleft.prg>
#include <tb_obj/static_colorRect.prg>
#include <tb_obj/static_colWidth.prg>
#include <tb_obj/static_configure.prg>
#include <tb_obj/static_cright.prg>
#include <tb_obj/static_deHilite.prg>
#include <tb_obj/static_delColumn.prg>
#include <tb_obj/static_down.prg>
#include <tb_obj/static_end.prg>
#include <tb_obj/static_forceStable.prg>
#include <tb_obj/static_getColumn.prg>
#include <tb_obj/static_goBottom.prg>
#include <tb_obj/static_goTop.prg>
#include <tb_obj/static_hilite.prg>
#include <tb_obj/static_home.prg>
#include <tb_obj/static_insColumn.prg>
#include <tb_obj/static_invalidate.prg>
#include <tb_obj/static_pageDown.prg>
#include <tb_obj/static_pageUp.prg>
#include <tb_obj/static_panEnd.prg>
#include <tb_obj/static_panHome.prg>
#include <tb_obj/static_panLeft.prg>
#include <tb_obj/static_panRight.prg>
#include <tb_obj/static_refreshAll.prg>
#include <tb_obj/static_refreshCurrent.prg>
#include <tb_obj/static_setColumn.prg>
#include <tb_obj/static_stabilize.prg>
#include <tb_obj/static_tb_hitTest.prg>
#include <tb_obj/static_tb_set_default_keys.prg>
#include <tb_obj/static_tb_setkey.prg>
#include <tb_obj/static_up.prg>
**********************************************************************
#include <tb_obj/_recover_tbrowse.prg>
#include <tb_obj/TBrowseDB.prg>
#include <tb_obj/TBrowseNew.prg>
**********************************************************************
