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

/****            Scale has no own signals              ****/

/**********************************************************/

/* Register Scale in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_scale()
{
   return GTK_TYPE_SCALE;
}

CLIP_DLLEXPORT GtkType
_gtk_type_hscale()
{
   return gtk_hscale_get_type();
}

CLIP_DLLEXPORT GtkType
_gtk_type_vscale()
{
   return gtk_vscale_get_type();
}

long
_clip_type_scale()
{
   return GTK_WIDGET_SCALE;
}

long
_clip_type_hscale()
{
   return GTK_WIDGET_HSCALE;
}

long
_clip_type_vscale()
{
   return GTK_WIDGET_VSCALE;
}

const char *
_clip_type_name_scale()
{
   return "GTK_WIDGET_SCALE";
}

const char *
_clip_type_name_hscale()
{
   return "GTK_WIDGET_HSCALE";
}

const char *
_clip_type_name_vscale()
{
   return "GTK_WIDGET_VSCALE";
}

int
clip_INIT___SCALE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_scale, _clip_type_name_scale, _gtk_type_scale, _gtk_type_range, NULL);
   _wtype_table_put(_clip_type_hscale, _clip_type_name_hscale, _gtk_type_hscale, _gtk_type_scale, NULL);
   _wtype_table_put(_clip_type_vscale, _clip_type_name_vscale, _gtk_type_vscale, _gtk_type_scale, NULL);
   return 0;
}

int
clip_GTK_HSCALENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GtkAdjustment *adj;
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);

   adj = cadj ? GTK_ADJUSTMENT(cadj->widget) : NULL;
   wid = gtk_hscale_new(adj);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_VSCALENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GtkAdjustment *adj;
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);

   adj = cadj ? GTK_ADJUSTMENT(cadj->widget) : NULL;
   wid = gtk_vscale_new(adj);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCALESETADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cscale, GTK_IS_SCALE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);
   gtk_widget_set(cscale->widget, "adjustment", GTK_ADJUSTMENT(cadj->widget), NULL);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCALEGETADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cadj;
   GtkAdjustment *adj;

   CHECKCWID(cscale, GTK_IS_SCALE);

   adj = GTK_RANGE(&GTK_SCALE(cscale->widget)->range)->adjustment;
   cadj = _list_get_cwidget(ClipMachineMemory, adj);
   if (!cadj)
      cadj = _register_widget(ClipMachineMemory, (GtkWidget *) adj, NULL);
   if (cadj)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cadj->obj);
   return 0;
 err:
   return 1;
}

/* Sets the number of decimal places that are displayed in the value. */
int
clip_GTK_SCALESETDIGITS(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   gint digits = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cscale, GTK_IS_SCALE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_scale_set_digits(GTK_SCALE(cscale->widget), digits);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCALEGETDIGITS(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   GtkArg arg;
   CHECKCWID(cscale, GTK_IS_SCALE);
   arg.type = GTK_TYPE_INT;
   arg.name = "digits";
   gtk_widget_get(cscale->widget, &arg);
   _clip_retni(ClipMachineMemory, arg.d.int_data);
   return 0;
 err:
   return 1;
}

/* Specifies whether the current value is displayed as a string next to the slider. */
int
clip_GTK_SCALESETDRAWVALUE(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   gboolean draw_value = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cscale, GTK_IS_SCALE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      draw_value = TRUE;
   gtk_scale_set_draw_value(GTK_SCALE(cscale->widget), draw_value);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCALEGETDRAWVALUES(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   GtkArg arg;
   CHECKCWID(cscale, GTK_IS_SCALE);
   arg.type = GTK_TYPE_BOOL;
   arg.name = "draw-value";
   gtk_widget_get(cscale->widget, &arg);
   _clip_retl(ClipMachineMemory, arg.d.bool_data);
   return 0;
 err:
   return 1;
}

/* Sets the position in which the current value is displayed. */
int
clip_GTK_SCALESETVALUEPOS(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   GtkPositionType pos = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cscale, GTK_IS_SCALE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_scale_set_value_pos(GTK_SCALE(cscale->widget), pos);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCALEGETVALUEPOS(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   GtkArg arg;
   CHECKCWID(cscale, GTK_IS_SCALE);
   arg.type = GTK_TYPE_INT;
   arg.name = "value-pos";
   gtk_widget_get(cscale->widget, &arg);
   _clip_retni(ClipMachineMemory, arg.d.int_data);
   return 0;
 err:
   return 1;
}

/* An internal function used to get the maximum
 * width needed to display the value string. */
int
clip_GTK_SCALEGETVALUEWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cscale = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cscale, GTK_IS_SCALE);
   _clip_retni(ClipMachineMemory, gtk_scale_get_value_width(GTK_SCALE(cscale->widget)));
   return 0;
 err:
   return 1;
}
