/*
    Copyright (C) 2001  ITK
    Author   : Alena (alena@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include "ci_inkey.ch"
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
#include "listitem/static___init.ch"
#include "listitem/static___refreshUnion.ch"
#include "listitem/static___setcolor.ch"
#include "listitem/static_addItem.ch"
#include "listitem/static_clear.ch"
#include "listitem/static_clearFindBuffer.ch"
#include "listitem/static_delItem.ch"
#include "listitem/static_down.ch"
#include "listitem/static_drawTitle.ch"
#include "listitem/static_End.ch"
#include "listitem/static_find.ch"
#include "listitem/static_getItem.ch"
#include "listitem/static_getSelected.ch"
#include "listitem/static_handleKey.ch"
#include "listitem/static_Home.ch"
#include "listitem/static_insItem.ch"
#include "listitem/static_killFocus.ch"
#include "listitem/static_killUnion.ch"
#include "listitem/static_left.ch"
#include "listitem/static_li_select.ch"
#include "listitem/static_PageDown.ch"
#include "listitem/static_PageUp.ch"
#include "listitem/static_refresh.ch"
#include "listitem/static_right.ch"
#include "listitem/static_setColumns.ch"
#include "listitem/static_setFocus.ch"
#include "listitem/static_setItem.ch"
#include "listitem/static_setKey.ch"
#include "listitem/static_setNewBound.ch"
#include "listitem/static_setTitle.ch"
#include "listitem/static_setUnion.ch"
#include "listitem/static_setWidthCol.ch"
#include "listitem/static_up.ch"
**********************************************************************
#include "listitem/_recover_listitem.ch"
#include "listitem/listItemNew.ch"
**********************************************************************
