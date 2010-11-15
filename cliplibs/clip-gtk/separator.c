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

/****          Separators has no signals               ****/

/**********************************************************/

/* Register separators in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_separator()
{
   return GTK_TYPE_SEPARATOR;
}

CLIP_DLLEXPORT GtkType
_gtk_type_hseparator()
{
   return GTK_TYPE_HSEPARATOR;
}

CLIP_DLLEXPORT GtkType
_gtk_type_vseparator()
{
   return GTK_TYPE_VSEPARATOR;
}

long
_clip_type_separator()
{
   return GTK_WIDGET_SEPARATOR;
}

long
_clip_type_hseparator()
{
   return GTK_WIDGET_HSEPARATOR;
}

long
_clip_type_vseparator()
{
   return GTK_WIDGET_VSEPARATOR;
}

const char *
_clip_type_name_separator()
{
   return "GTK_WIDGET_SEPARATOR";
}

const char *
_clip_type_name_hseparator()
{
   return "GTK_WIDGET_HSEPARATOR";
}

const char *
_clip_type_name_vseparator()
{
   return "GTK_WIDGET_VSEPARATOR";
}

int
clip_INIT___SEPARATOR(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_separator, _clip_type_name_separator, _gtk_type_separator, _gtk_type_widget, NULL);
   _wtype_table_put(_clip_type_hseparator, _clip_type_name_hseparator, _gtk_type_hseparator, _gtk_type_separator, NULL);
   _wtype_table_put(_clip_type_vseparator, _clip_type_name_vseparator, _gtk_type_vseparator, _gtk_type_separator, NULL);
   return 0;
}

/**** HSEPARATOR constructor ****/
int
clip_GTK_HSEPARATORNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   wid = gtk_hseparator_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/**** VSEPARATOR constructor ****/
int
clip_GTK_VSEPARATORNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   wid = gtk_vseparator_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/
