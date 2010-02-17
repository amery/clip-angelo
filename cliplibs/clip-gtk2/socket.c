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
static SignalTable socket_signals[] = {
   {"plug-added", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_PLUG_ADDED_SIGNAL},
   {"plug-removed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_PLUG_REMOVED_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register widget signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_socket()
{
   return GTK_TYPE_SOCKET;
}

long
_clip_type_socket()
{
   return GTK_WIDGET_SOCKET;
}

const char *
_clip_type_name_socket()
{
   return "GTK_WIDGET_SOCKET";
}

int
clip_INIT___SOCKET(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_socket, _clip_type_name_socket, _gtk_type_socket, _gtk_type_container, socket_signals);
   return 0;
}

/**********************************************************/

/**** SOCKET constructor ****/
int
clip_GTK_SOCKETNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_socket_new();

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SOCKETSTEAL(ClipMachine * ClipMachineMemory)
{
   C_widget *csocket = _fetch_cw_arg(ClipMachineMemory);
   GdkNativeWindow id = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(csocket, GTK_IS_SOCKET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_socket_steal(GTK_SOCKET(csocket->widget), id);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SOCKETADDID(ClipMachine * ClipMachineMemory)
{
   C_widget *csocket = _fetch_cw_arg(ClipMachineMemory);
   GdkNativeWindow id = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(csocket, GTK_IS_SOCKET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_socket_add_id(GTK_SOCKET(csocket->widget), id);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SOCKETGETID(ClipMachine * ClipMachineMemory)
{
   C_widget *csocket = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(csocket, GTK_IS_SOCKET);

   _clip_retni(ClipMachineMemory, gtk_socket_get_id(GTK_SOCKET(csocket->widget)));

   return 0;
 err:
   return 1;
}
