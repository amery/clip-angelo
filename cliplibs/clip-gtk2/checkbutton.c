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
/* Signals table */
static SignalTable check_button_signals[] = {
   {"draw-indicator", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DRAW_INDICATOR_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register check button in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_check_button()
{
   return GTK_TYPE_CHECK_BUTTON;
}

long
_clip_type_check_button()
{
   return GTK_WIDGET_CHECK_BUTTON;
}

const char *
_clip_type_name_check_button()
{
   return "GTK_WIDGET_CHECK_BUTTON";
}

int
clip_INIT___CHECK_BUTTON(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_check_button, _clip_type_name_check_button, _gtk_type_check_button, _gtk_type_toggle_button, check_button_signals);
   return 0;
}

/**** CHECK BUTTON constructor ****/
int
clip_GTK_CHECKBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   char *title = _clip_parc(ClipMachineMemory, 2);
   char *pchar = _clip_parc(ClipMachineMemory, 3);
   GtkWidget *wid = NULL, *label = NULL;
   C_widget *cwid;
   guint accel_key = 0;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (pchar)
      {
	 char *pc;
	 for (pc = title; pc && *pc; pc++)
	    if (*pc == *pchar)
	       *pc = '_';
      }
//        else
//              *pchar = '_';
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      {
	 LOCALE_TO_UTF(title);
	 wid = gtk_check_button_new_with_label(title);
	 label = GTK_BIN(&(GTK_BUTTON(wid)->bin))->child;
	 if (pchar)
	    accel_key = gtk_label_parse_uline(GTK_LABEL(label), title);
	 FREE_TEXT(title);
      }
   else
      wid = gtk_check_button_new();
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   _clip_mputn(ClipMachineMemory, &cwid->obj, HASH_ACCELKEY, accel_key);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_CHECKBUTTONNEWWITHMNEMONIC(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gchar *title = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(title);
   wid = gtk_check_button_new_with_mnemonic(title);
   FREE_TEXT(title);

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/
