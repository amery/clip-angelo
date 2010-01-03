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
static SignalTable toggle_button_signals[] = {
   {"toggled", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_TOGGLED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register toggle button in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_toggle_button()
{
   return GTK_TYPE_TOGGLE_BUTTON;
}

long
_clip_type_toggle_button()
{
   return GTK_WIDGET_TOGGLE_BUTTON;
}

const char *
_clip_type_name_toggle_button()
{
   return "GTK_WIDGET_TOGGLE_BUTTON";
}

int
clip_INIT___TOGGLE_BUTTON(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_toggle_button, _clip_type_name_toggle_button,
		    _gtk_type_toggle_button, _gtk_type_button, toggle_button_signals);
   return 0;
}

/**** TOGGLE BUTTON constructor ****/
int
clip_GTK_TOGGLEBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *title = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
    {
       LOCALE_TO_UTF(title);
       wid = gtk_toggle_button_new_with_label(title);
       FREE_TEXT(title);
    }
   else
      wid = gtk_toggle_button_new();
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Determines whether or not the toggle button is drawn on screen.
 * The default mode is FALSE, which results in the button being
 * displayed. To make the button invisible, set draw_indicator to TRUE. */
int
clip_GTK_TOGGLEBUTTONSETDRAWMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  mode = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_TOGGLE_BUTTON);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      mode = TRUE;
   gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(cbtn->widget), mode);
   return 0;
 err:
   return 1;
}

/* Changes the state of the toggle button. */
int
clip_GTK_TOGGLEBUTTONTOGGLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  state;

   CHECKCWID(cbtn, GTK_IS_TOGGLE_BUTTON);
   state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cbtn->widget));
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cbtn->widget), !state);
//      gtk_toggle_button_toggled(GTK_TOGGLE_BUTTON(cbtn->widget));
   return 0;
 err:
   return 1;
}

/* Queries a GtkToggleButton and returns it's current state.
 * Returns TRUE if the toggle button is pressed in and FALSE
 * if it is raised. */
int
clip_GTK_TOGGLEBUTTONGETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_TOGGLE_BUTTON);
   _clip_retl(ClipMachineMemory, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cbtn->widget)));
   return 0;
 err:
   return 1;
}

/* Sets the status of the toggle button. Set to TRUE if you want
 * the GtkToggleButton to be 'pressed in', and FALSE to raise it.
 * This action causes the toggled signal to be emitted. */
int
clip_GTK_TOGGLEBUTTONSETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  active = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_TOGGLE_BUTTON);
   CHECKOPT2(2, LOGICAL_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      active = TRUE;
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cbtn->widget), active);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLEBUTTONSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_spar(ClipMachineMemory, 2);

   GtkStyle *style;

   GtkButton *button;

   CHECKCWID(cbtn, GTK_IS_TOGGLE_BUTTON);
   CHECKARG(2, MAP_type_of_ClipVarType);
   button = &(GTK_TOGGLE_BUTTON(cbtn->widget)->button);
   style = gtk_style_copy(GTK_BIN(&(button->bin))->child->style);
  //gtk_style_unref(GTK_BIN(&(button->bin))->child->style);
   _map_to_style(ClipMachineMemory, mstyle, style);
   gtk_widget_set_style(GTK_BIN(&(button->bin))->child, style);
   return 0;
 err:
   return 1;
}
