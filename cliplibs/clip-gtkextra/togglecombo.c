/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkextracfg.h"

#include <gtkextra/gtkextra.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

#include "ci_clip-gtkextra.ch"
#include "ci_clip-gtkextra.h"

/**********************************************************/
static gint
handle_changed_signal(GtkWidget * widget, gint row, gint col, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COL, col + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

/* Signals table */
static SignalTable toggle_combo_signals[] = {
   {"changed", GSF(handle_changed_signal), ESF(object_emit_signal), GTK_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register ColorCombo in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_toggle_combo()
{
   return gtk_toggle_combo_get_type();
}

long
_clip_type_toggle_combo()
{
   return GTK_WIDGET_TOGGLE_COMBO;
}

const char *
_clip_type_name_toggle_combo()
{
   return "GTK_WIDGET_TOGGLE_COMBO";
}

int
clip_INIT___TOGGLE_COMBO(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_toggle_combo, _clip_type_name_toggle_combo, _gtk_type_toggle_combo, _gtk_type_combo_box, toggle_combo_signals);
   return 0;
}

/**** TOGGLE COMBO constructors ****/
int
clip_GTK_TOGGLECOMBONEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gint nrows = INT_OPTION(ClipMachineMemory, 2, 1);
   gint ncols = INT_OPTION(ClipMachineMemory, 3, 1);

   GtkWidget *wid = NULL;
   C_widget *cwid, *cbutton, *ctable;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   wid = gtk_toggle_combo_new(nrows, ncols);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   cbutton = _register_widget(ClipMachineMemory, GTK_COMBO_BOX(wid)->button, NULL);
   ctable = _register_widget(ClipMachineMemory, GTK_TOGGLE_COMBO(wid)->table, NULL);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_BUTTON, &cbutton->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_TABLE, &ctable->obj);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_TOGGLECOMBOGETNROWS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctgc = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctgc, GTK_IS_TOGGLE_COMBO);
   gtk_toggle_combo_get_nrows(GTK_TOGGLE_COMBO(ctgc->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLECOMBOGETNCOLS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctgc = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctgc, GTK_IS_TOGGLE_COMBO);
   gtk_toggle_combo_get_ncols(GTK_TOGGLE_COMBO(ctgc->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLECOMBOSELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctgc = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint col = INT_OPTION(ClipMachineMemory, 3, 1);
   CHECKCWID(ctgc, GTK_IS_TOGGLE_COMBO);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_toggle_combo_select(GTK_TOGGLE_COMBO(ctgc->widget), row - 1, col - 1);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLECOMBOGETSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *ctgc = _fetch_cw_arg(ClipMachineMemory);
   gint row, col;
   CHECKCWID(ctgc, GTK_IS_TOGGLE_COMBO);
   gtk_toggle_combo_get_selection(GTK_TOGGLE_COMBO(ctgc->widget), &row, &col);
   _clip_storni(ClipMachineMemory, row + 1, 2, 0);
   _clip_storni(ClipMachineMemory, col + 1, 3, 0);
   return 0;
 err:
   return 1;
}
