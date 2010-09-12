/*
    Copyright (C) 2001  ITK
    Author   : Alena (alena@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************
#include <ci_inkey.ch>
#include <ci_clipcfg.h>
#include <ci_box.ch>
#define __CHARSET	"cp437"
#define COLOR_BROWSER	"0/3,  14/3, 1/3, 14/3, 14/1, 11/3, 0/7, 15/1, 15/3,   1/3"
#define COLOR_MENU 	"0/7,0/2,4/7,2/5,15/7,0/7"
/*                      ____   ________  __________  ____   _________  _____   _____ */
/*                      backgr links     buttons     header getobj 0/1 <i></i> <code>  */
**********************************************************
#define COLOR_DIALOG	"0/7, 15/1"
#define COLOR_TEXTAREA	"11/1,11/1,0/2,0/3,0/7,1/3"
#define TAB_SIZE	8
#define DELIMITER	"�"
#define LISTDELTA	4
#define LIST_TYPE	1
#define LIST_DEEP	2
#define LIST_START	3
#define LIST_VID	4
#define MARGIN_LEFT	2

#define _BUTTON_	1
#define _GET_		2
#define _CHECK_		3
#define _RADIO_		4
#define _GETTEXT_	5
**********************************************************
#include <hbrowse/static_add_Table.prg>
#include <hbrowse/static_br_addObject.prg>
#include <hbrowse/static_br_getUrl.prg>
#include <hbrowse/static_br_menu_Create.prg>
#include <hbrowse/static_br_reload.prg>
#include <hbrowse/static_br_SetCodePage.prg>
#include <hbrowse/static_Browse_File.prg>
#include <hbrowse/static_getHcolor.prg>
#include <hbrowse/static_goBack.prg>
#include <hbrowse/static_goNext.prg>
#include <hbrowse/static_new_clr.prg>
#include <hbrowse/static_newLine.prg>
#include <hbrowse/static_newRefer.prg>
#include <hbrowse/static_openBrowser.prg>
#include <hbrowse/static_parse_A.prg>
#include <hbrowse/static_parse_Col_Table.prg>
#include <hbrowse/static_parse_Dd.prg>
#include <hbrowse/static_parse_Dt.prg>
#include <hbrowse/static_parse_Font.prg>
#include <hbrowse/static_parse_Form.prg>
#include <hbrowse/static_parse_H.prg>
#include <hbrowse/static_parse_Head.prg>
#include <hbrowse/static_parse_Hr.prg>
#include <hbrowse/static_parse_Image.prg>
#include <hbrowse/static_parse_Input.prg>
#include <hbrowse/static_parse_Li.prg>
#include <hbrowse/static_parse_Meta.prg>
#include <hbrowse/static_parse_Ol.prg>
#include <hbrowse/static_parse_Table.prg>
#include <hbrowse/static_parse_Tbl_Caption.prg>
#include <hbrowse/static_parse_TextArea.prg>
#include <hbrowse/static_parse_Title.prg>
#include <hbrowse/static_parse_Ul.prg>
#include <hbrowse/static_parseNewUrl.prg>
#include <hbrowse/static_parseTag.prg>
#include <hbrowse/static_rest_clr.prg>
#include <hbrowse/static_runing.prg>
**********************************************************
#include <hbrowse/hbrowse.prg>
#include <hbrowse/replace_symbols.prg>
#include <hbrowse/split_cell.prg>
#include <hbrowse/split_string.prg>
#include <hbrowse/splitSpaceFrom.prg>
**********************************************************
