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

/**********************************************************/
/* Signals table */
static SignalTable adj_signals[] = {
   {"changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CHANGED_SIGNAL},
   {"value-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_VALUE_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

static SignalTable data_signals[] = {
   {"disconnect", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DISCONNECT_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register adjustment in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_adjustment()
{
   return GTK_TYPE_ADJUSTMENT;
}

long
_clip_type_adjustment()
{
   return GTK_WIDGET_ADJUSTMENT;
}

const char *
_clip_type_name_adjustment()
{
   return "GTK_WIDGET_ADJUSTMENT";
}

CLIP_DLLEXPORT GtkType
_gtk_type_data()
{
   return GTK_TYPE_DATA;
}

long
_clip_type_data()
{
   return GTK_WIDGET_DATA;
}

const char *
_clip_type_name_data()
{
   return "GTK_WIDGET_DATA";
}

int
clip_INIT___ADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_adjustment, _clip_type_name_adjustment, _gtk_type_adjustment, _gtk_type_data, adj_signals);
   return 0;
}

/* Register data in global table */
int
clip_INIT___DATA(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_data, _clip_type_name_data, _gtk_type_data, NULL, data_signals);
   return 0;
}

/**** Adjustment constructor ****/
int
clip_GTK_ADJUSTMENTNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gfloat    value = _clip_parnd(ClipMachineMemory, 2);

   gfloat    lower = _clip_parnd(ClipMachineMemory, 3);

   gfloat    upper = _clip_parnd(ClipMachineMemory, 4);

   gfloat    step_inc = _clip_parnd(ClipMachineMemory, 5);

   gfloat    page_inc = _clip_parnd(ClipMachineMemory, 6);

   gfloat    page_size = _clip_parnd(ClipMachineMemory, 7);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT(7, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      value = 0;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      lower = 0;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      upper = 100;
   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      step_inc = 1;
   if (_clip_parinfo(ClipMachineMemory, 6) == UNDEF_type_of_ClipVarType)
      page_inc = 10;
   if (_clip_parinfo(ClipMachineMemory, 7) == UNDEF_type_of_ClipVarType)
      page_size = 10;
   wid = (GtkWidget *) gtk_adjustment_new(value, lower, upper, step_inc, page_inc, page_size);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_ADJUSTMENTCHANGED(ClipMachine * ClipMachineMemory)
{
   C_widget *cadj = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cadj, GTK_IS_ADJUSTMENT);
   gtk_adjustment_changed(GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_ADJUSTMENTVALUECHANGED(ClipMachine * ClipMachineMemory)
{
   C_widget *cadj = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cadj, GTK_IS_ADJUSTMENT);
   gtk_adjustment_value_changed(GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_ADJUSTMENTCLAMPPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cadj = _fetch_cw_arg(ClipMachineMemory);

   gfloat    lower = _clip_parnd(ClipMachineMemory, 2);

   gfloat    upper = _clip_parnd(ClipMachineMemory, 3);

   CHECKCWID(cadj, GTK_IS_ADJUSTMENT);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   gtk_adjustment_clamp_page(GTK_ADJUSTMENT(cadj->widget), lower, upper);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ADJUSTMENTSETVALUE(ClipMachine * ClipMachineMemory)
{
   C_widget *cadj = _fetch_cw_arg(ClipMachineMemory);

   gfloat    value = _clip_parnd(ClipMachineMemory, 2);

   CHECKCWID(cadj, GTK_IS_ADJUSTMENT);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   gtk_adjustment_set_value(GTK_ADJUSTMENT(cadj->widget), value);
   return 0;
 err:
   return 1;
}

/* Alena: get current adjustment values:
lower, upper, value
*/
int
clip_GTK_ADJUSTMENTGETVALUE(ClipMachine * ClipMachineMemory)
{
   C_widget *cadj = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cadj, GTK_IS_ADJUSTMENT);

   _clip_storni(ClipMachineMemory, GTK_ADJUSTMENT(cadj->widget)->value, 2, 0);
   _clip_storni(ClipMachineMemory, GTK_ADJUSTMENT(cadj->widget)->lower, 3, 0);
   _clip_storni(ClipMachineMemory, GTK_ADJUSTMENT(cadj->widget)->upper, 4, 0);

   return 0;
 err:
   return 1;
}
