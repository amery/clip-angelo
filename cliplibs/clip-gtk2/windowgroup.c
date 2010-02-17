/*
    Copyright (C) 2004  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

/* WindowGroup has no signals */

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_window_group()
{
   return GTK_TYPE_WINDOW_GROUP;
}

long
_clip_type_window_group()
{
   return GTK_OBJECT_WINDOW_GROUP;
}

const char *
_clip_type_name_window_group()
{
   return "GTK_OBJECT_WINDOW_GROUP";
}

int
clip_INIT___WINDOWGROUP(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_window_group, _clip_type_name_window_group, _gtk_type_window_group, NULL, NULL);
   return 0;
}

/******************************************************************************
* gtk_WindowGroupNew( wGroupObj ) --> wGroupNewObject
******************************************************************************/
int
clip_GTK_WINDOWGROUPNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_object *cwgrp;
   GtkWindowGroup *wgrp;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wgrp = gtk_window_group_new();

   if (wgrp)
      {
	 cwgrp = _list_get_cobject(ClipMachineMemory, wgrp);
	 if (!cwgrp)
	    cwgrp = _register_object(ClipMachineMemory, wgrp, GTK_TYPE_WINDOW_GROUP, cv, NULL);
	 if (cwgrp)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwgrp->obj);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_WindowGroupAddWindow( wGroup, window ) --> NIL
******************************************************************************/
int
clip_GTK_WINDOWGROUPADDWINDOW(ClipMachine * ClipMachineMemory)
{
   C_object *cwgrp = _fetch_co_arg(ClipMachineMemory);
   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cwgrp, GTK_IS_WINDOW_GROUP(cwgrp->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_group_add_window(GTK_WINDOW_GROUP(cwgrp->object), GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_WindowGroupRemoveWindow( wGroup, window ) --> NIL
******************************************************************************/
int
clip_GTK_WINDOWGROUPREMOVEWINDOW(ClipMachine * ClipMachineMemory)
{
   C_object *cwgrp = _fetch_co_arg(ClipMachineMemory);
   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cwgrp, GTK_IS_WINDOW_GROUP(cwgrp->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_group_remove_window(GTK_WINDOW_GROUP(cwgrp->object), GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}
