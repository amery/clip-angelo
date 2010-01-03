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
static SignalTable item_signals[] = {
   {"select", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SELECT_SIGNAL},
   {"toggle", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_TOGGLE_SIGNAL},
   {"deselect", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DESELECT_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register item signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_item()
{
   return GTK_TYPE_ITEM;
}

long
_clip_type_item()
{
   return GTK_WIDGET_ITEM;
}

const char *
_clip_type_name_item()
{
   return "GTK_WIDGET_ITEM";
}

int
clip_INIT___ITEM(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_item, _clip_type_name_item, _gtk_type_item, _gtk_type_container, item_signals);
   return 0;
}

/**********************************************************/

int
clip_GTK_ITEMSELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(citm, GTK_IS_ITEM);
   gtk_item_select(GTK_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_ITEMDESELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(citm, GTK_IS_ITEM);
   gtk_item_deselect(GTK_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_ITEMTOGGLE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(citm, GTK_IS_ITEM);
   gtk_item_toggle(GTK_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}
