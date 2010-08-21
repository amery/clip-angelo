
/*
    Copyright (C) 2004  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

/* Signals table */
static    gint
handle_toggled_signal(GtkToggleAction * action, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable toggle_action_signals[] = {
   {"toggled", GSF(handle_toggled_signal), ESF(object_emit_signal),
    GTK_TOGGLED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_toggle_action()
{
   return GTK_TYPE_TOGGLE_ACTION;
}

long
_clip_type_toggle_action()
{
   return GTK_OBJECT_TOGGLE_ACTION;
}

const char *
_clip_type_name_toggle_action()
{
   return "GTK_OBJECT_TOGGLE_ACTION";
}

int
clip_INIT___TOGGLEACTION(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_toggle_action, _clip_type_name_toggle_action,
		    _gtk_type_toggle_action, _gtk_type_action, toggle_action_signals);
   return 0;
}

int
clip_GTK_TOGGLEACTIONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *name = _clip_parc(ClipMachineMemory, 2);

   gchar    *label = _clip_parc(ClipMachineMemory, 3);

   gchar    *tooltip = _clip_parc(ClipMachineMemory, 4);

   gchar    *stock_id = _clip_parc(ClipMachineMemory, 5);

   C_object *caction;

   GtkToggleAction *action;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   LOCALE_TO_UTF(label);
   if (tooltip)
      LOCALE_TO_UTF(tooltip);
   if (stock_id)
      LOCALE_TO_UTF(stock_id);

   action = gtk_toggle_action_new(name, label, tooltip, stock_id);

   if (action)
    {
       caction = _list_get_cobject(ClipMachineMemory, action);
       if (!caction)
	  caction = _register_object(ClipMachineMemory, action, GTK_TYPE_TOGGLE_ACTION, cv, NULL);
       if (caction)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &caction->obj);
    }

   FREE_TEXT(name);
   FREE_TEXT(label);
   if (tooltip)
      FREE_TEXT(tooltip);
   if (stock_id)
      FREE_TEXT(stock_id);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLEACTIONTOGGLED(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_TOGGLE_ACTION(caction->object));

   gtk_toggle_action_toggled(GTK_TOGGLE_ACTION(caction->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLEACTIONSETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   gboolean  is_active = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_TOGGLE_ACTION(caction->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_toggle_action_set_active(GTK_TOGGLE_ACTION(caction->object), is_active);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLEACTIONGETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_TOGGLE_ACTION(caction->object));

   _clip_retl(ClipMachineMemory, gtk_toggle_action_get_active(GTK_TOGGLE_ACTION(caction->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLEACTIONSETDRAWASRADIO(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   gboolean  draw_as_radio = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_TOGGLE_ACTION(caction->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_toggle_action_set_draw_as_radio(GTK_TOGGLE_ACTION(caction->object), draw_as_radio);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLEACTIONGETDRAWASRADIO(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_TOGGLE_ACTION(caction->object));

   _clip_retl(ClipMachineMemory, gtk_toggle_action_get_draw_as_radio(GTK_TOGGLE_ACTION(caction->object)));

   return 0;
 err:
   return 1;
}
