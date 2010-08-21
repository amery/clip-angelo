/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/****            Scrollbars has no signals             ****/
/**********************************************************/

/* Register menu item signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_scrollbar()
{
   return GTK_TYPE_SCROLLBAR;
}

CLIP_DLLEXPORT GtkType
_gtk_type_hscrollbar()
{
   return GTK_TYPE_HSCROLLBAR;
}

CLIP_DLLEXPORT GtkType
_gtk_type_vscrollbar()
{
   return GTK_TYPE_VSCROLLBAR;
}

long
_clip_type_scrollbar()
{
   return GTK_WIDGET_SCROLLBAR;
}

long
_clip_type_hscrollbar()
{
   return GTK_WIDGET_HSCROLLBAR;
}

long
_clip_type_vscrollbar()
{
   return GTK_WIDGET_VSCROLLBAR;
}

const char *
_clip_type_name_scrollbar()
{
   return "GTK_WIDGET_SCROLLBAR";
}

const char *
_clip_type_name_hscrollbar()
{
   return "GTK_WIDGET_HSCROLLBAR";
}

const char *
_clip_type_name_vscrollbar()
{
   return "GTK_WIDGET_VSCROLLBAR";
}

int
clip_INIT___SCROLLBAR(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_scrollbar, _clip_type_name_scrollbar, _gtk_type_scrollbar, _gtk_type_range, NULL);
   _wtype_table_put(_clip_type_hscrollbar, _clip_type_name_hscrollbar, _gtk_type_hscrollbar, _gtk_type_scrollbar, NULL);
   _wtype_table_put(_clip_type_vscrollbar, _clip_type_name_vscrollbar, _gtk_type_vscrollbar, _gtk_type_scrollbar, NULL);
   return 0;
}

/**********************************************************/

/****  Horizontal scrollbar constructor ****/
int
clip_GTK_HSCROLLBARNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkAdjustment *adj;

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);

   adj = cadj ? GTK_ADJUSTMENT(cadj->widget) : NULL;
   wid = gtk_hscrollbar_new(adj);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

/****  Vertical scrollbar constructor ****/
int
clip_GTK_VSCROLLBARNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkAdjustment *adj;

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);

   adj = cadj ? GTK_ADJUSTMENT(cadj->widget) : NULL;
   wid = gtk_vscrollbar_new(adj);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/
