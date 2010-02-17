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

/****           Rulers has no own signals              ****/

/**********************************************************/

/* Register Ruler in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_ruler()
{
   return GTK_TYPE_RULER;
}

CLIP_DLLEXPORT GtkType
_gtk_type_hruler()
{
   return gtk_hruler_get_type();
}

CLIP_DLLEXPORT GtkType
_gtk_type_vruler()
{
   return gtk_vruler_get_type();
}

long
_clip_type_ruler()
{
   return GTK_WIDGET_RULER;
}

long
_clip_type_hruler()
{
   return GTK_WIDGET_HRULER;
}

long
_clip_type_vruler()
{
   return GTK_WIDGET_VRULER;
}

const char *
_clip_type_name_ruler()
{
   return "GTK_WIDGET_RULER";
}

const char *
_clip_type_name_hruler()
{
   return "GTK_WIDGET_HRULER";
}

const char *
_clip_type_name_vruler()
{
   return "GTK_WIDGET_VRULER";
}

int
clip_INIT___RULER(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_ruler, _clip_type_name_ruler, _gtk_type_ruler, _gtk_type_widget, NULL);
   _wtype_table_put(_clip_type_hruler, _clip_type_name_hruler, _gtk_type_hruler, _gtk_type_ruler, NULL);
   _wtype_table_put(_clip_type_vruler, _clip_type_name_vruler, _gtk_type_vruler, _gtk_type_ruler, NULL);
   return 0;
}

int
clip_GTK_HRULERNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_hruler_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_VRULERNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_vruler_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* This calls the GTKMetricType to set the ruler to units defined.
 * Available units are GTK_PIXELS, GTK_INCHES, or GTK_CENTIMETERS.
 * The default unit of measurement is GTK_PIXELS. */
int
clip_GTK_RULERSETMETRIC(ClipMachine * ClipMachineMemory)
{
   C_widget *crul = _fetch_cw_arg(ClipMachineMemory);
   GtkMetricType metric = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(crul, GTK_IS_RULER);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_ruler_set_metric(GTK_RULER(crul->widget), metric);
   return 0;
 err:
   return 1;
}

/* This sets the range of the ruler using gfloat lower,
 * gfloat upper, gfloat position, and gfloat max_size. */
int
clip_GTK_RULERSETRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *crul = _fetch_cw_arg(ClipMachineMemory);
   gfloat lower = _clip_parnd(ClipMachineMemory, 2);
   gfloat upper = _clip_parnd(ClipMachineMemory, 3);
   gfloat position = _clip_parnd(ClipMachineMemory, 4);
   gfloat max_size = _clip_parnd(ClipMachineMemory, 5);
   CHECKCWID(crul, GTK_IS_RULER);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   gtk_ruler_set_range(GTK_RULER(crul->widget), lower, upper, position, max_size);
   return 0;
 err:
   return 1;
}
