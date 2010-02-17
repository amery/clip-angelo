
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
handle_actions_changed_signal(GtkUIManager * manager, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static gint
handle_add_widget_signal(GtkUIManager * manager, GtkWidget * wid, C_signal * cs)
{
   C_widget *cwid;
   OBJECTPREPARECV(cs, cv);
   cwid = _list_get_cwidget(cs->co->cmachine, wid);
   if (!cwid)
      cwid = _register_widget(cs->co->cmachine, wid, NULL);
   if (cwid)
      _clip_madd(cs->co->cmachine, &cv, HASH_WIDGET, &cwid->obj);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static gint
handle_proxy_signal(GtkUIManager * manager, GtkAction * action, GtkWidget * wid, C_signal * cs)
{
   C_object *cact;
   C_widget *cwid;
   OBJECTPREPARECV(cs, cv);
   cact = _list_get_cobject(cs->co->cmachine, action);
   if (!cact)
      cact = _register_object(cs->co->cmachine, action, GTK_TYPE_ACTION, NULL, NULL);
   if (cact)
      _clip_madd(cs->co->cmachine, &cv, HASH_ACTION, &cact->obj);
   cwid = _list_get_cwidget(cs->co->cmachine, wid);
   if (!cwid)
      cwid = _register_widget(cs->co->cmachine, wid, NULL);
   if (cwid)
      _clip_madd(cs->co->cmachine, &cv, HASH_WIDGET, &cwid->obj);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static gint
handle_activate_signal(GtkUIManager * manager, GtkAction * action, C_signal * cs)
{
   C_object *cact;
   OBJECTPREPARECV(cs, cv);
   cact = _list_get_cobject(cs->co->cmachine, action);
   if (!cact)
      cact = _register_object(cs->co->cmachine, action, GTK_TYPE_ACTION, NULL, NULL);
   if (cact)
      _clip_madd(cs->co->cmachine, &cv, HASH_ACTION, &cact->obj);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable ui_manager_signals[] = {
   {"actions-changed", GSF(handle_actions_changed_signal),
    ESF(object_emit_signal), GTK_ACTIONS_CHANGED_SIGNAL},
   {"add-widget", GSF(handle_add_widget_signal), ESF(object_emit_signal),
    GTK_ADD_WIDGET_SIGNAL},
   {"connect-proxy", GSF(handle_proxy_signal), ESF(object_emit_signal),
    GTK_CONNECT_PROXY_SIGNAL},
   {"disconnect-proxy", GSF(handle_proxy_signal), ESF(object_emit_signal),
    GTK_DISCONNECT_PROXY_SIGNAL},
   {"post-activate", GSF(handle_activate_signal), ESF(object_emit_signal),
    GTK_POST_ACTIVATE_SIGNAL},
   {"pre-activate", GSF(handle_activate_signal), ESF(object_emit_signal),
    GTK_PRE_ACTIVATE_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_ui_manager()
{
   return GTK_TYPE_UI_MANAGER;
}

long
_clip_type_ui_manager()
{
   return GTK_OBJECT_UI_MANAGER;
}

const char *
_clip_type_name_ui_manager()
{
   return "GTK_OBJECT_UI_MANAGER";
}

int
clip_INIT___UIMANAGER(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_ui_manager, _clip_type_name_ui_manager, _gtk_type_ui_manager, NULL, ui_manager_signals);
   return 0;
}

int
clip_GTK_UIMANAGERNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_object *cmanager;
   GtkUIManager *manager;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   manager = gtk_ui_manager_new();

   if (manager)
      {
	 cmanager = _list_get_cobject(ClipMachineMemory, manager);
	 if (!cmanager)
	    cmanager = _register_object(ClipMachineMemory, manager, GTK_TYPE_UI_MANAGER, cv, NULL);
	 if (cmanager)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cmanager->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERSETADDTEAROFFS(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   gboolean add_tearoffs = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_ui_manager_set_add_tearoffs(GTK_UI_MANAGER(cmanager->object), add_tearoffs);

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERGETADDTEAROFFS(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));

   _clip_retl(ClipMachineMemory, gtk_ui_manager_get_add_tearoffs(GTK_UI_MANAGER(cmanager->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERINSERTACTIONGROUP(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   C_object *cgaction = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint pos = _clip_parni(ClipMachineMemory, 3);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cgaction, GTK_IS_ACTION_GROUP(cgaction->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   if (pos > -1)
      pos--;

   gtk_ui_manager_insert_action_group(GTK_UI_MANAGER(cmanager->object), GTK_ACTION_GROUP(cgaction->object), pos);

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERREMOVEACTIONGROUP(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   C_object *cgaction = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cgaction, GTK_IS_ACTION_GROUP(cgaction->object));

   gtk_ui_manager_remove_action_group(GTK_UI_MANAGER(cmanager->object), GTK_ACTION_GROUP(cgaction->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERGETACTIONGROUPS(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   GList *list;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));

   list = gtk_ui_manager_get_action_groups(GTK_UI_MANAGER(cmanager->object));

   if (list)
      {
	 ClipVar *cv = RETPTR(ClipMachineMemory);
	 long n = g_list_length(list);

	 _clip_array(ClipMachineMemory, cv, 1, &n);
	 for (n = 0; list; list = g_list_next(list))
	    {
	       C_object *cgaction;
	       GtkActionGroup *gaction;

	       gaction = list->data;
	       cgaction = _list_get_cobject(ClipMachineMemory, gaction);
	       if (!cgaction)
		  cgaction = _register_object(ClipMachineMemory, gaction, GTK_TYPE_ACTION_GROUP, NULL, NULL);
	       if (cgaction)
		  _clip_aset(ClipMachineMemory, cv, &cgaction->obj, 1, &n);
	    }
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERGETACCELGROUP(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   GtkAccelGroup *agroup;
   C_widget *cagroup;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));

   agroup = gtk_ui_manager_get_accel_group(GTK_UI_MANAGER(cmanager->object));

   if (agroup)
      {
	 cagroup = _list_get_cwidget(ClipMachineMemory, agroup);
	 if (!cagroup)
	    cagroup = _register_widget(ClipMachineMemory, GTK_WIDGET(agroup), NULL);
	 if (cagroup)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cagroup->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERGETWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   gchar *path = _clip_parc(ClipMachineMemory, 2);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   wid = gtk_ui_manager_get_widget(GTK_UI_MANAGER(cmanager->object), path);

   if (wid)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, wid);
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, wid, NULL);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERGETTOPLEVELS(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   GtkUIManagerItemType type = _clip_parni(ClipMachineMemory, 2);
   GSList *list;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   list = gtk_ui_manager_get_toplevels(GTK_UI_MANAGER(cmanager->object), type);

   if (list)
      {
	 ClipVar *cv = RETPTR(ClipMachineMemory);
	 long n = g_slist_length(list);

	 _clip_array(ClipMachineMemory, cv, 1, &n);
	 for (n = 0; list; list = g_slist_next(list))
	    {
	       C_widget *cwid;
	       GtkWidget *wid;

	       wid = list->data;
	       cwid = _list_get_cwidget(ClipMachineMemory, wid);
	       if (!cwid)
		  cwid = _register_widget(ClipMachineMemory, wid, NULL);
	       if (cwid)
		  _clip_aset(ClipMachineMemory, cv, &cwid->obj, 1, &n);
	    }
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERGETACTION(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   gchar *path = _clip_parc(ClipMachineMemory, 2);
   GtkAction *action;
   C_object *caction;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   action = gtk_ui_manager_get_action(GTK_UI_MANAGER(cmanager->object), path);

   if (action)
      {
	 caction = _list_get_cobject(ClipMachineMemory, action);
	 if (!caction)
	    caction = _register_object(ClipMachineMemory, action, GTK_TYPE_ACTION, NULL, NULL);
	 if (caction)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &caction->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERADDUIFROMSTRING(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   gchar *buffer = _clip_parc(ClipMachineMemory, 2);
   gssize length = _clip_parni(ClipMachineMemory, 3);
   GError *error;
   guint ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   ret = gtk_ui_manager_add_ui_from_string(GTK_UI_MANAGER(cmanager->object), buffer, length, &error);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERADDUIFROMFILE(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   gchar *filename = _clip_parc(ClipMachineMemory, 2);
   GError *error;
   guint ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   ret = gtk_ui_manager_add_ui_from_file(GTK_UI_MANAGER(cmanager->object), filename, &error);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERNEWMERGEID(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   guint ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));

   ret = gtk_ui_manager_new_merge_id(GTK_UI_MANAGER(cmanager->object));

   _clip_retni(ClipMachineMemory, ret);

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERADDUI(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   guint merge_id = _clip_parni(ClipMachineMemory, 2);
   gchar *path = _clip_parc(ClipMachineMemory, 3);
   gchar *name = _clip_parc(ClipMachineMemory, 4);
   gchar *action = _clip_parc(ClipMachineMemory, 5);
   GtkUIManagerItemType type = _clip_parni(ClipMachineMemory, 6);
   gboolean top = _clip_parl(ClipMachineMemory, 7);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   CHECKARG(4, CHARACTER_type_of_ClipVarType);
   CHECKARG(5, CHARACTER_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, LOGICAL_type_of_ClipVarType);

   gtk_ui_manager_add_ui(GTK_UI_MANAGER(cmanager->object), merge_id, path, name, action, type, top);

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERREMOVEUI(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);
   guint merge_id = _clip_parni(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_ui_manager_remove_ui(GTK_UI_MANAGER(cmanager->object), merge_id);

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERGETUI(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));

   _clip_retc(ClipMachineMemory, gtk_ui_manager_get_ui(GTK_UI_MANAGER(cmanager->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_UIMANAGERENSUREUPDATE(ClipMachine * ClipMachineMemory)
{
   C_object *cmanager = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmanager, GTK_IS_UI_MANAGER(cmanager->object));

   gtk_ui_manager_ensure_update(GTK_UI_MANAGER(cmanager->object));

   return 0;
 err:
   return 1;
}
