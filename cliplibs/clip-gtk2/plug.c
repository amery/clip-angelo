/*
    Copyright (C) 2004  ITK
    Author  : Kornilova E.V. <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/* Signals table */
static SignalTable plug_signals[] = {
   {"embedded", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_EMBEDDED_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register widget signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_plug()
{
   return GTK_TYPE_PLUG;
}

long
_clip_type_plug()
{
   return GTK_WIDGET_PLUG;
}

const char *
_clip_type_name_plug()
{
   return "GTK_WIDGET_PLUG";
}

int
clip_INIT___PLUG(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_plug, _clip_type_name_plug, _gtk_type_plug, _gtk_type_window, plug_signals);
   return 0;
}

/**********************************************************/

/**** PLUG constructor ****/
int
clip_GTK_PLUGCONSTRUCT(ClipMachine * ClipMachineMemory)
{
   C_widget *cplug = _fetch_cw_arg(ClipMachineMemory);
   GdkNativeWindow id = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cplug, GTK_IS_PLUG);

   gtk_plug_construct(GTK_PLUG(cplug->widget), id);

   return 0;
 err:
   return 1;
}

int
clip_GTK_PLUGNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GdkNativeWindow id = _clip_parni(ClipMachineMemory, 2);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   wid = gtk_plug_new(id);

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_PLUGGETID(ClipMachine * ClipMachineMemory)
{
   C_widget *cplug = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cplug, GTK_IS_PLUG);

   _clip_retni(ClipMachineMemory, (int) gtk_plug_get_id(GTK_PLUG(cplug->widget)));

   return 0;
 err:
   return 1;
}
