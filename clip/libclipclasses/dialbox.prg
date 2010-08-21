/*
    Copyright (C) 2001  ITK
    Author   : Alena (alena@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
******************************************************************
#include <ci_inkey.ch>
#include <ci_box.ch>	//1   2	   3    4   5   6   7	 8   9	 0
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
#include <dialbox/static___setcolor.prg>
#include <dialbox/static_db_backspace.prg>
#include <dialbox/static_db_bottom.prg>
#include <dialbox/static_db_button.prg>
#include <dialbox/static_db_cancelblock.prg>
#include <dialbox/static_db_check.prg>
#include <dialbox/static_db_check_line.prg>
#include <dialbox/static_db_close.prg>
#include <dialbox/static_db_curword.prg>
#include <dialbox/static_db_delete.prg>
#include <dialbox/static_db_destroy.prg>
#include <dialbox/static_db_down.prg>
#include <dialbox/static_db_drawbox.prg>
#include <dialbox/static_db_end.prg>
#include <dialbox/static_db_enter.prg>
#include <dialbox/static_db_find.prg>
#include <dialbox/static_db_get.prg>
#include <dialbox/static_db_getstrelem.prg>
#include <dialbox/static_db_getstring.prg>
#include <dialbox/static_db_gettext.prg>
#include <dialbox/static_db_goitem.prg>
#include <dialbox/static_db_goto.prg>
#include <dialbox/static_db_handlekey.prg>
#include <dialbox/static_db_home.prg>
#include <dialbox/static_db_init.prg>
#include <dialbox/static_db_insert.prg>
#include <dialbox/static_db_killfocus.prg>
#include <dialbox/static_db_left.prg>
#include <dialbox/static_db_makestr.prg>
#include <dialbox/static_db_menu_create.prg>
#include <dialbox/static_db_pagedown.prg>
#include <dialbox/static_db_pageup.prg>
#include <dialbox/static_db_radio.prg>
#include <dialbox/static_db_refresh.prg>
#include <dialbox/static_db_right.prg>
#include <dialbox/static_db_select.prg>
#include <dialbox/static_db_setvalue.prg>
#include <dialbox/static_db_text.prg>
#include <dialbox/static_db_top.prg>
#include <dialbox/static_db_up.prg>
******************************************************************
#include <dialbox/_recover_dialogbox.prg>
#include <dialbox/dialogbox.prg>
******************************************************************
