/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include <ci_button.ch>
#include <ci_box.ch>
#include <ci_setcurs.ch>
#include <ci_set.ch>
**********************************************************************

#ifdef __UNIX__
   #define LB_PUSHSYM "#"
#else
   #define LB_PUSHSYM chr(31)
#endif
**********************************************************************
#include <listbox/static___setBuffer.prg>
#include <listbox/static___setcolor.prg>
#include <listbox/static_lb_addItem.prg>
#include <listbox/static_lb_close.prg>
#include <listbox/static_lb_delItem.prg>
#include <listbox/static_lb_display.prg>
#include <listbox/static_lb_findText.prg>
#include <listbox/static_lb_getData.prg>
#include <listbox/static_lb_getItem.prg>
#include <listbox/static_lb_getText.prg>
#include <listbox/static_lb_hitTest.prg>
#include <listbox/static_lb_insItem.prg>
#include <listbox/static_lb_killFocus.prg>
#include <listbox/static_lb_nextItem.prg>
#include <listbox/static_lb_nextPage.prg>
#include <listbox/static_lb_open.prg>
#include <listbox/static_lb_prevItem.prg>
#include <listbox/static_lb_prevPage.prg>
#include <listbox/static_lb_scroll.prg>
#include <listbox/static_lb_select.prg>
#include <listbox/static_lb_setData.prg>
#include <listbox/static_lb_setFocus.prg>
#include <listbox/static_lb_setItem.prg>
#include <listbox/static_lb_setText.prg>
**********************************************************************
#include <listbox/_listbox_.prg>
#include <listbox/_recover_listbox.prg>
#include <listbox/listBox.prg>
**********************************************************************
