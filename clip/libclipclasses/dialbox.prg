/*
    Copyright (C) 2001  ITK
    Author   : Alena (alena@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
******************************************************************
#include "ci_inkey.ch"
#include "ci_box.ch"	//1   2	   3    4   5   6   7	 8   9	 0
******************************************************************
#define DBCOLORS	"0/3,1/3,14/3,14/1,1/3,14/1,0/3,14/1,9/3,14/1"
//#define DBCOLORS	"0/7,1/7,14/7,14/1,1/7,3/7,0/7,14/1,9/7,14/1"
#define _BUTTON_	1
#define _GETB_		2
#define _CHECK_		3
#define _RADIO_		4
#define _GETTEXT_	5
#define TABSIZE		8
******************************************************************
/*
say, get, qout
color="col1, col2, col3, col4, col5, col6, col7, col8"
col1 - ������� �����
col2 - ���������� �����
col3 - ������
col4 - �������� ������
col5 - get ������ �� ��������
col6 - get ������ ��������
col7 - checkbox ������ �� ��������
col8 - checkbox ������ ��������
col9 - radiogroup ������ �� ��������
col10 - radiogroup ������ ��������
*/
******************************************************************
#include "dialbox/static___setcolor.ch"
#include "dialbox/static_db_backspace.ch"
#include "dialbox/static_db_bottom.ch"
#include "dialbox/static_db_button.ch"
#include "dialbox/static_db_cancelblock.ch"
#include "dialbox/static_db_check.ch"
#include "dialbox/static_db_check_line.ch"
#include "dialbox/static_db_close.ch"
#include "dialbox/static_db_curword.ch"
#include "dialbox/static_db_delete.ch"
#include "dialbox/static_db_destroy.ch"
#include "dialbox/static_db_down.ch"
#include "dialbox/static_db_drawbox.ch"
#include "dialbox/static_db_end.ch"
#include "dialbox/static_db_enter.ch"
#include "dialbox/static_db_find.ch"
#include "dialbox/static_db_get.ch"
#include "dialbox/static_db_getstrelem.ch"
#include "dialbox/static_db_getstring.ch"
#include "dialbox/static_db_gettext.ch"
#include "dialbox/static_db_goitem.ch"
#include "dialbox/static_db_goto.ch"
#include "dialbox/static_db_handlekey.ch"
#include "dialbox/static_db_home.ch"
#include "dialbox/static_db_init.ch"
#include "dialbox/static_db_insert.ch"
#include "dialbox/static_db_killfocus.ch"
#include "dialbox/static_db_left.ch"
#include "dialbox/static_db_makestr.ch"
#include "dialbox/static_db_menu_create.ch"
#include "dialbox/static_db_pagedown.ch"
#include "dialbox/static_db_pageup.ch"
#include "dialbox/static_db_radio.ch"
#include "dialbox/static_db_refresh.ch"
#include "dialbox/static_db_right.ch"
#include "dialbox/static_db_select.ch"
#include "dialbox/static_db_setvalue.ch"
#include "dialbox/static_db_text.ch"
#include "dialbox/static_db_top.ch"
#include "dialbox/static_db_up.ch"
******************************************************************
#include "dialbox/_recover_dialogbox.ch"
#include "dialbox/dialogbox.ch"
******************************************************************
