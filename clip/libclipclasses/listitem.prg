/*
    Copyright (C) 2001  ITK
    Author   : Alena (alena@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include <ci_inkey.ch>
**********************************************************************
/*
color "1,     2, 3, 4, 5, 6"
1- border, item
2- current item in focus
3- current item out of focus
4- title
5- selected item
6- current selected item
7- found symbols
*/
#define LI_EXIT			-1
#define LI_CONTINUE		0
#define LI_EXCEPTION		1
**********************************************************************
#include <listitem/static___init.prg>
#include <listitem/static___refreshUnion.prg>
#include <listitem/static___setcolor.prg>
#include <listitem/static_addItem.prg>
#include <listitem/static_clear.prg>
#include <listitem/static_clearFindBuffer.prg>
#include <listitem/static_delItem.prg>
#include <listitem/static_down.prg>
#include <listitem/static_drawTitle.prg>
#include <listitem/static_End.prg>
#include <listitem/static_find.prg>
#include <listitem/static_getItem.prg>
#include <listitem/static_getSelected.prg>
#include <listitem/static_handleKey.prg>
#include <listitem/static_Home.prg>
#include <listitem/static_insItem.prg>
#include <listitem/static_killFocus.prg>
#include <listitem/static_killUnion.prg>
#include <listitem/static_left.prg>
#include <listitem/static_li_select.prg>
#include <listitem/static_PageDown.prg>
#include <listitem/static_PageUp.prg>
#include <listitem/static_refresh.prg>
#include <listitem/static_right.prg>
#include <listitem/static_setColumns.prg>
#include <listitem/static_setFocus.prg>
#include <listitem/static_setItem.prg>
#include <listitem/static_setKey.prg>
#include <listitem/static_setNewBound.prg>
#include <listitem/static_setTitle.prg>
#include <listitem/static_setUnion.prg>
#include <listitem/static_setWidthCol.prg>
#include <listitem/static_up.prg>
**********************************************************************
#include <listitem/_recover_listitem.prg>
#include <listitem/listItemNew.prg>
**********************************************************************
