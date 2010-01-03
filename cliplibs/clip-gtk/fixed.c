/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/*********************** SIGNALS **************************/

/* Fixed has no signals */

/**********************************************************/

/* Register Fixed in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_fixed()
{
   return GTK_TYPE_FIXED;
}

long
_clip_type_fixed()
{
   return GTK_WIDGET_FIXED;
}

const char *
_clip_type_name_fixed()
{
   return "GTK_WIDGET_FIXED";
}

int
clip_INIT___FIXED(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_fixed, _clip_type_name_fixed, _gtk_type_fixed, _gtk_type_container, NULL);
   return 0;
}

int
clip_GTK_FIXEDNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_fixed_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* Adds a widget to a GtkFixed container at the given position. */
int
clip_GTK_FIXEDPUT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfix = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      x = _clip_parni(ClipMachineMemory, 3);

   gint      y = _clip_parni(ClipMachineMemory, 4);

   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cfix, GTK_IS_FIXED);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      x = cwid->widget->allocation.x;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      y = cwid->widget->allocation.y;
   gtk_fixed_put(GTK_FIXED(cfix->widget), cwid->widget, x, y);

   return 0;
 err:
   return 1;
}

/* Moves a child of a GtkFixed container to the given position. */
int
clip_GTK_FIXEDMOVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cfix = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      x = _clip_parni(ClipMachineMemory, 3);

   gint      y = _clip_parni(ClipMachineMemory, 4);

   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cfix, GTK_IS_FIXED);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      x = cwid->widget->allocation.x;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      y = cwid->widget->allocation.y;
   gtk_fixed_move(GTK_FIXED(cfix->widget), cwid->widget, x, y);

   return 0;
 err:
   return 1;
}
