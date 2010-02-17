
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
handle_proxy_signal(GtkActionGroup * actiongr, GtkAction * action, GtkWidget * wid, C_signal * cs)
{
   C_object *caction;
   C_widget *cwid;
   OBJECTPREPARECV(cs, cv);
   caction = _list_get_cobject(cs->cw->cmachine, action);
   if (!caction)
      caction = _register_object(cs->cw->cmachine, action, GTK_TYPE_ACTION, NULL, NULL);
   _clip_madd(cs->cw->cmachine, &cv, HASH_ACTION, &caction->obj);

   cwid = _list_get_cwidget(cs->cw->cmachine, wid);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, wid, NULL);
   _clip_madd(cs->cw->cmachine, &cv, HASH_PROXY, &cwid->obj);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static gint
handle_activate_signal(GtkActionGroup * actiongr, GtkAction * action, C_signal * cs)
{
   C_object *caction;
   OBJECTPREPARECV(cs, cv);
   caction = _list_get_cobject(cs->cw->cmachine, action);
   if (!caction)
      caction = _register_object(cs->cw->cmachine, action, GTK_TYPE_ACTION, NULL, NULL);
   _clip_madd(cs->cw->cmachine, &cv, HASH_ACTION, &caction->obj);

   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable action_group_signals[] = {
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
_gtk_type_action_group()
{
   return GTK_TYPE_ACTION_GROUP;
}

long
_clip_type_action_group()
{
   return GTK_OBJECT_ACTION_GROUP;
}

const char *
_clip_type_name_action_group()
{
   return "GTK_OBJECT_ACTION_GROUP";
}

int
clip_INIT___ACTIONGROUP(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_action_group, _clip_type_name_action_group, _gtk_type_action_group, NULL, action_group_signals);
   return 0;
}

int
clip_GTK_ACTIONGROUPNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   C_object *cagroup;
   GtkActionGroup *agroup;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);

   agroup = gtk_action_group_new(name);

   if (agroup)
      {
	 cagroup = _list_get_cobject(ClipMachineMemory, agroup);
	 if (!cagroup)
	    cagroup = _register_object(ClipMachineMemory, agroup, GTK_TYPE_ACTION_GROUP, cv, NULL);
	 if (cagroup)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cagroup->obj);
      }

   FREE_TEXT(name);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPGETNAME(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   gchar *name;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));

   name = (gchar *) gtk_action_group_get_name(GTK_ACTION_GROUP(cagroup->object));

   LOCALE_FROM_UTF(name);

   _clip_retc(ClipMachineMemory, name);

   FREE_TEXT(name);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPGETSENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));

   _clip_retl(ClipMachineMemory, gtk_action_group_get_sensitive(GTK_ACTION_GROUP(cagroup->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPSETSENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   gboolean sensitive = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_action_group_set_sensitive(GTK_ACTION_GROUP(cagroup->object), sensitive);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPGETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));

   _clip_retl(ClipMachineMemory, gtk_action_group_get_visible(GTK_ACTION_GROUP(cagroup->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPSETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   gboolean visible = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_action_group_set_visible(GTK_ACTION_GROUP(cagroup->object), visible);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPGETACTION(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   GtkAction *action;
   C_object *caction;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   action = gtk_action_group_get_action(GTK_ACTION_GROUP(cagroup->object), name);

   if (action)
      {
	 caction = _list_get_cobject(ClipMachineMemory, action);
	 if (!caction)
	    caction = _register_object(ClipMachineMemory, action, GTK_TYPE_ACTION, NULL, NULL);
	 if (caction)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &caction->obj);
      }

   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPLISTACTIONS(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   GList *list;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));

   list = gtk_action_group_list_actions(GTK_ACTION_GROUP(cagroup->object));

   if (list)
      {
	 ClipVar *cv = RETPTR(ClipMachineMemory);
	 long n = g_list_length(list);

	 _clip_array(ClipMachineMemory, cv, 1, &n);
	 for (n = 0; list; list = g_list_next(list))
	    {
	       C_object *caction;
	       GtkAction *action;

	       action = list->data;
	       caction = _list_get_cobject(ClipMachineMemory, action);
	       if (!caction)
		  caction = _register_object(ClipMachineMemory, action, GTK_TYPE_ACTION, NULL, NULL);
	       if (caction)
		  _clip_aset(ClipMachineMemory, cv, &caction->obj, 1, &n);
	    }
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPADDACTION(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   C_object *caction = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   gtk_action_group_add_action(GTK_ACTION_GROUP(cagroup->object), GTK_ACTION(caction->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPADDACTIONWITHACCEL(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   C_object *caction = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gchar *accel = _clip_parc(ClipMachineMemory, 3);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(accel);
   gtk_action_group_add_action_with_accel(GTK_ACTION_GROUP(cagroup->object), GTK_ACTION(caction->object), accel);

   FREE_TEXT(accel);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPREMOVEACTION(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   C_object *caction = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   gtk_action_group_remove_action(GTK_ACTION_GROUP(cagroup->object), GTK_ACTION(caction->object));

   return 0;
 err:
   return 1;
}

static void
destroy_d(gpointer data)
{
   C_var *c = (C_var *) data;
   ClipVar stack[1];
   ClipVar res;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   _clip_mclone(c->ClipMachineMemory, &stack[0], &c->co->obj);
   _clip_eval(c->ClipMachineMemory, &c->cfunc2, 1, stack, &res);

   _clip_destroy(c->ClipMachineMemory, &res);

   _clip_destroy(c->ClipMachineMemory, &stack[0]);
}

int
clip_GTK_ACTIONGROUPADDACTIONS(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *carr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   guint n_actions = _clip_parni(ClipMachineMemory, 3);
   C_var *c = NEW(C_var);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   if (carr)
      {
	 GtkActionEntry *acts;
	 gint i;

	 acts = malloc(carr->count_of_ClipArrVar * sizeof(GtkActionEntry));
	 memset(acts, 0, sizeof(GtkActionEntry) * carr->count_of_ClipArrVar);
	 for (i = 0; i < n_actions; i++)
	    _map_to_action_entry(ClipMachineMemory, &carr->ClipVar_items_of_ClipArrVar[i], &acts[i]);

	 c->ClipMachineMemory = ClipMachineMemory;
	 gtk_action_group_add_actions(GTK_ACTION_GROUP(cagroup->object), acts, n_actions, c);
	 free(acts);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPADDACTIONSFULL(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *carr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   guint n_actions = _clip_parni(ClipMachineMemory, 3);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 4);
   C_var *c = NEW(C_var);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG2(4, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   if (carr)
      {
	 GtkActionEntry *acts;
	 gint i;

	 acts = malloc(carr->count_of_ClipArrVar * sizeof(GtkActionEntry));
	 memset(acts, 0, sizeof(GtkActionEntry) * carr->count_of_ClipArrVar);
	 for (i = 0; i < n_actions; i++)
	    _map_to_action_entry(ClipMachineMemory, &carr->ClipVar_items_of_ClipArrVar[i], &acts[i]);

	 c->ClipMachineMemory = ClipMachineMemory;
	 c->co = cagroup;
	 _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
	 gtk_action_group_add_actions_full(GTK_ACTION_GROUP(cagroup->object), acts, n_actions, c, (GDestroyNotify) destroy_d);
	 free(acts);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPADDTOGGLEACTIONS(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *carr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   guint n_actions = _clip_parni(ClipMachineMemory, 3);
   C_var *c = NEW(C_var);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   if (carr)
      {
	 GtkToggleActionEntry *acts;
	 gint i;

	 acts = malloc(carr->count_of_ClipArrVar * sizeof(GtkToggleActionEntry));
	 memset(acts, 0, sizeof(GtkToggleActionEntry) * carr->count_of_ClipArrVar);
	 for (i = 0; i < n_actions; i++)
	    _map_to_toggle_action_entry(ClipMachineMemory, &carr->ClipVar_items_of_ClipArrVar[i], &acts[i]);

	 c->ClipMachineMemory = ClipMachineMemory;
	 gtk_action_group_add_toggle_actions(GTK_ACTION_GROUP(cagroup->object), acts, n_actions, c);
	 free(acts);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPADDTOGGLEACTIONSFULL(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *carr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   guint n_actions = _clip_parni(ClipMachineMemory, 3);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 4);
   C_var *c = NEW(C_var);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG2(4, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   if (carr)
      {
	 GtkToggleActionEntry *acts;
	 gint i;

	 acts = malloc(carr->count_of_ClipArrVar * sizeof(GtkToggleActionEntry));
	 memset(acts, 0, sizeof(GtkToggleActionEntry) * carr->count_of_ClipArrVar);
	 for (i = 0; i < n_actions; i++)
	    _map_to_toggle_action_entry(ClipMachineMemory, &carr->ClipVar_items_of_ClipArrVar[i], &acts[i]);

	 c->ClipMachineMemory = ClipMachineMemory;
	 c->co = cagroup;
	 _clip_mclone(ClipMachineMemory, &c->cfunc2, cfunc);
	 gtk_action_group_add_toggle_actions_full(GTK_ACTION_GROUP(cagroup->object), acts, n_actions, c, (GDestroyNotify) destroy_d);
	 free(acts);
      }

   return 0;
 err:
   return 1;
}

static void
call_b(gpointer data)
{
   C_var *c = (C_var *) data;
   ClipVar stack[1];
   ClipVar res;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   _clip_mclone(c->ClipMachineMemory, &stack[0], &c->co->obj);
   _clip_eval(c->ClipMachineMemory, &c->cfunc, 1, stack, &res);

   _clip_destroy(c->ClipMachineMemory, &res);

   _clip_destroy(c->ClipMachineMemory, &stack[0]);
}

int
clip_GTK_ACTIONGROUPADDRADIOACTIONS(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *carr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   guint n_actions = _clip_parni(ClipMachineMemory, 3);
   gint value = _clip_parni(ClipMachineMemory, 4);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 5);
   C_var *c = NEW(C_var);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG2(5, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   if (carr)
      {
	 GtkRadioActionEntry *acts;
	 gint i;

	 acts = malloc(carr->count_of_ClipArrVar * sizeof(GtkRadioActionEntry));
	 memset(acts, 0, sizeof(GtkRadioActionEntry) * carr->count_of_ClipArrVar);
	 for (i = 0; i < n_actions; i++)
	    _map_to_radio_action_entry(ClipMachineMemory, &carr->ClipVar_items_of_ClipArrVar[i], &acts[i]);

	 c->ClipMachineMemory = ClipMachineMemory;
	 c->co = cagroup;
	 _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
	 gtk_action_group_add_radio_actions(GTK_ACTION_GROUP(cagroup->object), acts, n_actions, value, (GCallback) call_b, c);
	 free(acts);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPADDRADIOACTIONSFULL(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *carr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   guint n_actions = _clip_parni(ClipMachineMemory, 3);
   gint value = _clip_parni(ClipMachineMemory, 4);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 5);
   ClipVar *cfunc2 = _clip_spar(ClipMachineMemory, 6);
   C_var *c = NEW(C_var);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG2(5, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   CHECKARG2(6, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   if (carr)
      {
	 GtkRadioActionEntry *acts;
	 gint i;

	 acts = malloc(carr->count_of_ClipArrVar * sizeof(GtkRadioActionEntry));
	 memset(acts, 0, sizeof(GtkRadioActionEntry) * carr->count_of_ClipArrVar);
	 for (i = 0; i < n_actions; i++)
	    _map_to_radio_action_entry(ClipMachineMemory, &carr->ClipVar_items_of_ClipArrVar[i], &acts[i]);

	 c->ClipMachineMemory = ClipMachineMemory;
	 c->co = cagroup;
	 _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
	 _clip_mclone(ClipMachineMemory, &c->cfunc2, cfunc2);
	 gtk_action_group_add_radio_actions_full(GTK_ACTION_GROUP(cagroup->object), acts, n_actions, value, (GCallback) call_b, c, (GDestroyNotify) destroy_d);
	 free(acts);
      }

   return 0;
 err:
   return 1;
}

static gchar *
translate_func(const gchar * path, gpointer data)
{
   C_var *c = (C_var *) data;
   ClipVar stack[2];
   ClipVar str;
   ClipVar res;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   _clip_mclone(c->ClipMachineMemory, &stack[0], &c->co->obj);
   _clip_var_str(path, strlen(path), &str);
   _clip_mclone(c->ClipMachineMemory, &stack[1], &str);
   _clip_eval(c->ClipMachineMemory, &c->cfunc, 2, stack, &res);

   _clip_destroy(c->ClipMachineMemory, &res);
   _clip_destroy(c->ClipMachineMemory, &str);

   _clip_destroy(c->ClipMachineMemory, &stack[0]);

	if (res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
		return res.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
   else
      return NULL;

}

static void
destroy_notify(gpointer data)
{
   C_var *c = (C_var *) data;
   ClipVar stack[1];
   ClipVar res;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   _clip_mclone(c->ClipMachineMemory, &stack[0], &c->co->obj);
   _clip_eval(c->ClipMachineMemory, &c->cfunc, 1, stack, &res);

   _clip_destroy(c->ClipMachineMemory, &res);

   _clip_destroy(c->ClipMachineMemory, &stack[0]);
}

int
clip_GTK_ACTIONGROUPSETTRANSLATEFUNC(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 2);
   ClipVar *cfunc2 = _clip_spar(ClipMachineMemory, 3);
   C_var *c = NEW(C_var);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   CHECKARG2(3, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->co = cagroup;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
   _clip_mclone(ClipMachineMemory, &c->cfunc2, cfunc2);
   gtk_action_group_set_translate_func(GTK_ACTION_GROUP(cagroup->object), (GtkTranslateFunc) translate_func, c, (GtkDestroyNotify) destroy_notify);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGROUPSETTRANSLATIONDOMAIN(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   gchar *domain = _clip_parc(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_action_group_set_translation_domain(GTK_ACTION_GROUP(cagroup->object), domain);

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_ACTIONGROUPTRANSLATESTRING(ClipMachine * ClipMachineMemory)
{
   C_object *cagroup = _fetch_co_arg(ClipMachineMemory);
   gchar *string = _clip_parc(ClipMachineMemory, 2);
   gchar *resstr;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cagroup, GTK_IS_ACTION_GROUP(cagroup->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(string);
   resstr = (gchar *) gtk_action_group_translate_string(GTK_ACTION_GROUP(cagroup->object), string);
   LOCALE_FROM_UTF(resstr);
   _clip_retc(ClipMachineMemory, resstr);

   FREE_TEXT(string);
   FREE_TEXT(resstr);

   return 0;
 err:
   return 1;
}

#endif
