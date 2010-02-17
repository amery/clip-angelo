
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
static gint
handle_changed_signal(GtkRadioAction * action, GtkRadioAction * current, C_signal * cs)
{
   C_object *ccur;
   OBJECTPREPARECV(cs, cv);
   ccur = _list_get_cobject(cs->co->cmachine, current);
   if (!ccur)
      ccur = _register_object(cs->co->cmachine, current, GTK_TYPE_RADIO_ACTION, NULL, NULL);
   if (ccur)
      _clip_madd(cs->co->cmachine, &cv, HASH_CURRENT, &ccur->obj);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable radio_action_signals[] = {
   {"changed", GSF(handle_changed_signal), ESF(object_emit_signal),
    GTK_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_radio_action()
{
   return GTK_TYPE_RADIO_ACTION;
}

long
_clip_type_radio_action()
{
   return GTK_OBJECT_RADIO_ACTION;
}

const char *
_clip_type_name_radio_action()
{
   return "GTK_OBJECT_RADIO_ACTION";
}

int
clip_INIT___RADIOACTION(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_radio_action, _clip_type_name_radio_action, _gtk_type_radio_action, _gtk_type_toggle_action, radio_action_signals);
   return 0;
}

int
clip_GTK_RADIOACTIONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   gchar *label = _clip_parc(ClipMachineMemory, 3);
   gchar *tooltip = _clip_parc(ClipMachineMemory, 4);
   gchar *stock_id = _clip_parc(ClipMachineMemory, 5);
   gint value = _clip_parni(ClipMachineMemory, 6);
   C_object *caction;
   GtkRadioAction *action;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   LOCALE_TO_UTF(label);
   if (tooltip)
      LOCALE_TO_UTF(tooltip);
   if (stock_id)
      LOCALE_TO_UTF(stock_id);

   action = gtk_radio_action_new(name, label, tooltip, stock_id, value);

   if (action)
      {
	 caction = _list_get_cobject(ClipMachineMemory, action);
	 if (!caction)
	    caction = _register_object(ClipMachineMemory, action, GTK_TYPE_RADIO_ACTION, cv, NULL);
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
clip_GTK_RADIOACTIONGETGROUP(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);
   GSList *group;
   C_object *cgroup;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_RADIO_ACTION(caction->object));

   group = gtk_radio_action_get_group(GTK_RADIO_ACTION(caction->object));

   cgroup = _list_get_cobject(ClipMachineMemory, group);
   if (!cgroup)
      cgroup = _register_object(ClipMachineMemory, group, GTK_TYPE_OBJECT, NULL, NULL);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cgroup->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_RADIOACTIONSETGROUP(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);
   C_object *cgroup = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_RADIO_ACTION(caction->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cgroup, GTK_IS_OBJECT(cgroup->object));

   gtk_radio_action_set_group(GTK_RADIO_ACTION(caction->object), (GSList *) cgroup->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_RADIOACTIONGETCURRENTVALUE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_RADIO_ACTION(caction->object));

   _clip_retni(ClipMachineMemory, gtk_radio_action_get_current_value(GTK_RADIO_ACTION(caction->object)));

   return 0;
 err:
   return 1;
}
