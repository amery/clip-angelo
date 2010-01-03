/*
    Copyright (C) 2001 - 2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    	      Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/* Signal handlers */
static    gint
handle_trough_click_signal(GtkRange * wid, gint x, gint y, float *jump_perc, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, y);
   INVOKESIGHANDLER(GTK_WIDGET(wid), cs, cv);
}

static    gint
handle_motion_signal(GtkRange * wid, gint xdelta, gint ydelta, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_XDELTA, xdelta);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_YDELTA, ydelta);
   INVOKESIGHANDLER(GTK_WIDGET(wid), cs, cv);
}

static    gint
handle_adjust_bounds_signal(GtkRange * wid, gdouble arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ARG1, arg1);
   INVOKESIGHANDLER(GTK_WIDGET(wid), cs, cv);
}

static    gint
handle_move_slider_signal(GtkRange * wid, GtkScrollType type, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_SCROLLTYPE, type);
   INVOKESIGHANDLER(GTK_WIDGET(wid), cs, cv);
}

/* Signals table */
static SignalTable range_signals[] = {
   {"draw-background", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DRAW_BACKGROUND_SIGNAL},
   {"clear-background", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CLEAR_BACKGROUND_SIGNAL},
   {"draw-trough", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DRAW_TROUGH_SIGNAL},
   {"draw-slider", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DRAW_SLIDER_SIGNAL},
   {"step-forward", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_STEP_FORW_SIGNAL},
   {"step-backward", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_STEP_BACK_SIGNAL},
   {"slider-update", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SLIDER_UPDATE_SIGNAL},
   {"trough-click", GSF(handle_trough_click_signal), ESF(object_emit_signal),
    GTK_TROUGH_CLICK_SIGNAL},
   {"motion", GSF(handle_motion_signal), ESF(object_emit_signal),
    GTK_MOTION_SIGNAL},
   {"adjust-bounds", GSF(handle_adjust_bounds_signal),
    ESF(object_emit_signal), GTK_ADJUST_BOUNDS_SIGNAL},
   {"move-slider", GSF(handle_move_slider_signal), ESF(object_emit_signal),
    GTK_MOVE_SLIDER_SIGNAL},
   {"value-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_VALUE_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register range in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_range()
{
   return GTK_TYPE_RANGE;
}

long
_clip_type_range()
{
   return GTK_WIDGET_RANGE;
}

const char *
_clip_type_name_range()
{
   return "GTK_WIDGET_RANGE";
}

int
clip_INIT___RANGE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_range, _clip_type_name_range, _gtk_type_range, _gtk_type_widget, range_signals);
   return 0;
}

int
clip_GTK_RANGEGETADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   GtkAdjustment *adj;

   C_widget *cadj;

   ClipVar  *ret = RETPTR(ClipMachineMemory);

   CHECKCWID(crange, GTK_IS_RANGE);
   adj = gtk_range_get_adjustment(GTK_RANGE(crange->widget));
   cadj = _list_get_cwidget(ClipMachineMemory, adj);
   if (!cadj)
      cadj = _register_widget(ClipMachineMemory, (GtkWidget *) adj, NULL);
   if (cadj)
      _clip_mclone(ClipMachineMemory, ret, &cadj->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGESETUPDATEPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   GtkUpdateType policy = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(crange, GTK_IS_RANGE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_range_set_update_policy(GTK_RANGE(crange->widget), policy);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGESETADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(crange, GTK_IS_RANGE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cadj, GTK_IS_ADJUSTMENT);
   gtk_range_set_adjustment(GTK_RANGE(crange->widget), GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGEGETINVERTED(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(crange, GTK_IS_RANGE);
   _clip_retl(ClipMachineMemory, gtk_range_get_inverted(GTK_RANGE(crange->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGESETINVERTED(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   gboolean  setting = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(crange, GTK_IS_RANGE);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   gtk_range_set_inverted(GTK_RANGE(crange->widget), setting);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGEGETUPDATEPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(crange, GTK_IS_RANGE);
   _clip_retni(ClipMachineMemory, (int) gtk_range_get_update_policy(GTK_RANGE(crange->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGEGETVALUE(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(crange, GTK_IS_RANGE);
   _clip_retnd(ClipMachineMemory, gtk_range_get_value(GTK_RANGE(crange->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGESETINCREMENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   gdouble   step = _clip_parnd(ClipMachineMemory, 2);

   gdouble   page = _clip_parnd(ClipMachineMemory, 3);

   CHECKCWID(crange, GTK_IS_RANGE);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   gtk_range_set_increments(GTK_RANGE(crange->widget), step, page);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGESETRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   gdouble   min = _clip_parnd(ClipMachineMemory, 2);

   gdouble   max = _clip_parnd(ClipMachineMemory, 3);

   CHECKCWID(crange, GTK_IS_RANGE);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   gtk_range_set_range(GTK_RANGE(crange->widget), min, max);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RANGESETVALUE(ClipMachine * ClipMachineMemory)
{
   C_widget *crange = _fetch_cw_arg(ClipMachineMemory);

   gdouble   value = _clip_parnd(ClipMachineMemory, 2);

   CHECKCWID(crange, GTK_IS_RANGE);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   gtk_range_set_value(GTK_RANGE(crange->widget), value);
   return 0;
 err:
   return 1;
}
